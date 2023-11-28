#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

const char *L0[20] = {
	" Zero", " One", " Two", " Three", " Four", " Five", " Six", " Seven", " Eight", " Nine",
	" Ten", " Eleven", " Twelve", " Thirteen", " Fourteen", " Fifteen", " Sixteen", " Seventeen", " Eighteen", " Nineteen",
};

const char *L1[10] = {
	" Nothing", " Ten", " Twenty", " Thirty", " Forty", " Fifty", " Sixty", " Seventy", " Eighty", " Ninety",
};

const char *H[3] = {" Thousand", " Million", " Billion"};

class Solution {
public:
	void append(std::string& res, const char *str)
	{
		if (res.empty())
			res = str + 1;
		else
			res += str;
	}
	void tripletToWord(std::string& res, int num)
	{
		if (num >= 100) {
			int h = num / 100;
			num = num % 100;
			append(res, L0[h]);
			append(res, " Hundred");
		}
		if (num == 0)
			return;
		if (num < 20) {
			append(res, L0[num]);
			return;
		}
		int h = num / 10;
		int l = num % 10;
		append(res, L1[h]);
		if (l != 0)
			append(res, L0[l]);
	}

	std::string numberToWords(int num)
	{
		if (num == 0)
			return L0[0] + 1;

		std::string res;

		int div = 1;
		for (size_t i = 0; i < sizeof(H) / sizeof(H[0]); i++)
			div *= 1000;

		for (size_t i = sizeof(H) / sizeof(H[0]); i-- != 0;) {
			if (num >= div) {
				int h = num / div;
				num = num % div;
				tripletToWord(res, h);
				append(res, H[i]);
			}
			div /= 1000;
		}

		if (num != 0)
			tripletToWord(res, num);

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
check(int data, const std::string& expected)
{
	Solution sol;
	auto got = sol.numberToWords(data);
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
	check(0, "Zero");
	check(123, "One Hundred Twenty Three");
	check(12345, "Twelve Thousand Three Hundred Forty Five");
	check(1234567, "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven");
}
