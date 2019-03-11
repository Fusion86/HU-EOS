#include <string>
#include <iostream>

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: ./and arg1 arg2" << std::endl;
        return 1;
    }

    std::string arg1(argv[1]);
    std::string arg2(argv[2]);

    if (arg1.size() != arg2.size())
    {
        std::cout << "Argument 1 and 2 need to have the same amount of characters" << std::endl;
    }

    std::string output;

    for (int i = 0; i < arg1.size(); i++)
    {
        output += arg1[i] & arg2[i];
    }

    std::cout << output << std::endl;;

    return 0;
}
