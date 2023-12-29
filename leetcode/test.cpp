#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

using namespace std;

auto init = []()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	return 0;
}();

class Solution {
public:
	std::vector<int> method(const std::vector<int>& data)
	{
		return data;
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

template <class T>
bool
any_order_equal(const T& a, const T& b)
{
	return a == b;
}

template <class T>
bool
any_order_equal(const std::vector<T>& a, const std::vector<T>& b)
{
	if (a.size() != b.size())
		return false;
	for (size_t i = 0; i < a.size(); i++) {
		const T& x = a[i];
		size_t c1 = 0, c2 = 0;
		for (const auto &y : a)
			if (any_order_equal(x, y))
				c1++;
		for (const auto &y : b)
			if (any_order_equal(x, y))
				c2++;
		if (c1 != c2)
			return false;
	}
	return true;
}

void
check(const std::vector<int>& data, const std::vector<int>& expected)
{
	Solution sol;
	auto got = sol.method(data);
	if (got != expected) {
	//if (!any_order_equal(got, expected)) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check({1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});
}
