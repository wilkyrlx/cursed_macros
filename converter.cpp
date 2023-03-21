using namespace std;

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>

#define CURSE_CHAR L'₻'

// Define global vars
std::map<std::string, int> token_map;   // maps strings to a unique integer
int curr_count;     // in token_map, each string requires a unique integer. This is it


/*  Splits a line into tokens and adds them to the map if they do not exist in it yet
    For example:
        for(int i=0;i<=10;i++){return null;} -> for ( int i = 0 ; i <= 10 ; i ++ ) { return null ; }
        std::string test_variable="test"; -> std :: string test_variable = "test" ;
*/
string map_line(string input_line) {
    // TODO: finish this
    /* Pseudocode:
        1. use regex to get words; \w+
        2. add a space before and after each word
        3. get all groups of chars separated by spaces \S+
        4a (simple). add each group to map and then convert
        4b (efficient). check all groups of non-words, split into parts (maybe \W+)
        5b. add each group to map, then convert
   */
    return input_line;
}

int regex_f() {
    std::string input = "for(int i=0;i<=10;i++){return \"test h\";}";
    
    // equivalent to (\w+)|"(.?)"|'(.*?)'
    std::regex pattern("(\\w+)|\"(.*?)\"|'(.*?)'");  // matches one or more word characters

    std::string output = std::regex_replace(input, pattern, " $& ");  // add a space before and after each match

    std::cout << "Input:  " << input << std::endl;
    std::cout << "Output: " << output << std::endl;

    return 0;
}

void put_token_map(string token) {
    // only adds to map if not already in map
    if (token_map.find(token) == token_map.end()) {
        token_map[token] = curr_count;
        curr_count++;
    }
}


// Initialize some basic tokens into the map
int initialize_map() {
    curr_count = 1;
    put_token_map(";"); // 1
    put_token_map("("); // 2
    put_token_map(")"); // 3
    put_token_map("{"); // 4
    put_token_map("}"); // 5
}

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

    string line;
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
