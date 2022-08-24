#pragma once

#include <assimp/IOStream.hpp>

#include "Asset.h"

namespace age {

class AssimpIOStream : public Assimp::IOStream {
    friend class AssimpIOSystem;

protected:
    explicit AssimpIOStream(const std::string &pathname);

public:
    size_t Read(void *buffer, size_t size, size_t count) override;
    size_t Write(const void *buffer, size_t size, size_t count) override;
    aiReturn Seek(size_t offset, aiOrigin origin) override;
    size_t Tell() const override;
    size_t FileSize() const override;
    void Flush() override;

private:
    mutable Asset asset;
};

} // namespace age