#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <cmath>

//using namespace std;

using number_t = uint32_t;
using count_t = uint16_t;
constexpr size_t MAX_BASE = 16;

template <number_t N, bool IS_LAST = (N <= MAX_BASE)>
struct Cont {
	static constexpr size_t ct_pow(size_t pow, size_t base)
	{
		return pow > 0 ? base * ct_pow(pow - 1, base) : 1;
	}

	static constexpr size_t ct_log(size_t a, size_t base)
	{
		return a >= base ? 1 + ct_log(a / base, base) : 0;
	}

	static constexpr number_t NEXT = ct_pow(ct_log(N - 1, MAX_BASE), MAX_BASE);
	static constexpr size_t SIZE = (N - 1) / NEXT + 1;

	count_t counts[SIZE] = {};
	Cont<NEXT> next[SIZE];

	void reset()
	{
		for (size_t i = 0; i < SIZE; i++) {
			if (counts[i] > 0) {
				next[i].reset();
				counts[i] = 0;
			}
		}
	}
	void add(number_t num)
	{
		counts[num / NEXT]++;
		next[num / NEXT].add(num % NEXT);
	}
	number_t get(count_t pos)
	{
		number_t i = 0;
		while (pos >= counts[i])
			pos -= counts[i++];
		return i * NEXT + next[i].get(pos);
	}
	std::pair<number_t, number_t> get2(count_t pos)
	{
		number_t i = 0;
		while (pos >= counts[i])
			pos -= counts[i++];
		if (pos + 1 < counts[i]) {
			std::pair<number_t, number_t> tmp = next[i].get2(pos);
			return {i * NEXT + tmp.first, i * NEXT + tmp.second};
		}
		number_t first = i * NEXT + next[i].get(pos);
		pos++;
		do {
			pos -= counts[i++];
		} while (pos >= counts[i]);
		number_t second = i * NEXT + next[i].get(pos);
		return {first, second};
	}
};

template <number_t N>
struct Cont<N, true> {
	static constexpr size_t SIZE = N;
	count_t counts[SIZE];

	void reset()
	{
		for (size_t i = 0; i < SIZE; i++)
			counts[i] = 0;
	}
	void add(number_t num)
	{
		counts[num]++;
	}
	number_t get(count_t pos)
	{
		number_t i = 0;
		while (pos >= counts[i])
			pos -= counts[i++];
		return i;
	}
	std::pair<number_t, number_t> get2(count_t pos)
	{
		number_t i = 0;
		while (pos >= counts[i])
			pos -= counts[i++];
		number_t j = i;
		pos++;
		while (pos >= counts[j])
			pos -= counts[j++];
		return {i, j};
	}
};

Cont<200001> cont;

class MedianFinder {
public:
	uint16_t total = 0;

	MedianFinder()
	{
		cont.reset();
	}


	void addNum(int num)
	{
		cont.add(num + 100000);
		total++;
	}

	double findMedian()
	{
		if (total % 2 == 1)
			return double(cont.get(total / 2)) - 100000;
		auto [a, b] = cont.get2(total / 2 - 1);
		return (double(a) + double(b)) / 2 - 100000;
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
	for (size_t k = 0; k < 100; k ++)
	{
		MedianFinder perf;
		double res = 0;
		for (size_t i = 0; i < 50000; i++) {
			int x = rand() % 200000 - 100000;
			perf.addNum(x);
			res += perf.findMedian();
		}
		std::cout << res << std::endl;
	}
	for (size_t k = 0; k < 100; k ++)
	{
		MedianFinder perf;
		double res = 0;
		for (size_t i = 0; i < 50000; i++) {
			int x = i % 200000 - 100000;
			perf.addNum(x);
			res += perf.findMedian();
		}
		std::cout << res << std::endl;
	}

}
