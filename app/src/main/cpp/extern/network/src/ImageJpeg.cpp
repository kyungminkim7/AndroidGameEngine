#include <network/ImageJpeg.h>

#include <system_error>

#include <libjpeg-turbo/turbojpeg.h>

#include <network/msgs/Uint8Array_generated.h>

namespace ntwk {
namespace ImageJpeg {

std::shared_ptr<flatbuffers::DetachedBuffer> compressImage(unsigned int width, unsigned int height,
                                                           uint8_t channels, const uint8_t data[],
                                                           int quality) {
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

    auto jpegSize = tjBufSize(width, height, subsample);
    if (jpegSize <= 0) {
        throw std::system_error(std::make_error_code(std::errc::invalid_argument),
                                "Failed to get jpeg size");
    }

    // Compress image
    std::shared_ptr<void> compressor(tjInitCompress(), tjDestroy);
    if (!compressor) {
        throw std::system_error(std::make_error_code(std::errc::resource_unavailable_try_again),
                                "Failed to create compressor");
    }

    auto jpeg = std::make_unique<uint8_t[]>(jpegSize);
    auto pJpeg = jpeg.get();

    auto result = tjCompress2(compressor.get(), data, width, 0, height, format,
                              &pJpeg, &jpegSize, subsample, quality,
                              TJFLAG_FASTDCT | TJFLAG_NOREALLOC);

    if (result != 0) {
        throw std::system_error(std::make_error_code(std::errc::protocol_error),
                                "Failed to compress image");
    }

    // Build message
    flatbuffers::FlatBufferBuilder msgBuilder(jpegSize + 100);
    auto jpegMsgData = msgBuilder.CreateVector(jpeg.get(), jpegSize);
    auto jpegMsg = msgs::CreateUint8Array(msgBuilder, jpegMsgData);
    msgBuilder.Finish(jpegMsg);

    return std::make_shared<flatbuffers::DetachedBuffer>(msgBuilder.Release());
}

Image decompressImage(const uint8_t jpegBuffer[]) {
    std::shared_ptr<void> decompressor(tjInitDecompress(), tjDestroy);

    // Get jpeg image properties
    auto jpeg = msgs::GetUint8Array(jpegBuffer);
    int width, height, subsample, colorspace;
    if (tjDecompressHeader3(decompressor.get(), jpeg->data()->data(), jpeg->data()->size(),
                            &width, &height, &subsample, &colorspace) != 0) {
        throw std::system_error(std::make_error_code(std::errc::protocol_error),
                                "Failed to decompress header");
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
        throw std::system_error(std::make_error_code(std::errc::protocol_error),
                                "Failed to detect correct colorspace");
    }

    // Decompress image
    Image image(width, height, channels,
                std::make_unique<uint8_t[]>(width * height * channels));
    auto result = tjDecompress2(decompressor.get(), jpeg->data()->data(), jpeg->data()->size(),
                                image.data.get(), width, 0, height, format,
                                TJFLAG_FASTDCT | TJFLAG_NOREALLOC);
    if (result != 0) {
        throw std::system_error(std::make_error_code(std::errc::protocol_error),
                                "Failed to decompress img");
    }
    return image;
}

} // namespace ImageJpeg
} // namespace ntwk
