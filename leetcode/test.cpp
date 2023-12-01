#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <cmath>

//using namespace std;

class MedianFinder {
public:
	std::vector<int> a, b;

	MedianFinder()
	{

	}

	void push_a(int num)
	{
		a.push_back(num);
		std::push_heap(a.begin(), a.end());
	}

	int pop_a()
	{
		std::pop_heap(a.begin(), a.end());
		int res = a.back();
		a.pop_back();
		return res;
	}

	void push_b(int num)
	{
		b.push_back(num);
		std::push_heap(b.begin(), b.end(), std::greater<int>{});
	}

	int pop_b()
	{
		std::pop_heap(b.begin(), b.end(), std::greater<int>{});
		int res = b.back();
		b.pop_back();
		return res;
	}

	void addNum(int num)
	{
		if (a.empty() && b.empty())
			return a.push_back(num);

		if (b.empty()) {
			if (num < a[0])
				std::swap(num, a[0]);
			return b.push_back(num);
		}

		if (num < a[0]) {
			if (a.size() > b.size())
				push_b(pop_a());
			push_a(num);
		} else if (num > b[0]) {
			if (b.size() >= a.size())
				push_a(pop_b());
			push_b(num);
		} else {
			if (a.size() > b.size())
				push_b(num);
			else
				push_a(num);
		}
	}

	double findMedian()
	{
		if (a.size() > b.size())
			return a[0];
		else
			return (double(a[0]) + double(b[0])) / 2;
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

MedianFinder finder;

void
check(double expected)
{
	auto got = finder.findMedian();
	if (fabs(got - expected) > 1e-5) {
	//if (!any_order_equal(got, expected)) {
		std::cout << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	finder.addNum(1);
	finder.addNum(2);
	check(1.5);
	finder.addNum(3);
	check(2);
}
