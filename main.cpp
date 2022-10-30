#include <optional>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <climits>

struct args
{
    std::ifstream file;
    std::string X;
    uint32_t N{};
};

struct linkList {
    char c;
    struct linkList* next; 
};



uint32_t hashString(std::string str);
std::optional<args> getArgs(int argc, char* argv[]);

int main(int argc, char* argv[])
{

    std::optional<args> arguments = getArgs(argc, argv);
    if (!arguments)
        return EXIT_FAILURE;

    auto argumen = *std::move(arguments);


    //read first size of N chars
    char c = {}, first;
    unsigned int row = 0, shifti = 0, buffi = 0, i = 0;
    std::string substring(argumen.X.size(), '0');

    unsigned long absi = 0;
    linkList *akt = new linkList(), * begin ;
    begin = akt;
    for (; i < argumen.X.size();i++) {
        argumen.file.get(c);

        if (int(c) <= 0 || int(c) > 127) {
            std::cout << int(c) << " " << c << '\n'; 
            return EXIT_FAILURE;
        }
        substring[i] = c;
            akt->c = c;
        if (i < argumen.X.size() - 1) {
            akt->next = new linkList();
            akt = akt->next;
        }

    }
    akt->next = begin;


    
    i = 0;
    const uint32_t xHash = hashString(argumen.X); 
    uint32_t subHash = hashString(substring);
 
    int posDist = -1,  rowDist = 0;
    long absPosSub = -INT_MAX;

    char buffer[16384];
    std::streamsize bufferSize = std::size(buffer);
    size_t gc = 0;

    argumen.file.read(buffer, bufferSize);
    gc = argumen.file.gcount();
    
    while (gc > 0) {


        while (buffi < gc) {

            //Rain-Karp search algorithm
            if (xHash == subHash ) {

                akt = begin;
                for (shifti = 0; shifti < substring.size() ; shifti++) {
                    if (akt->c != argumen.X[shifti])
                        break;
                    akt = akt->next;
                }
                if (shifti == argumen.X.size()) {


                    if (absi - absPosSub  <= argumen.N) {

                        if (posDist >= 0) {
                           std::cout << rowDist << " " << posDist << "\n";
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
            
            }

            // get new char
            //argumen.file.get(c);
            c = buffer[buffi];
            if (int(c) <= 0 || int(c) > 127)
                return EXIT_FAILURE;

           

            // shift substring by one char
            first = begin->c;
            begin->c = c;
            begin = begin->next;
        


            //counting
            i++;
            absi++;
            buffi++;
            if (first == '\n') {
                row++;
                i = 0;
            }


            subHash -= first;
            subHash += c;

        }
        
        argumen.file.read(buffer, bufferSize);
        gc = argumen.file.gcount();
        buffi = 0;
        

    }
     
    //Rain-Karp search algorithm for last char
    if (xHash == subHash && argumen.X == substring) {

        if (absi - absPosSub <= argumen.N) {

            if (posDist >= 0) {
                std::cout << rowDist << " " << posDist << "\n";
                posDist = -1;
            }
            absPosSub = absi;
            std::cout << row << " " << i << "\n";
        }
        else {
            absPosSub = absi;
            posDist = i;
            rowDist = row;
        }

    }

    //Freeing memory
    for (shifti = 0; shifti < substring.size(); shifti++) {
        akt = begin;
        begin = begin->next;
        delete akt;

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
    if (arguments.X.size() >= 256 || arguments.X.size() == 0)
        return {};
    //arguments.X = "dccbbdcddddaabaad\nadd"; //; TMP hardcoded 


    uint32_t a;
    try {
        for (char* c = argv[3]; *c; c++)
           if(!std::isdigit(*c))
                throw(std::runtime_error(argv[3]));

        a = std::stoul(argv[3]);
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
