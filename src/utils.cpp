
#include <sys/stat.h>
#include "utils.hpp"

bool is_dir(const std::string& path) {
    struct stat buf = {};
    stat(path.c_str(), &buf);
    return S_ISDIR(buf.st_mode);
}