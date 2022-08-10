#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int best[100000];

class Solution {
public:
	int maxProfit(const std::vector<int>& prices) {
		best[0] = 0;
		int m = prices[0];
		int b = 0;
		for (size_t i = 1; i < prices.size(); i++) {
			if (b < prices[i] - m)
				b = prices[i] - m;
			if (m > prices[i])
				m = prices[i];
			best[i] = b;
		}
		m = prices.back();
		b = 0;
		int res = 0;
		for (size_t i = prices.size() - 1; i-- != 0;) {
			if (b < m - prices[i])
				b = m - prices[i];
			if (m < prices[i])
				m = prices[i];
			if (res < b + best[i])
				res = b + best[i];
		}
		return res;
	}
};

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::vector<T>& vec)
{
	strm << '[';
	bool first = true;
	for (const auto& x : vec) {
		strm << (first ? "" : ", ") << x;
		first = false;
	}
	return strm << ']';
}

void
check(const std::vector<int>& data, int expected)
{
	Solution sol;
	auto got = sol.maxProfit(data);
	if (got != expected) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check({3, 3, 5, 0, 0, 3, 1, 4}, 6);
	check({1, 2, 3, 4, 5}, 4);
	check({7, 6, 4, 3, 1}, 0);

	check({1, 2}, 1);
	check({10, 11, 1, 10}, 10);
//	check({}, 0);
}
