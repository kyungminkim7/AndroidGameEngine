#include <network/Image.h>

#include <algorithm>
#include <utility>

#include <network/msgs/Image_generated.h>

namespace ntwk {

Image::Image(unsigned int width, unsigned int height, uint8_t channels,
             std::unique_ptr<uint8_t[]> &&data) :
    width(width), height(height), channels(channels), data(std::move(data)) { }

std::shared_ptr<flatbuffers::DetachedBuffer> Image::makeBuffer(unsigned int width, unsigned int height,
                                                               uint8_t channels, const uint8_t data[]) {
    const auto size = width * height * channels;
    flatbuffers::FlatBufferBuilder builder(size + 100);
    auto imageData = builder.CreateVector(data, size);
    auto image = msgs::CreateImage(builder, width, height, channels, imageData);
    builder.Finish(image);
    return std::make_shared<flatbuffers::DetachedBuffer>(builder.Release());
}

Image Image::makeImage(const uint8_t buffer[]) {
    auto imageBuffer = msgs::GetImage(buffer);
    Image image(imageBuffer->width(), imageBuffer->height(), imageBuffer->channels(),
                std::make_unique<uint8_t[]>(imageBuffer->data()->size()));
    std::copy(imageBuffer->data()->cbegin(), imageBuffer->data()->cend(),
              image.data.get());
    return image;
}

} // namespace ntwk
