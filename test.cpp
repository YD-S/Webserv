#include <fstream>
#include <iostream>
#include <stdexcept>

class fileValidate {
public:
    std::ifstream validate_config_file() {
        std::ifstream file("config.txt");
        // Check if the file is open and handle errors if needed
        if (!file.is_open()) {
            // Handle the error, e.g., throw an exception
            throw std::runtime_error("Error opening file: config.txt");
        }
        // Return the file by reference
        return file;
    }
};

int main() {
    fileValidate validator;
    try {
        std::ifstream file = validator.validate_config_file();
        // Use the file...
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
