#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <set>

//using namespace std;

class Solution {
public:
	std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
		auto itr = nums.begin();
		std::multiset<int> s;
		for (; k > 0; k--)
			s.insert(*itr++);
		std::vector<int> res;
		res.push_back(*s.rbegin());
		auto del = nums.begin();
		while (itr != nums.end()) {
			s.erase(s.find(*del++));
			s.insert(*itr++);
			res.push_back(*s.rbegin());
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
check(const std::vector<int>& data, int k, const std::vector<int>& expected)
{
	Solution sol;
	auto got = sol.maxSlidingWindow(data, k);
	if (got != expected) {
	//if (!any_order_equal(got, expected)) {
		std::cout << data << ", " << k
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check({1,3,-1,-3,5,3,6,7}, 3, {3,3,5,5,6,7});
	check({1}, 1, {1});
}
