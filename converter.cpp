#include <stdio.h>
#include <iostream>
#include <fstream>

#define CURSE_CHAR 'c'

int converter(const char* path) {
    // Open the input file for reading
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Error: failed to open input file\n";
        return 1;
    }

    // Open the output file for writing
    std::ofstream output_file("output.txt");
    if (!output_file.is_open()) {
        std::cerr << "Error: failed to open output file\n";
        return 1;
    }

    // Read from the input file and write to the output file
    char c;
    while (input_file.get(c)) {
        output_file.put(c);
    }

    // Close the input and output files
    input_file.close();
    output_file.close();

    std::cout << "File copied successfully\n";

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
    // TODO: does not handle unicode right now
    char curse_char = CURSE_CHAR;
    char input_char = *argv[2];
    if(argc == 3 && input_char != '\0' && input_char != ' ' && input_char != '\t') {
        curse_char = input_char;
    }

    converter(argv[1]);
    
    return 0;
}
