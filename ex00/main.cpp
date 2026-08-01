#include "BitcoinExchange.hpp"

int main(int  ac,char  **av)
{
    std::cout << std::fixed << std::setprecision(1);
    if(ac != 2)
    {
        std::cout  << "Expected av0[./btc] av1[file_input.txt]\n";
        return 1;
    }
    try  
    {
        BitcoinExchange   my_money;
        my_money.execute(av[1]);
    }

    catch(std::exception  &e)
    {
        std::cout  <<  e.what() <<  std::endl;
        return 1;
    }
    return  0;
}