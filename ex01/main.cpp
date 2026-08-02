#include "RPN.hpp"

int  main(int ac,char **av)
{
    if(ac != 2)
    {
        std::cout  <<  "Error : program + input_distances\n";
        return 1;
    }
    try{
        RPN   flay;
        std::string  input(av[1]);
        flay.calculate_distances(input);
    }
    catch(std::exception &e)
    {
        std::cout << e.what();
    }
    return 0;
}