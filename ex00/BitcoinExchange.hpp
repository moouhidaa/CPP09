#pragma  once
#include <exception>
#include  <iostream>
#include  <string>
#include  <map>
#include  <sstream>
#include  <fstream>
#include <cctype>
#include <string>
#include  <iomanip>

class  BitcoinExchange
{
    private:
        std::map<std::string, double>  key_value;
        void     Read_csv_file();
        double   valid_value(std::string);
        bool     valid_date(std::string);
        void     exchange_with_price(std::string ,double);
    private:
        BitcoinExchange(BitcoinExchange const  &);
        BitcoinExchange &operator=(BitcoinExchange const &);
    public:
        BitcoinExchange();
        ~BitcoinExchange();
    public:
        std::map<std::string, double> const &get_data() const ;
        void  execute(const char *File);

        class  Useless_file : std::exception
        {
            public:
                virtual char const  *what() const throw()
                {
                    return ("Error: could not open file\n");
                }
        };

        class  Invalid_price : std::exception
        {
            public:
                virtual char const  *what() const throw()
                {
                    return ("Unvalid price Occured\n");
                }
        };

        class  Invalid_column_format : std::exception
        {
            public:
                virtual char const  *what() const throw()
                {
                    return ("The File should be started with date,exchange_rate'\n");
                }
        };

};