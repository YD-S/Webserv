
#include "macros.h"
#include "ServerConfig.hpp"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        LOG_ERROR("Usage: ./webserv <config_file>");
        return 1;
    }

    LOG_INFO("Starting webserv...");
    LOG_DEBUG("Reading config file: " << argv[1]);
    char *config_file = argv[1];
    (void) config_file;

    ServerConfig config = ServerConfig()
            .addListen("localhost", 8080)
            .addListen("localhost", 8081)
            .setServerName("localhost")
            .setDefaultLocation(LocationConfig()
                    .addIndex("index.html")
                    .addIndex("index.php")
                    .setRoot("/var/www/html")
                    .setPath("/")
                    .addMethod("GET")
                    .addMethod("POST")
                    .setDirectoryListingEnabled(true)
                    .setDirectoryResponseFile("/var/www/html/.directory")
                    .setCgiEnabled(true)
                    .setCgiPath("/var/www/html/cgi-bin")
                    .setCgiExtension(".php")
                    .setUploadEnabled(true)
                    .setUploadPath("/var/www/html/uploads")
                    )
            .addLocation(LocationConfig()
                    .addIndex("index.html")
                    .addIndex("index.php")
                    .setRoot("/var/www/html")
                    .setPath("/test")
                    .addMethod("GET")
                    .addMethod("POST")
                    .setDirectoryListingEnabled(true)
                    .setDirectoryResponseFile("/var/www/html/.directory")
                    .setCgiEnabled(true)
                    .setCgiPath("/var/www/html/cgi-bin")
                    .setCgiExtension(".php")
                    .setUploadEnabled(true)
                    .setUploadPath("/var/www/html/uploads")
                    );


}