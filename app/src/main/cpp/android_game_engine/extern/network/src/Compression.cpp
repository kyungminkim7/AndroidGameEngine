#include <network/Compression.h>

#include <libjpeg-turbo/turbojpeg.h>
#include <sensor_msgs/Image_generated.h>
#include <std_msgs/Uint8Array_generated.h>

#include <network/Image.h>

namespace ntwk {
namespace Compression {

std::shared_ptr<flatbuffers::DetachedBuffer> IdentityPolicy::compressMsg(std::shared_ptr<flatbuffers::DetachedBuffer> msg) {
    return std::move(msg);
}

std::unique_ptr<uint8_t[]> IdentityPolicy::decompressMsg(std::unique_ptr<uint8_t[]> msgBuffer) {
    return std::move(msgBuffer);
}

namespace Image {

std::shared_ptr<flatbuffers::DetachedBuffer> IdentityPolicy::compressMsg(unsigned int width, unsigned int height,
                                                                         uint8_t channels, const uint8_t data[]) {
    const auto size = width * height * channels;
    flatbuffers::FlatBufferBuilder imgMsgBuilder(size + 100);
    auto imgMsgData = imgMsgBuilder.CreateVector(data, size);
    auto imgMsg = sensor_msgs::CreateImage(imgMsgBuilder, width, height, channels, imgMsgData);
    imgMsgBuilder.Finish(imgMsg);
    return std::make_shared<flatbuffers::DetachedBuffer>(imgMsgBuilder.Release());
}

std::unique_ptr<ntwk::Image> IdentityPolicy::decompressMsg(std::unique_ptr<uint8_t[]> msgBuffer) {
    auto imgMsg = sensor_msgs::GetImage(msgBuffer.get());
    auto img = std::make_unique<ntwk::Image>();
    img->width = imgMsg->width();
    img->height = imgMsg->height();
    img->data = std::make_unique<uint8_t[]>(imgMsg->data()->size());
    std::copy(imgMsg->data()->cbegin(), imgMsg->data()->cend(), img->data.get());
    return std::move(img);
}

std::shared_ptr<flatbuffers::DetachedBuffer> JpegPolicy::compressMsg(unsigned int width, unsigned int height,
                                                                     uint8_t channels, const uint8_t data[]) {
    int format;
    switch (channels) {
    case 1:
        format = TJPF_GRAY;
        break;
    case 3:
        format = TJPF_RGB;
        break;
    case 4:
        format = TJPF_RGBA;
        break;
    default:
        return nullptr;
    }

    const auto subsample = TJSAMP_444;
    const auto quality = 75;

    auto jpegSize = tjBufSize(width, height, subsample);
    if (jpegSize <= 0) {
        return nullptr;
    }

    // Compress image
    auto compressor = tjInitCompress();
    if (compressor == NULL) {
        return nullptr;
    }

    auto jpeg = std::make_unique<uint8_t[]>(jpegSize);
    auto pJpeg = jpeg.get();

    auto result = tjCompress2(compressor, data, width, 0, height, format,
                              &pJpeg, &jpegSize, subsample, quality,
                              TJFLAG_FASTDCT | TJFLAG_NOREALLOC);
    tjDestroy(compressor);

    if (result != 0) {
        return nullptr;
    }

    // Build message
    flatbuffers::FlatBufferBuilder msgBuilder(jpegSize + 100);
    auto jpegMsgData = msgBuilder.CreateVector(jpeg.get(), jpegSize);
    auto jpegMsg = std_msgs::CreateUint8Array(msgBuilder, jpegMsgData);
    msgBuilder.Finish(jpegMsg);

    return std::make_shared<flatbuffers::DetachedBuffer>(msgBuilder.Release());
}

std::unique_ptr<ntwk::Image> JpegPolicy::decompressMsg(std::unique_ptr<uint8_t[]> jpegMsgBuffer) {
    // Initialize decompressor
    auto decompressor = tjInitDecompress();
    if (decompressor == NULL) {
        return nullptr;
    }

    // Get jpeg image properties
    auto jpegMsg = std_msgs::GetUint8Array(jpegMsgBuffer.get());
    int width, height, subsample, colorspace;
    if (tjDecompressHeader3(decompressor, jpegMsg->data()->data(), jpegMsg->data()->size(),
                            &width, &height, &subsample, &colorspace) != 0) {
        tjDestroy(decompressor);
        return nullptr;
    }

    uint8_t channels;
    int format;
    switch (colorspace) {
    case TJCS_GRAY:
        channels = 1;
        format = TJPF_GRAY;
        break;

    case TJCS_YCbCr:
    case TJCS_RGB:
        channels = 3;
        format = TJPF_RGB;
        break;

    default:
        tjDestroy(decompressor);
        return nullptr;
    }

    // Decompress image
    auto img = std::make_unique<ntwk::Image>();
    img->width = width;
    img->height = height;
    img->channels = channels;
    img->data = std::make_unique<uint8_t[]>(width * height * channels);

    auto result = tjDecompress2(decompressor, jpegMsg->data()->data(), jpegMsg->data()->size(),
                                img->data.get(), width, 0, height, format, TJFLAG_FASTDCT | TJFLAG_NOREALLOC);
    tjDestroy(decompressor);

    return result == 0 ? std::move(img) : nullptr;
}

} // namespace Image
} // namespace Compression
} // namespace ntwk
