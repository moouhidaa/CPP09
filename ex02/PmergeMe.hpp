#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstddef>


template <typename Container>
class PmergeMe
{
	public:
		typedef typename Container::value_type value_type;

	private:
		std::vector<value_type>		_data;       
		std::vector<size_t>			_sortedIdx;
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
		std::vector<size_t> jacobsthalUpTo(size_t limit)
		{
			std::vector<size_t> j;
			j.push_back(0);
			j.push_back(1);
			while (j.back() < limit)
				j.push_back(j[j.size() - 1] + 2 * j[j.size() - 2]);
			return (j);
		}

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

		size_t binaryInsertPos(const std::vector<size_t> &chain, size_t lower, size_t high, size_t idx)
		{
			while (lower < high)
			{
				size_t mid = lower + (high - lower) / 2;
				if (less(chain[mid], idx))
					lower = mid + 1;
				else
					high = mid;
			}
			return (lower);
		}

		std::vector<size_t> fordJohnson(std::vector<size_t> chain)
		{
			size_t n = chain.size();
			if (n <= 1)
				return (chain);
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

			std::vector<size_t> mainChain;
			for (size_t p = 0; p < pairs.size(); ++p)
				mainChain.push_back(pairs[p].big);
			std::vector<size_t> sortedMain = fordJohnson(mainChain);

			std::vector<size_t> smallOf(_data.size());
			for (size_t p = 0; p < pairs.size(); ++p)
				smallOf[pairs[p].big] = pairs[p].small;

			std::vector<size_t> result = sortedMain;
			size_t k = sortedMain.size();

			if (k > 0)
				result.insert(result.begin(), smallOf[sortedMain[0]]);

			std::vector<size_t> order = jacobsthalInsertOrder(k);
			for (size_t oi = 0; oi < order.size(); ++oi)
			{
				size_t pos = order[oi];          
				size_t bigIdx = sortedMain[pos - 1];
				size_t smallIdx = smallOf[bigIdx];

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
