
#include "macros.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        LOG_ERROR("Usage: ./webserv <config_file>");
        return 1;
    }

    LOG_INFO("Starting webserv...");
    char *config_file = argv[1];
    LOG_DEBUG("Reading config file: " << config_file);
    (void) config_file;

}