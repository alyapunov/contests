#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <map>
#include <cstring>

//using namespace std;

template <size_t N>
struct Set {
	uint64_t data[(N + 63) / 64] = {};
	Set<(N + 63) / 64> index;

	void set(uint64_t i)
	{
		uint64_t j = i / 64;
		uint64_t k = i % 64;
		data[j] |= 1ull << k;
		index.set(j);
	}

	void clr(uint64_t i)
	{
		uint64_t j = i / 64;
		uint64_t k = i % 64;
		data[j] &= ~(1ull << k);
		if (data[j] == 0)
			index.clr(j);
	}

	uint64_t find() const
	{
		uint64_t j = index.find();
		int k = 0x3f ^ __builtin_clzll(data[j]);
		return j * 64 + k;
	}

	bool has() const
	{
		return index.has();
	}
};

template <>
struct Set<1> {
	bool bit = false;
	void set(uint64_t) { bit = true; }
	void clr(uint64_t) { bit = false; }
	uint64_t find() const { return 0; }
	bool has() const { return bit; }
};


template <size_t N>
struct SuperSet {
	Set<N> index;
	unsigned short counts[N] = {};

	void add(uint64_t i)
	{
		counts[i]++;
		index.set(i);
	}

	void del(uint64_t i)
	{
		if (--counts[i] == 0)
			index.clr(i);
	}

	void reset()
	{
		while (index.has()) {
			uint64_t i = index.index.find();
			index.index.clr(i);
			index.data[i] = 0;
			memset(counts + i * 64, 0, 64 * sizeof(counts[0]));
		}
	}

	uint64_t find() const
	{
		return index.find();
	}

};

SuperSet<20032> superset;

class Solution {
public:
	std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
		superset.reset();

		std::vector<int> res;
		res.reserve(nums.size() + 1 - k);

		auto itr = nums.begin();
		for (; k > 0; k--)
			superset.add(10000 + *itr++);

		res.push_back(superset.find() - 10000);
		auto del = nums.begin();
		while (itr != nums.end()) {
			superset.add(10000 + *itr++);
			superset.del(10000 + *del++);
			res.push_back(superset.find() - 10000);
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
	check({1, -1}, 1, {1, -1});
}
