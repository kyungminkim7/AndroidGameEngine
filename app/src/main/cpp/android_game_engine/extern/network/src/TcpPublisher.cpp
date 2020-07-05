#include <network/TcpPublisher.h>

#include <asio/read.hpp>
#include <asio/write.hpp>

#include <sensor_msgs/Image_generated.h>

#include <iostream>

namespace ntwk {

using namespace asio::ip;

std::shared_ptr<TcpPublisher> TcpPublisher::create(asio::io_context &publisherContext,
                                                   unsigned short port,
                                                   Compression compression) {
    std::shared_ptr<TcpPublisher> publisher(new TcpPublisher(publisherContext, port, compression));
    publisher->listenForConnections();
    return publisher;
}

TcpPublisher::TcpPublisher(asio::io_context &publisherContext, unsigned short port, Compression compression) :
    publisherContext(publisherContext),
    socketAcceptor(publisherContext, tcp::endpoint(tcp::v4(), port)),
    compression(compression) { }

void TcpPublisher::listenForConnections() {
    auto socket = std::make_unique<tcp::socket>(this->publisherContext);
    auto pSocket = socket.get();

    // Save connected sockets for later publishing and listen for more connections
    this->socketAcceptor.async_accept(*pSocket,
                                      [publisher=shared_from_this(), socket=std::move(socket)](const auto &error) mutable {
        if (error) {
            throw asio::system_error(error);
        }

        publisher->connectedSockets.emplace_back(std::move(socket));
        publisher->listenForConnections();
    });
}

void TcpPublisher::removeSocket(Socket *socket) {
    for (auto iter = this->connectedSockets.cbegin(); iter != this->connectedSockets.cend(); ) {
        if (iter->socket.get() == socket->socket.get()) {
            iter = this->connectedSockets.erase(iter);
            return;
        } else {
            ++iter;
        }
    }
}

void TcpPublisher::publish(std::shared_ptr<flatbuffers::DetachedBuffer> msg) {
    asio::post(this->publisherContext, [publisher=shared_from_this(), msg=std::move(msg)]() mutable {
        // Compress (if applicable)
        switch (publisher->compression) {
        case Compression::ZLIB:
            msg = zlib::encodeMsg(msg.get());
            break;

        default:
            break;
        }

        // Send msg
        auto msgHeader = std::make_shared<std_msgs::Header>(msg->size());

        for (auto &s : publisher->connectedSockets) {
            if (s.readyToWrite) {
                s.readyToWrite = false;
                publisher->sendMsgHeader(publisher, &s, msgHeader, msg, 0u);
            }
        }
    });
}

void TcpPublisher::publishImage(unsigned int width, unsigned int height,
                                uint8_t channels, const uint8_t data[]) {
    switch (this->compression) {
    case Compression::JPEG:
        this->publish(jpeg::encodeMsg(width, height, channels, data));
        break;
    default: {
            const auto size = width * height * channels;
            flatbuffers::FlatBufferBuilder imgMsgBuilder(size + 100);
            auto imgMsgData = imgMsgBuilder.CreateVector(data, size);
            auto imgMsg = sensor_msgs::CreateImage(imgMsgBuilder, width, height, channels, imgMsgData);
            imgMsgBuilder.Finish(imgMsg);
            this->publish(std::make_shared<flatbuffers::DetachedBuffer>(imgMsgBuilder.Release()));
        }
        break;
    }
}

void TcpPublisher::sendMsgHeader(std::shared_ptr<ntwk::TcpPublisher> publisher, Socket *socket,
                                 std::shared_ptr<const std_msgs::Header> msgHeader,
                                 std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                                 unsigned int totalMsgHeaderBytesTransferred) {
    // Publish msg header
    auto pMsgHeader = reinterpret_cast<const uint8_t*>(msgHeader.get());
    asio::async_write(*socket->socket, asio::buffer(pMsgHeader + totalMsgHeaderBytesTransferred,
                                                    sizeof(std_msgs::Header) - totalMsgHeaderBytesTransferred),
                      [publisher=std::move(publisher), socket,
                      msgHeader=std::move(msgHeader), msg=std::move(msg),
                      totalMsgHeaderBytesTransferred](const auto &error, auto bytesTransferred) mutable {
        // Tear down socket if fatal error
        if (error) {
            publisher->removeSocket(socket);
            return;
        }

        // Send the rest of the header if it was only partially sent
        totalMsgHeaderBytesTransferred += bytesTransferred;
        if (totalMsgHeaderBytesTransferred < sizeof(std_msgs::Header)) {
            sendMsgHeader(std::move(publisher), socket, std::move(msgHeader), std::move(msg), totalMsgHeaderBytesTransferred);
            return;
        }

        // Send the msg
        sendMsg(std::move(publisher), socket, std::move(msg), 0u);
    });
}

void TcpPublisher::sendMsg(std::shared_ptr<ntwk::TcpPublisher> publisher, Socket *socket,
                           std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                           unsigned int totalMsgBytesTransferred) {
    auto pMsg = msg.get();
    asio::async_write(*socket->socket, asio::buffer(pMsg->data() + totalMsgBytesTransferred,
                                                    pMsg->size() - totalMsgBytesTransferred),
                      [publisher=std::move(publisher), socket, msg=std::move(msg),
                      totalMsgBytesTransferred](const auto &error, auto bytesTransferred) mutable {
        // Tear down socket if fatal error
        if (error) {
            publisher->removeSocket(socket);
            return;
        }

        // Send the rest of the msg if it was only partially sent
        totalMsgBytesTransferred += bytesTransferred;
        if (totalMsgBytesTransferred < msg->size()) {
            sendMsg(std::move(publisher), socket, std::move(msg), totalMsgBytesTransferred);
            return;
        }

        // Wait for ack signal from subscriber
        receiveMsgControl(std::move(publisher), socket,
                          std::make_unique<std_msgs::MessageControl>(), 0u);
    });
}

void TcpPublisher::receiveMsgControl(std::shared_ptr<TcpPublisher> publisher, Socket *socket,
                                     std::unique_ptr<std_msgs::MessageControl> msgCtrl,
                                     unsigned int totalMsgCtrlBytesReceived) {
    auto pMsgCtrl = reinterpret_cast<uint8_t*>(msgCtrl.get());
    asio::async_read(*socket->socket, asio::buffer(pMsgCtrl + totalMsgCtrlBytesReceived,
                                                   sizeof(std_msgs::MessageControl) - totalMsgCtrlBytesReceived),
                     [publisher=std::move(publisher), socket, msgCtrl=std::move(msgCtrl),
                     totalMsgCtrlBytesReceived](const auto &error, auto bytesReceived) mutable {
        // Tear down socket if fatal error
        if (error) {
            publisher->removeSocket(socket);
            return;
        }

        // Receive the rest of the msg ctrl if it was only partially received
        totalMsgCtrlBytesReceived += bytesReceived;
        if (totalMsgCtrlBytesReceived < sizeof(std_msgs::MessageControl)) {
            receiveMsgControl(std::move(publisher), socket,
                              std::move(msgCtrl), totalMsgCtrlBytesReceived);
            return;
        }

        if (*msgCtrl != std_msgs::MessageControl::ACK) {
            // Reset the connection if a successful Ack signal is not received
            publisher->removeSocket(socket);
        } else {
            socket->readyToWrite = true;
        }
    });
}

} // namespace ntwk
