#include <iostream>
#include  <string>

template <typename Container>
class PmergeMe {
public:
    typedef std::pair<int, int> IntPair; // .first = bigger, .second = smaller

    PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    ~PmergeMe();

    void parseInput(int argc, char **argv);   // char** -> std::vector<int>
    void makePairs();                          // group into pairs, sort each internally
    void sortPairs();                           // sort the pair-list by .first
    void printResult() const;

private:
    std::vector<int> _numbers;
    Container        _pairs;
    bool             _hasStraggler;
    int              _straggler;
};