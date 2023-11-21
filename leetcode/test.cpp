#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

class Solution {
public:
	static constexpr size_t MAX_LEN = 3*100000;
	static constexpr size_t MAX_STACK = MAX_LEN / 2;
	static unsigned long stack[MAX_STACK / sizeof(long) / 8];

	long res;
	long item;
	long sign;
	long stack_sign;
	size_t stack_pos;

	void push_sign()
	{
		size_t bit_off = stack_pos / sizeof(stack[0]);
		size_t bit_pos = stack_pos % sizeof(stack[0]);
		unsigned long bit = (stack_sign >> (sizeof(stack_sign) * 8 - 1)) & 1;
		stack[bit_off] |= (bit << bit_pos);
		stack_pos++;
		stack_sign *= sign;
		sign = 1;

	}

	void pop_sign()
	{
		size_t bit_off = stack_pos / sizeof(stack[0]);
		size_t bit_pos = stack_pos % sizeof(stack[0]);
		stack[bit_off] &= ~(1ull << bit_pos);
		stack_pos--;
		bit_off = stack_pos / sizeof(stack[0]);
		bit_pos = stack_pos % sizeof(stack[0]);
		unsigned long bit = (stack[bit_off] >> bit_pos) & 1;
		stack_sign = (long(bit << 63) >> 63) | 1;
	}

	void next()
	{
		res += item * sign * stack_sign;
		item = 0;
	}

	int calculate(std::string_view s) {
		res = 0;
		item = 0;
		sign = 1;
		stack_sign = 1;
		stack_pos = 0;
		for (char c: s) {
			switch (c) {
			case ' ':
				break;
			case '+':
				next();
				sign = 1;
				break;
			case '-':
				next();
				sign = -1;
				break;
			case '(':
				push_sign();
				break;
			case ')':
				next();
				pop_sign();
				break;
			default:
				item = item * 10 + (c - '0');
			}
		}
		next();
		return res;
	}
};

unsigned long Solution::stack[MAX_STACK / sizeof(long) / 8];

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
check(const std::string& data, int expected)
{
	Solution sol;
	auto got = sol.calculate(data);
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
	check("1 + 1", 2);
	check("-1 + 2", 1);
	check("-1 - 2", -3);
	check("(-1 - 2)", -3);
	check("((1))", 1);
	check("(-(1))", -1);
	check("-(-(1))", 1);
	check("-(-(-1))", -1);
	check("101 + 002",103);
	check("- 1 + 1", 0);
	check(" 2-1 + 20 ", 21);
	check("(1+(4+5+2)-3)+(6+8)", 23);
	check("(1+(4+5+2)-3)-(6+8)", -5);
	check("(1+(4+5+2)-3)-(6-(-1-1))", 1);
}
