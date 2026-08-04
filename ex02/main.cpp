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

	size_t start = 0;
	if (token[0] == '+')
		start = 1;
	if (start == token.size())
		return (false);

	for (size_t i = start; i < token.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[i])))
			return (false);
	}

	errno = 0;
	char *end = 0;
	long value = std::strtol(token.c_str(), &end, 10);

	if (errno == ERANGE || value > INT_MAX)
		return (false);

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
			std::cerr << "Error: invalid input \"" << av[i] << "\"" << std::endl;
			return (false);
		}
		numbers.push_back(value);
	}
	return (true);
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
		std::cerr << "Error: no input provided" << std::endl;
		std::cerr << "Usage: " << av[0] << " <positive integers...>" << std::endl;
		return (1);
	}

	std::vector<int> numbers;
	if (!parseArgs(argc, av, numbers))
		return (1);

	std::cout << "Before: ";
	printContainer(numbers);

	// --- sort with std::vector, timed ---
	std::clock_t vecStart = std::clock();
	PmergeMe<std::vector<int> > pmVec(numbers);
	std::vector<int> sortedVec = pmVec.getSorted();
	std::clock_t vecEnd = std::clock();

	// --- sort with std::deque, timed ---
	std::deque<int> dq(numbers.begin(), numbers.end());
	std::clock_t dqStart = std::clock();
	PmergeMe<std::deque<int> > pmDq(dq);
	std::deque<int> sortedDq = pmDq.getSorted();
	std::clock_t dqEnd = std::clock();

	std::cout << "After:  ";
	printContainer(sortedVec);

	double vecUs = static_cast<double>(vecEnd - vecStart) / CLOCKS_PER_SEC * 1000000.0;
	double dqUs = static_cast<double>(dqEnd - dqStart) / CLOCKS_PER_SEC * 1000000.0;

	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::vector : " << vecUs << " us "
		<< "(" << pmVec.comparisons() << " comparisons)" << std::endl;
	std::cout << "Time to process a range of " << numbers.size()
		<< " elements with std::deque  : " << dqUs << " us "
		<< "(" << pmDq.comparisons() << " comparisons)" << std::endl;

	return (0);
}
