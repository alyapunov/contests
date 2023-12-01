#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

using namespace std;

enum op_t {
	JUNCT,
	MULT,
	PLUS,
	MINUS,
};

const char *op_str[] = {"", "*", "+", "-"};

struct Applier {
	long sum = 0;
	long mul = 1;
	long val = 0;
	bool allowed = true;

	void init(int dig)
	{
		val = dig;
	}

	void add(int op, int dig)
	{
		switch (op) {
		case JUNCT:
			allowed = allowed && (val != 0);
			val = val * 10 + dig;
			break;
		case MULT:
			mul *= val;
			val = dig;
			break;
		case PLUS:
			sum += mul * val;
			mul = 1;
			val = dig;
			break;
		case MINUS:
			sum += mul * val;
			mul = -1;
			val = dig;
			break;
		default:
			__builtin_unreachable();
		};
	}

	long get() const
	{
		return sum + mul * val;
	}
};

class Solution {
public:
	std::vector<std::string> addOperators(const std::string& num, int trg) {
		int digits[10];
		size_t n = num.size();
		for (size_t i = 0; i < n; i++)
			digits[i] = num[i] - '0';

		int ops[10] = {-1};

		Applier app[10];
		app[0].init(digits[0]);
		for (size_t i = 1; i < n; i++) {
			app[i] = app[i - 1];
			app[i].add(ops[i], digits[i]);
		}

		std::vector<std::string> res;
		int prev = 0;
		for (int var = 0; var < (1 << ((n - 1) * 2)); var++) {
			int k = n;
			int v = var;
			while (v != prev) {
				k--;
				ops[k] = v % 4;
				v /= 4;
				prev /= 4;
			}
			prev = var;

			for (size_t i = k; i < n; i++) {
				app[i] = app[i - 1];
				app[i].add(ops[i], digits[i]);
			}

			if (app[n - 1].allowed && app[n - 1].get() == trg) {
				std::string str;
				str += char('0' + digits[0]);
				for (size_t i = 1; i < n; i++) {
					str += op_str[ops[i]];
					str += char('0' + digits[i]);
				}
				res.emplace_back(std::move(str));
			}
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
check(const std::string& num, int target, const std::vector<std::string>& expected)
{
	Solution sol;
	auto got = sol.addOperators(num, target);
	//if (got != expected) {
	if (!any_order_equal(got, expected)) {
		std::cout << num << " " << target
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
//	constexpr int N = 3;
//	int arr[N + 1] = {1, 2, 3, 4};
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			for (int k = 0; k < 4; k++) {
//				int ops[N] = {i, j, k};
//				Applier a;
//				a.init(arr[0]);
//				for (int ai = 0; ai < N; ai++)
//					a.add(ops[ai], arr[ai + 1]);
//
//				std::cout << char('0' + arr[0]);
//				for (int ai = 0; ai < N; ai++)
//					std::cout << op_str[ops[ai]] << char('0' + arr[ai + 1]);
//				std::cout << " = " << a.get() << std::endl;
//			}
//		}
//	}
//	return 0;

	check("1", 1, {"1"});
	check("123", 6, {"1*2*3","1+2+3"});
	check("105", 5, {"1*0+5","10-5"});
	check("232", 8, {"2*3+2","2+3*2"});
	check("1333", 28, {"1+3*3*3"});
	check("1333", -26, {"1-3*3*3"});
	check("951", 5, {"9-5+1"});
	check("9511", 5, {"9-5+1*1", "9-5*1+1"});
	check("3456237490", 9191, {});
	check("00", 0, {"0*0","0+0","0-0"});
}
