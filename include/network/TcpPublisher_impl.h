#include <asio/write.hpp>
#include <flatbuffers/flatbuffers.h>

#include <std_msgs/Header_generated.h>

namespace ntwk {

template<typename T>
void TcpPublisher::publish(std::shared_ptr<T> msg) {
    auto msgHeader = std::make_shared<std_msgs::Header>(sizeof(T));

    {
        std::lock_guard<std::mutex> guard(this->socketsMutex);
        for (auto &socket : this->connectedSockets) {
            // Publish msg header
            asio::async_write(*socket, asio::buffer(msgHeader.get(), sizeof(std_msgs::Header)),
                              [publisher=shared_from_this(), socket=socket.get(), msgHeader, msg](const auto &error, auto bytesTransferred) mutable {
                if (error) {
                    publisher->removeSocket(socket);
                    return;
                }

                // Publish msg
                auto pMsg = msg.get();
                asio::async_write(*socket, asio::buffer(pMsg, sizeof(T)),
                                  [publisher=std::move(publisher), socket, msg=std::move(msg)](const auto &error, auto bytesTransferred){
                    if (error) {
                        publisher->removeSocket(socket);
                        return;
                    }
                });
            });
        }
    }
}

template<>
void TcpPublisher::publish(std::shared_ptr<flatbuffers::DetachedBuffer> msg);

} // namespace ntwk
