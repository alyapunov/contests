#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

auto init = []()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	return 0;
}();

struct LetterSum {
	uint16_t count[26] = {};
	int total = 0;
	void clear()
	{
		*this = LetterSum{};
	}
	void add(char c)
	{
		if (count[c - 'a']++ == 0)
			total++;
	}
	void del(char c)
	{
		if (--count[c - 'a'] == 0)
			total--;
	}
};

int buf[10001];

class Solution {
public:
	int more(const std::string& s, int k, int pos, LetterSum& prev)
	{
		int l = s.size(), b = pos, e = pos;
		if (b == l)
			return 0;
		int can = 0;
		for (size_t i = 0; i < 26; i++)
			if (prev.count[i] == 0)
				can |= 1 << i;
		LetterSum sum;
		int dontneed = 0;
		while (true) {
			dontneed |= 1 << (s[e] - 'a');
			sum.add(s[e++]);
			if (sum.total >= k) {
				if (can &~ dontneed)
					return 1 + buf[e - 1];
				return 0;
			}
			if (e == l)
				return 1;
		}
	}
	int maxPartitionsAfterOperations(const std::string& s, int k)
	{
		if (k >= 26)
			return 1;

		LetterSum sum;
		int l, b, e;
		l = b = e = s.size();
		buf[e] = 0;
		while (b != 0) {
			sum.add(s[--b]);
			while (sum.total > k)
				sum.del(s[--e]);
			buf[b] = 1 + buf[e];
		}

		b = e = 0;
		int m = buf[0];
		int p = 0;
		do {
			sum.clear();
			do {
				sum.add(s[e++]);
				if (sum.total > k) {
					sum.del(s[--e]);
					break;
				}
				if (sum.total >= k && e - b > k ) {
					m = std::max(m, p + 1 + buf[e - 1]);
					if (sum.count[s[e - 1] - 'a'] > 1)
						m = std::max(m, p + 1 +
								more(s, k, e,
								     sum));
				}
			} while (e < l);
			while (e < l) {
				sum.add(s[e++]);
				if (sum.total > k) {
					sum.del(s[--e]);
					break;
				}
			}
			b = e;
			p++;
		} while (b != l);
		return m;
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
check(const std::string& data, int k, int expected)
{
	Solution sol;
	auto got = sol.maxPartitionsAfterOperations(data, k);
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
	check("accca", 2, 3);
	check("aabaab", 3, 1);
	check("xxyz", 1, 4);
	check("baacccb", 1, 6);
	check("ccbaabbba", 2, 4);
}
