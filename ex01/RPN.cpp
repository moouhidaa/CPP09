#include  "RPN.hpp"

RPN::RPN()
{

}

RPN::RPN(std::stack<double>  Another_D)
{
    distances  = Another_D;
}

RPN::~RPN()
{

}

RPN  &RPN::operator=(RPN  const &obj)
{
    if(this !=  &obj)
        this->distances =  obj.distances;
    return  *this;
}

void  RPN::calculate_distances(std::string input)
{
    for(unsigned int  i = 0;i < input.size();i++)
    {
        char  c = input[i];
        if(c == ' ')
            continue;
        else if(std::isdigit(c))
        {
            double  x =   c - '0';
            distances.push(x);
        }
        else if(c == '+' &&  distances.size() >= 2)
        {
            double  a =  distances.top();
            distances.pop();
            double  b = distances.top();
            distances.pop();
            distances.push(a + b);
        }
        else if(c == '*' &&  distances.size() >= 2)
        {
            double  a =  distances.top();
            distances.pop();
            double  b = distances.top();
            distances.pop();
            distances.push(a * b);
        }
        else if(c == '-' &&  distances.size() >= 2)
        {
            double  a =  distances.top();
            distances.pop();
            double  b = distances.top();
            distances.pop();
            distances.push(b -a);
        }
        else if(c == '/' &&  distances.size() >= 2)
        {
            double  a =  distances.top();
            distances.pop();
            double  b = distances.top();
            distances.pop();
            distances.push(b / a);
        }
        else
            throw Invlaid_input();
    }
    if(distances.size() == 1)
    {
        std::cout  << distances.top() << "\n";
        distances.pop();
    }
    else
        throw  Invlaid_input();
}