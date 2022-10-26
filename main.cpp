#include <optional>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct args
{
    std::ifstream file;
    std::string X;
    uint32_t N{};
};



uint32_t hashString(std::string str);
std::optional<args> getArgs(int argc, char* argv[]);
uint32_t hashModifyString(std::string str, uint32_t hash, char first); //obsolite

int main(int argc, char* argv[])
{

    std::optional<args> arguments = getArgs(argc, argv);
    if (!arguments)
        return EXIT_FAILURE;

    auto argumen = *std::move(arguments);


    //read first size of N chars
    char c = {}, first;
    unsigned int row = 0, buffi = 0;
    std::string substring(argumen.X.size(), '0');

    unsigned int i = 0, absi = 0;
    for (; i < argumen.X.size();i++) {
        argumen.file.get(c);

        if (int(c) < 0 || int(c) > 127) {
            std::cout << int(c) << " " << c << '\n';
            return EXIT_FAILURE;
        }

        substring[i] = c;
    }

    i = 0;
    const uint32_t xHash = hashString(argumen.X);
    uint32_t subHash = hashString(substring);
 
    int posDist = -1, absPosSub = -INT_MAX, rowDist = 0;

    while (argumen.file) {

        //Rain-Karp search algorithm
        if (xHash == subHash && argumen.X == substring) {

            if (absi - absPosSub  <= argumen.N) {

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
        argumen.file.get(c);
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
         first = substring[0];
        
        for (buffi = 0; buffi < substring.size() - 1; buffi++) {
            substring[buffi] = substring[buffi + 1];
        }
        substring[buffi] = c;

        subHash -= first;
        subHash += substring[substring.size() - 1];

    }

  
	return 0;
}


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
    //arguments.X = "Abababaabaabdbaaacbbbcbcacaab\ncdaadadcdcabbdbddadcadddcacaacbaabaabddacbcaddccaccddbdcdcc\ncbbddaddacabadccdccbbacabdccdccdbdbadacccbaacddcddadaacdabbaaddabdcabbcbdadadb\nabddaccdccccdddabbacdabcacdbcdabcbbcbb\nbcabbbcbbadbaaabdcbbaddcdacdadacccdddbaaacdd"; //; TMP hardcoded 

    int a;
    try {
        a = std::stoi(argv[3]);
        if (a <= 0)
            throw(std::runtime_error(argv[3]));
    }
    catch(std::exception& e) {
        std::cout << e.what();
        return {};
    }
    arguments.N = a;

    return arguments;
}


uint32_t hashString(std::string str)
{
    uint32_t hash = 0;
    for (unsigned int i = 0; i < str.size(); i++)
    {
        hash += str[i];
    }

    return hash;
}

//Obsolite
uint32_t hashModifyString(std::string str, uint32_t hash, char first)
{
        hash -= first;
        hash += str[str.size() - 1];

    return hash;
}