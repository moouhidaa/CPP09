#include  "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{

}
BitcoinExchange::~BitcoinExchange()
{

}

void  BitcoinExchange::Read_csv_file()
{
    std::ifstream   file("data.csv");
    std::string     buffer;
    std::getline(file,buffer);
    if(buffer !=  "date,exchange_rate")
        throw Invalid_column_format();
    while(std::getline(file,buffer))
    {
        std::istringstream  line(buffer);
        std::string  date;
        std::string  data;
        std::getline(line,date,',');
        std::getline(line,data);
        double pr;
        std::istringstream   price(data);
        if(!(price >> pr))
            throw  Invalid_price();
        key_value[date] =  pr;
    }
    file.close();
}

bool   BitcoinExchange::valid_date(std::string date)
{
    if(date.empty() ||  date.size() !=  10 || date[4] != '-' || date[7] != '-')
        return  false;
    for(unsigned int i = 0; i < date.size(); i++)
    {
        if(i == 4 ||  i == 7)
            continue;
        if(!std::isdigit(date[i]))
            return  false;
    }
    if((date[5] == '0' && date[6] == '0' )|| (date[9] == '0' && date[8] == '0'))
        return  false;
    if((date[5] == '1' && date[6] > '2' ))
        return  false;
    if(date[8] > '3' )
        return false;
    return  true;
}


double  BitcoinExchange::valid_value(std::string  value)
{

    std::istringstream  ss(value);
    double              convert;
    if(!(ss >> convert))
    {
        std::cout << "Invalid Number Occured\n";
        return -1;
    }
    else if(convert < 0)
    {
        std::cout  << "Error: not a positive number.\n";
        return -1;
    }
    else if(convert > 1000)
    {
        std::cout  << "Error: too large a number.\n";
        return -1;
    }    
    return  convert;
}

 void     BitcoinExchange::exchange_with_price(std::string  date,double value)
 {
    
     std::map<std::string ,double>::iterator it = key_value.find(date);
    if(key_value.find(date) != key_value.end())
        std::cout  << date << " => " << value << " = " << it->second << " " << value * it->second <<  "\n";
    else
    {
         std::map<std::string ,double>::iterator it2 = key_value.lower_bound(date);
        if(it2 == key_value.end())
            std::cout  << date << " => " << value << " = " << it->second << " " << value * it->second <<  "\n";
        else{
            it--;
            std::cout  << date << " => " << value << " = " << value * it->second <<  "\n";
        }
    } 
 }

 void  BitcoinExchange::execute(const char *File)
 {
    Read_csv_file();
    std::string  buff;
    std::ifstream  file(File);
    if(!file.is_open())
        throw  Useless_file();
    std::getline(file,buff);
    if(buff !=  "date | value")
        throw  Invalid_column_format();
    while(getline(file,buff))
    {
        std::string             date;
        std::string             value;
        double                  vl;
        std::istringstream     line(buff);
        std::getline(line ,date,'|');
        std::getline(line,value);

        if(!date.empty())
            date =  date.erase(date.size() - 1);
        if(!valid_date(date))
        {
            std::cout << "Error: bad input =>" << date << "\n";
            continue;
        }
        vl = valid_value(value);
        if(vl != -1)
            exchange_with_price(date,vl);
    }

 }