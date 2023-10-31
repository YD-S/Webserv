
#include <sys/stat.h>
#include "utils.hpp"

bool is_dir(const std::string& path) {
    struct stat buf = {};
    stat(path.c_str(), &buf);
    return S_ISDIR(buf.st_mode);
}

std::string pathToFilename(const std::string& path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}