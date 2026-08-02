#pragma once
#include  <stack>
#include <string>
#include  <iostream>

class RPN
{
    private:
        std::stack<double>   distances;
    public:
        RPN();
        RPN(std::stack<double>);
        RPN(RPN const &);
        RPN  &operator=(RPN  const &obj);
        ~RPN();

       void calculate_distances(std::string);
    class Invlaid_input : public std::exception
    {
        virtual const char *what() const throw()
        {
            return("Error\n");
        }
    };
};