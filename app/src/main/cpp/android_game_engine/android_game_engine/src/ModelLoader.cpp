#include <android_game_engine/ModelLoader.h>

namespace age {

ModelLoader::ModelLoader(const std::string &filepath) : filepath(filepath) {
    auto i = filepath.find_last_of('/') + 1;
    this->directory = filepath.substr(0, i);
    this->filename = filepath.substr(i);
}

std::string ModelLoader::getFilepath() const {return this->filepath;}
std::string ModelLoader::getDirectory() const {return this->directory;}
std::string ModelLoader::getFilename() const {return this->filename;}

} // namespace age