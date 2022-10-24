#include <optional>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct args
{
    std::ifstream file;
    std::string X;
    uint32_t N;
};

uint32_t hashString(std::string str);
std::optional<args> getArgs(int argc, char* argv[]);
uint32_t hashModifyString(std::string str, uint32_t hash, char first);

int main(int argc, char* argv[])
{
    
    std::optional<args> arguments = getArgs(argc, argv);
    if (!arguments) 
        return EXIT_FAILURE;
    
    //read first size of N chars
    char c;
    unsigned row = 0;
    std::string substring = "";
    unsigned int i = 0, absi = 0;
    for (; i < arguments.value().X.size();i++) {
        arguments.value().file.get(c);

        if (int(c) < 0 || int(c) > 127) {
            std::cout << int(c) << " " << c << '\n';
            return EXIT_FAILURE;
        }

        substring.push_back(c);
        
    }
    i = 0;
    const uint32_t xHash = hashString(arguments.value().X);
    uint32_t subHash = hashString(substring);

 
    int posDist = -1, absPosSub = -INT_MAX, rowDist = 0;

    while (arguments.value().file) {

        //Rain-Karp search algorithm
        if (xHash == subHash && arguments.value().X == substring) {

            if (absi - absPosSub  <= arguments.value().N) {

                if (posDist >= 0) {
                    std::cout << rowDist << " " << posDist  << "\n";
                    posDist = -1;
                }
                absPosSub = absi;
                std::cout << row << " " << i  << "\n";
            }
            else {

                absPosSub = absi;
                posDist = i;
                rowDist = row;


            }
            
        }

        // get new char
        arguments.value().file.get(c);
        if (int(c) < 0 || int(c) > 127)
            return EXIT_FAILURE;

        //counting
        i++;
        absi++;
        if (substring[0] == '\n') {
            row++;
            i = 0;
        }

        // shift substring by one char
        char first = substring[0];
        substring.push_back(c);
        substring.erase(substring.begin());
        subHash = hashModifyString(substring, subHash, first);

    }

  
	return 0;
}

//TODO checking
std::optional<args> getArgs(int argc, char* argv[]) {
    if (argc != 4)
        return {};

    args arguments;

    arguments.file.open(argv[1]);
    if (!arguments.file.is_open())
        return {};

    arguments.X = argv[2];
    if (arguments.X.size() >= 256)
        return {};

    uint32_t a = std::stoi(argv[3]);
    arguments.N = a;

    return arguments;
}


// inspired by algorithm from https://stackoverflow.com/questions/114085/fast-string-hashing-algorithm-with-low-collision-rates-with-32-bit-integer
uint32_t hashString(std::string str)
{
    uint32_t hash = 0;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        hash += str[i];
        //hash += (hash << 4);
        //hash ^= (hash >> 6);
    }

    //hash += (hash << 3);
    //hash ^= (hash >> 11);
    //hash += (hash << 15);

    return hash;
}

uint32_t hashModifyString(std::string str, uint32_t hash, char first)
{
        hash -= first;
        hash += str[str.size() - 1];

    return hash;
}