#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cctype>
#include <ctime>

static bool parsePositiveInt(const std::string &token, int &out)
{
	if (token.empty())
		return (false);
	for (size_t i = 0; i < token.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[i])))
			return (false);
	}
	long value = std::strtol(token.c_str(), NULL, 10);
	out = static_cast<int>(value);
	return (true);
}

static bool parseArgs(int argc, char **av, std::vector<int> &numbers)
{
	for (int i = 1; i < argc; ++i)
	{
		int value;
		if (!parsePositiveInt(av[i], value))
		{
			std::cerr << "Error: invalid input \"" << av[i] << "\n";
			return false;
		}
		numbers.push_back(value);
	}
	return true;
}

template <typename Container>
static void printContainer(const Container &c)
{
	for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
	{
		typename Container::const_iterator next = it;
		++next;
		std::cout << *it << (next != c.end() ? " " : "");
	}
	std::cout << std::endl;
}

int main(int argc, char **av)
{
	if (argc < 2)
	{
		std::cerr << "Error: no input provided\n";
		std::cerr << "Usage: " << av[0] << "With positive integers \n";
		return (1);
	}

	std::vector<int> numbers;
	if (!parseArgs(argc, av, numbers))
		return (1);
	std::cout << "Before: ";
	printContainer(numbers);

	std::clock_t time_start_v = std::clock();
	PmergeMe<std::vector<int> > pmVec(numbers);
	std::vector<int> sortedVec = pmVec.getSorted();
	std::clock_t time_end_v = std::clock();

	std::deque<int> dq(numbers.begin(), numbers.end());
	std::clock_t time_start_d = std::clock();
	PmergeMe<std::deque<int> > pmDq(dq);
	std::deque<int> sortedDq = pmDq.getSorted();
	std::clock_t time_end_d = std::clock();

	std::cout << "After:  ";
	printContainer(sortedVec);

	double vect_time = static_cast<double>( time_end_v - time_start_v) / CLOCKS_PER_SEC * 1000000.0;
	double deque_time = static_cast<double>(time_end_d - time_start_d) / CLOCKS_PER_SEC * 1000000.0;

	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::vector : " << vect_time << " us "
		<< "(" << pmVec.comparisons() << " comparisons)" << std::endl;
	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::deque  : " << deque_time << " us "
		<< "(" << pmDq.comparisons() << " comparisons)" << std::endl;

	return (0);
}
