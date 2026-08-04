#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstddef>

/*
** PmergeMe : Ford-Johnson (merge-insertion) sort
** ------------------------------------------------
** Works on any container whose value_type is comparable and that supports
** begin()/end() and insert() (std::vector, std::deque, std::list...).
**
** Everything is tracked by INDEX into one flat copy of the input (_data),
** never by value, so duplicate values never cause ambiguity.
**
** Pipeline (classic Ford-Johnson):
**   1) Pair elements 2 by 2; if the count is odd, the leftover becomes a
**      "straggler" inserted at the very end.
**   2) Recursively sort the chain made of the BIGGER element of each pair
**      -> this recursive call on a smaller input is the "merge" part.
**   3) Insert every "smaller" (pend) element back into the now-sorted
**      chain by binary search, in the order given by the Jacobsthal
**      sequence (1, then 3,2, then 5,4, then 11,10,...,6, ...), which is
**      what gives Ford-Johnson its minimal-comparisons guarantee.
**   4) Insert the straggler (if any) with one last binary search.
*/

template <typename Container>
class PmergeMe
{
	public:
		typedef typename Container::value_type value_type;

	private:
		std::vector<value_type>	_data;       // flat copy of the input, index = id
		std::vector<size_t>		_sortedIdx;  // result: indices into _data, sorted by value
		size_t						_comparisons;

		struct PairIdx
		{
			size_t big;
			size_t small;
			PairIdx(size_t b, size_t s) : big(b), small(s) {}
		};

		bool less(size_t a, size_t b)
		{
			++_comparisons;
			return (_data[a] < _data[b]);
		}

		// Jacobsthal numbers: J(0)=0, J(1)=1, J(n) = J(n-1) + 2*J(n-2)
		std::vector<size_t> jacobsthalUpTo(size_t limit)
		{
			std::vector<size_t> j;
			j.push_back(0);
			j.push_back(1);
			while (j.back() < limit)
				j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
			return (j);
		}

		// 1-based positions inside the sorted main chain, in the order the
		// matching pend elements (#2..#k) must be inserted
		std::vector<size_t> jacobsthalInsertOrder(size_t k)
		{
			std::vector<size_t> order;
			if (k < 2)
				return (order);
			std::vector<size_t> jac = jacobsthalUpTo(k);
			size_t low = 1;
			for (size_t i = 2; i < jac.size(); ++i)
			{
				size_t high = jac[i];
				if (high > k)
					high = k;
				for (size_t x = high; x > low; --x)
					order.push_back(x);
				low = high;
				if (low >= k)
					break;
			}
			return (order);
		}

		// binary search insertion position of index `idx` into chain[lo..hi)
		size_t binaryInsertPos(const std::vector<size_t> &chain, size_t lo, size_t hi, size_t idx)
		{
			while (lo < hi)
			{
				size_t mid = lo + (hi - lo) / 2;
				if (less(chain[mid], idx))
					lo = mid + 1;
				else
					hi = mid;
			}
			return (lo);
		}

		std::vector<size_t> fordJohnson(std::vector<size_t> chain)
		{
			size_t n = chain.size();
			if (n <= 1)
				return (chain);

			// 1) pair up, bigger element first
			std::vector<PairIdx> pairs;
			bool hasStraggler = false;
			size_t stragglerIdx = 0;
			size_t i = 0;

			for (; i + 1 < n; i += 2)
			{
				size_t a = chain[i];
				size_t b = chain[i + 1];
				if (less(a, b))
					std::swap(a, b);
				pairs.push_back(PairIdx(a, b));
			}
			if (n % 2 == 1)
			{
				stragglerIdx = chain[n - 1];
				hasStraggler = true;
			}

			// 2) recursively sort the chain of "big" elements
			std::vector<size_t> mainChain;
			for (size_t p = 0; p < pairs.size(); ++p)
				mainChain.push_back(pairs[p].big);
			std::vector<size_t> sortedMain = fordJohnson(mainChain);

			// map big index -> its small (pend) partner
			std::vector<size_t> smallOf(_data.size());
			for (size_t p = 0; p < pairs.size(); ++p)
				smallOf[pairs[p].big] = pairs[p].small;

			// 3) start from the sorted main chain, prepend the pend of the
			//    smallest big element directly (it's guaranteed smaller
			//    than the whole chain, no search needed)
			std::vector<size_t> result = sortedMain;
			size_t k = sortedMain.size();

			if (k > 0)
				result.insert(result.begin(), smallOf[sortedMain[0]]);

			std::vector<size_t> order = jacobsthalInsertOrder(k);
			for (size_t oi = 0; oi < order.size(); ++oi)
			{
				size_t pos = order[oi];                // 1-based position in sortedMain
				size_t bigIdx = sortedMain[pos - 1];
				size_t smallIdx = smallOf[bigIdx];

				// bound the search by where bigIdx currently sits: its own
				// pend can never be past it, which is what keeps the
				// number of comparisons minimal
				size_t bound = 0;
				while (bound < result.size() && result[bound] != bigIdx)
					++bound;

				size_t ip = binaryInsertPos(result, 0, bound, smallIdx);
				result.insert(result.begin() + ip, smallIdx);
			}

			if (hasStraggler)
			{
				size_t ip = binaryInsertPos(result, 0, result.size(), stragglerIdx);
				result.insert(result.begin() + ip, stragglerIdx);
			}

			return (result);
		}

	public:
		PmergeMe(const Container &input) : _comparisons(0)
		{
			for (typename Container::const_iterator it = input.begin(); it != input.end(); ++it)
				_data.push_back(*it);

			std::vector<size_t> chain;
			for (size_t idx = 0; idx < _data.size(); ++idx)
				chain.push_back(idx);

			_sortedIdx = fordJohnson(chain);
		}

		Container getSorted() const
		{
			Container out;
			for (size_t i = 0; i < _sortedIdx.size(); ++i)
				out.insert(out.end(), _data[_sortedIdx[i]]);
			return (out);
		}

		size_t comparisons() const { return (_comparisons); }
};

#endif
