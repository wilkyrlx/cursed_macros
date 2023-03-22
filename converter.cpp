using namespace std;

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <list>

#define CURSE_CHAR 'c'

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

string curse_int(int hex_count) {
    return std::string(hex_count, CURSE_CHAR);
}

/*  
    Adds a token to the map (put_token_map() ignores if it is already in the map)
    and then calculates the "cursed" form of the token
*/
string curse_token(string token) {
    put_token_map(token);   // tries to add to map, if does not exist
    int hex_count = token_map[token];

    // FIXME: this overflows with unicode characters
    return curse_int(hex_count);
}

/* Adds spaces to a line to create tokens. Does not split operators - uses REGEX
    Example:
        Input:  for(int i=0;i<=10;i++){return 'test h';}
        Output:  for ( int   i = 0 ; i <= 10 ; i ++){ return   'test h' ;}

    Special cases:
        words are matches: int, this_is_an_int, 10
        anything in quotes matches: "double", "double space", 'single', 'single "double" single', "double 'single' double"
        anything in opening closing angle brackets: <stdio.h>, <map>
*/
string tokenize_line_regex(string line) {
    string test_input = "for(int i=0;i<=10;i++){return \"test h\";}";
    string input = line;

    // equivalent to (?:'|").*(?:'|")|(?:<).*(?:>)|(\w+)
    std::regex pattern("(?:'|\").*(?:'|\")|(?:<).*(?:>)|(\\w+)");  // matches one or more word characters

    string output = std::regex_replace(input, pattern, " $& ");  // add a space before and after each match

    return output;
}

/* Converts a normal line to a cursed line - uses REGEX
    Note: To avoid unexpected behavior, tokenize the input line first with tokenize_line_regex()
    Will still work without tokenizing first, but may bloat the output file
    Example:
        Input:  for ( int   i = 0 ; i <= 10 ; i ++){ return   'test h' ;}
        Output: c cc ccc cccc ccccc cccccc ccc ccccccc cccccccc cccccc ccc ccccccccc cccccccccc ccccccccccc cccccccccccc
*/
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

    string total_line = "";
    for (const auto& match : matches) {
        total_line += curse_token(match) + " ";
    }

    return total_line;
}

// Initialize some commonly used tokens into the map to keep file size lower
void initialize_common_map() {
    put_token_map(";"); // 1
    put_token_map("("); // 2
    put_token_map(")"); // 3
    put_token_map("{"); // 4
    put_token_map("}"); // 5
}

int create_header() {
    std::ofstream outputFile("output/curse.h", std::ios::out | std::ios::binary);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    for(std::map<string,int>::iterator iter = token_map.begin(); iter != token_map.end(); ++iter) {
        string k =  iter->first;
        int v = iter->second;

        string newLine = "#define " + curse_int(v) + " " + k + "\n";
        outputFile << newLine;
    }

    
    outputFile.close();
    std::cout << "File header created successfully\n";
    return 0;
}

int converter(const char* path, wchar_t curse_char) {
    // Open files with UTF-8 encoding
    std::ifstream inputFile(path, std::ios::in | std::ios::binary);
    std::ofstream outputFile("output/output.cpp", std::ios::out | std::ios::binary);

    // Make sure files were opened successfully
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file\n";
        return 1;
    }
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    
    outputFile << "#include \"curse.h\"\n\n";

    string line;
    while (std::getline(inputFile, line)) {
        string check1 = "#define";
        string check2 = "#include";
        if (line.find(check1) == string::npos || line.find(check2) == string::npos) {
            // define or include was found not found, process the line

            // Process each line
            string tokenized_line = tokenize_line_regex(line);
            string cursed_line = curse_line_regex(tokenized_line);
            line = cursed_line;
            line += "\n";

        }
        
        // Write the line to the output file 
        outputFile << line;
    }

    // Close files when done
    inputFile.close();
    outputFile.close();
    std::cout << "File converted successfully\n";
    create_header();
    return 0;
}


int main(int argc, char const *argv[])
{
    std::cout << "Main running...\n";
    if(argc < 2) {
        std::cerr << "Incorrect arguments, usage: <input_path> [empty|c|<custom_path>] [h|n]\n";
        return 1;
    }

    // if first argument is "help", show help menu
    if(argc == 2 && strcmp(argv[1], "help") == 0) {
        std::cout << "Usage: <input_path> [empty|c|<custom_path>] [h|n]\n";
        std::cout << "input_path: path to the file to be converted\n";
        std::cout << "empty|c|<custom_path>: optional argument to specify the character to use for the conversion\n";
        std::cout << "    empty: use an empty character (default)\n";
        std::cout << "    c: use the character 'c'\n";
        std::cout << "    <custom_path>: uses words provided from a .txt file with words separated by spaces\n";
        std::cout << "h|n: optional argument to specify whether to write a header to the output file\n";
        std::cout << "    h: write a header file and a c file (default)\n";
        std::cout << "    n: no header, writes all macros to one file\n";
        return 0;
    }

    // Check for alternate conversion mode
    // 0 = normal, 1 = empty, 2 = c, 3 = custom from file
    int conversion_mode = 0; 
    if(argc > 2) {
        if(strcmp(argv[2], "empty") == 0) {
            conversion_mode = 1;
        } else if(strcmp(argv[2], "c") == 0) {
            conversion_mode = 2;
        } else {
            conversion_mode = 3;
        }
    }

    // Check for write mode
    // 0 = new header (normal), 1 = no header
    int write_mode = 0;
    if(argc > 3) {
        if(strcmp(argv[3], "n") == 0) {
            write_mode = 1;
        }
    }

    curr_count = 1;
    // initialize_common_map();
    converter(argv[1], CURSE_CHAR);
    
    return 0;
}
