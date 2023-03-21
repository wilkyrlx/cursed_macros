using namespace std;

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <list>

#define CURSE_CHAR L'₻'

// Define global vars
std::map<std::string, int> token_map;   // maps strings to a unique integer
int curr_count;     // in token_map, each string requires a unique integer. This is it


void put_token_map(string token) {
    // only adds to map if not already in map
    if (token_map.find(token) == token_map.end()) {
        token_map[token] = curr_count;
        curr_count++;
    }
}

/*  
    Adds a token to the map (put_token_map() ignores if it is already in the map)
    and then calculates the "cursed" form of the token
*/
string curse_token(string token) {
    put_token_map(token);   // tries to add to map, if does not exist
    int hex_count = token_map[token];

    // FIXME: this overflows with unicode characters
    return std::string(hex_count, 'c');
}

/* Adds spaces to a line to create tokens. Does not split operators
    Example:
        Input:  for(int i=0;i<=10;i++){return 'test h';}
        Output:  for ( int   i = 0 ; i <= 10 ; i ++){ return   'test h' ;}
*/
string tokenize_line_regex(string line) {
    string test_input = "for(int i=0;i<=10;i++){return \"test h\";}";
    string input = line;

    // equivalent to (?:'|").*(?:'|")|(\w+)
    // also consider (?:'|").*(?:'|")|(\S+)
    std::regex pattern("(?:'|\").*(?:'|\")|(\\w+)");  // matches one or more word characters

    string output = std::regex_replace(input, pattern, " $& ");  // add a space before and after each match

    std::cout << "Input:  " << input << std::endl;
    std::cout << "Output: " << output << std::endl;

    return output;
}

// Converts a normal line into a cursed_char line
string curse_line_regex(string line) {
    string test_input = "for(int i=0;i<=10;i++){return \"test h\";}";
    string input = line;

    // equivalent to (?:'|").*(?:'|")|(\S+)
    std::regex pattern("(?:'|\").*(?:'|\")|(\\S+)"); 

    std::sregex_iterator it(input.begin(), input.end(), pattern); // create an iterator for all matches
    std::sregex_iterator end;

    std::list<std::string> matches; // list to store the matches

    while (it != end) {
        std::smatch match = *it; // get the match
        matches.push_back(match.str()); // add the match to the list
        ++it; // advance the iterator to the next match
    }

    string total_line = " ";
    // print the list of matches
    for (const auto& match : matches) {
        total_line += curse_token(match) + " ";
    }
    std::cout << total_line;

    return total_line;
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
        string tokenized_line = tokenize_line_regex(line);
        string cursed_line = curse_line_regex(tokenized_line);
        line = cursed_line;
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
