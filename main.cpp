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


int main(int argc, char* argv[])
{
    std::optional<args> arguments = getArgs(argc, argv);
    if (!arguments) 
        return EXIT_FAILURE;
    
    //read first size of N chars
    char c;
    unsigned row = 0;
    std::string substring = "";
    for (uint16_t i=0; i < arguments.value().X.size(); i++) {
        arguments.value().file.get(c);
        substring.push_back(c);
        if(c == '\n')
            row++;
    }

    const uint32_t xHash = hashString(arguments.value().X);
    uint32_t subHash;

    unsigned int i = 0, absi = (unsigned int)arguments.value().X.size();
    int posDist = -1, posSub = -INT_MAX, rowDist = row;

    while (arguments.value().file) {

        //Rain-Karp algorithm
        subHash = hashString(substring);
        if (xHash == subHash && arguments.value().X == substring) {
            if (absi - posSub - arguments.value().X.size() <= arguments.value().N) {

                if (posDist >= 0) {
                    std::cout << rowDist << " " << posDist << "\n";
                    posDist = -1;
                }
                std::cout << row << " " << i << "\n";
            }
            else {
                posDist = i;
                rowDist = row;
            }
            posSub = i;
        }

        // get new char
        arguments.value().file.get(c);
        if (c == '\n') {
            row++;
            i = 0;
        }

        // shift substring by one char
        substring.push_back(c);
        substring.erase(substring.begin());
        i++;
        absi++;
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
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}