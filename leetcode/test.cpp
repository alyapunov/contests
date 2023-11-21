#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

using namespace std;

class Solution {
public:
	long countDigitOne(long n) {
		long res = 0;
		long p = 1;
		while (true) {
			long d = p * 10;
			long q = n / d;
			long r = n % d;
			res += q * p;
			if (r >= 2 * p)
				res += p;
			else if (r >= p)
				res += r - p + 1;
			if (q == 0)
				break;
			p *= 10;
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
check(int data, int expected)
{
	Solution sol;
	auto got = sol.countDigitOne(data);
	if (got != expected) {
	//if (!any_order_equal(got, expected)) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

constexpr int trivial(int x)
{
	int res = 0;
	for (int i = 0; i <= x; i++) {
		int j = i;
		while (j) {
			if (j % 10 == 1)
				res ++;
			j /= 10;
		}
	}
	return res;
}

int main()
{
	check(0, 0);
	check(13, 6);
	if (trivial(13) != 6)
		abort();
	for (size_t i = 0; i < 100; i++) {
		int x = rand() % 10000;
		check(x, trivial(x));
	}
	check(999999999, 900000000);
}
