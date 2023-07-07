#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <numeric>

//using namespace std;

int tmp[20000];

class Solution {
public:
	int candy(const std::vector<int> &ratings)
	{
		size_t s = ratings.size();
		tmp[0] = 1;
		for (size_t i = 0; ++i != s;) {
			if (ratings[i] > ratings[i - 1])
				tmp[i] = tmp[i - 1] + 1;
			else
				tmp[i] = 1;
		}
		int res = tmp[s - 1];
		for (size_t i = s - 1; i-- != 0;) {
			if (ratings[i] > ratings[i + 1]) {
				int x = tmp[i + 1] + 1;
				if (tmp[i] < x)
					tmp[i] = x;
			}
			res += tmp[i];
		}
		return res;

	}
};

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::optional<T>& opt)
{
	if (opt.has_value())
		return strm << opt.value();
	else
		return strm << "null";
}

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
	auto got = sol.candy(data);
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
	check({1, 0, 2}, 5);
	check({1, 2, 2}, 4);
	check({1, 2, 3, 2, 1}, 9);
	check({1, 2, 3, 1, 1}, 8);
	check({1, 1, 3, 2, 1}, 8);
	check({3, 2, 1, 2, 3}, 11);
	check({2, 3, 2, 1}, 7);
}
