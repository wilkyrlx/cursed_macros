#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#define CURSE_CHAR L'₻'

int converter(const char* path, wchar_t curse_char) {
    // Open files with UTF-8 encoding
    std::ifstream inputFile(path, std::ios::in | std::ios::binary);
    std::ofstream outputFile("output.txt", std::ios::out | std::ios::binary);

    // Make sure files were opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file\n";
        return 1;
    }
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Do whatever processing you need on the line here
        line += curse_char;
        line += "\n";

        // Write the line to the output file
        outputFile << line;
    }

    // Close files when done
    inputFile.close();
    outputFile.close();

    std::cout << "File converted successfully";

    return 0;
}


int main(int argc, char const *argv[])
{
    std::cout << "Main running...\n";
    if(argc < 2) {
        std::cerr << "Incorrect arguments, usage: <path> [optional_character]\n";
        return 1;
    }

    // sets curse_char to default unless optional_character argument is set and valid
    // TODO: does not handle unicode right now, must debug
    wchar_t curse_char = CURSE_CHAR;
    if(argc == 3) {
        curse_char = *argv[2];
    }

    converter(argv[1], curse_char);
    
    return 0;
}
