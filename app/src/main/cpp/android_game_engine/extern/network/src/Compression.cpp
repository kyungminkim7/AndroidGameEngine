#include <network/Compression.h>

#include <libjpeg-turbo/turbojpeg.h>
#include <std_msgs/Compressed_generated.h>
#include <std_msgs/Uint8Array_generated.h>
#include <zlib/zlib.h>

#include <network/Image.h>

namespace ntwk {

namespace zlib {

std::shared_ptr<flatbuffers::DetachedBuffer> encodeMsg(flatbuffers::DetachedBuffer *msg) {
    // Initialize compression
    z_stream zStream;
    zStream.zalloc = Z_NULL;
    zStream.zfree = Z_NULL;
    zStream.opaque = Z_NULL;

    if (deflateInit(&zStream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return nullptr;
    }

    // Compress msg
    zStream.avail_in = msg->size();
    zStream.next_in = msg->data();

    const auto numAvailableBytes = deflateBound(&zStream, msg->size());
    auto compressedBytes = std::make_unique<uint8_t[]>(numAvailableBytes);
    zStream.avail_out = numAvailableBytes;
    zStream.next_out = compressedBytes.get();

    if (deflate(&zStream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&zStream);
        return nullptr;
    }

    const auto numCompressedBytes = numAvailableBytes - zStream.avail_out;

    // Release resources no longer needed
    if (deflateEnd(&zStream)!= Z_OK) {
        return nullptr;
    }

    // Build compressedMsg
    flatbuffers::FlatBufferBuilder msgBuilder(numCompressedBytes + 100);
    auto compressedMsgData = msgBuilder.CreateVector(compressedBytes.get(), numCompressedBytes);
    auto compressedMsg = std_msgs::CreateCompressed(msgBuilder, msg->size(), compressedMsgData);
    msgBuilder.Finish(compressedMsg);

    return std::make_shared<flatbuffers::DetachedBuffer>(msgBuilder.Release());
}

std::unique_ptr<uint8_t[]> decodeMsg(uint8_t compressedMsgBuffer[]) {
    // Initialize decompression
    auto compressedMsg = std_msgs::GetMutableCompressed(compressedMsgBuffer);

    z_stream zStream;
    zStream.zalloc = Z_NULL;
    zStream.zfree = Z_NULL;
    zStream.opaque = Z_NULL;
    zStream.avail_in = compressedMsg->compressedData()->size();
    zStream.next_in = compressedMsg->mutable_compressedData()->data();

    if (inflateInit(&zStream) != Z_OK) {
        return nullptr;
    }

    // Decompress msg
    auto msg = std::make_unique<uint8_t[]>(compressedMsg->uncompressedDataSize());
    zStream.avail_out = compressedMsg->uncompressedDataSize();
    zStream.next_out = msg.get();

    auto result = inflate(&zStream, Z_FINISH);
    inflateEnd(&zStream);
    return result == Z_STREAM_END ? std::move(msg) : nullptr;
}

} // namespace zlib

namespace jpeg {

std::shared_ptr<flatbuffers::DetachedBuffer> encodeMsg(unsigned int width, unsigned int height,
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

std::unique_ptr<Image> decodeMsg(const uint8_t jpegMsgBuffer[]) {
    // Initialize decompressor
    auto decompressor = tjInitDecompress();
    if (decompressor == NULL) {
        return nullptr;
    }

    // Get jpeg image properties
    auto jpegMsg = std_msgs::GetUint8Array(jpegMsgBuffer);
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
    auto img = std::make_unique<Image>();
    img->width = width;
    img->height = height;
    img->channels = channels;
    img->data = std::make_unique<uint8_t[]>(width * height * channels);

    auto result = tjDecompress2(decompressor, jpegMsg->data()->data(), jpegMsg->data()->size(),
                                img->data.get(), width, 0, height, format, TJFLAG_FASTDCT | TJFLAG_NOREALLOC);
    tjDestroy(decompressor);

    return result == 0 ? std::move(img) : nullptr;
}

} // namespace jpeg

} // namespace ntwk
