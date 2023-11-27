#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <iostream>
#include <set>

template <class MASK_TYPE>
struct SimpleBitSet {
	MASK_TYPE data = 0;
	bool test(uint64_t i) const { return (data >> i) & 1; }
	bool has() const { return data != 0; }
	uint64_t min() const { assert(has()); return __builtin_ctzll(data); }
	uint64_t max() const { assert(has()); return 0x3F ^ __builtin_clzll(data); }
	MASK_TYPE set(uint64_t i) { data |= MASK_TYPE{1} << i; return data; }
	MASK_TYPE clr(uint64_t i) { data &= ~(MASK_TYPE{1} << i); return data; }
	void clear() { data = 0; }
};

template <size_t N, class MASK_TYPE = uint64_t,
	  bool IS_SIMPLE = N <= 8 * sizeof(MASK_TYPE)>
struct BitSet {
	static constexpr size_t BITS = sizeof(MASK_TYPE) * 8;
	static_assert(N > BITS, "IS_SIMPLE == true specialization expected");

	SimpleBitSet<MASK_TYPE> data[(N + BITS - 1) / BITS];
	BitSet<(N + BITS - 1) / BITS, MASK_TYPE> index;

	bool test(uint64_t i) const
	{
		return data[i / BITS].test(i % BITS);
	}

	bool has() const
	{
		return index.has();
	}

	uint64_t min() const
	{
		assert(has());
		uint64_t j = index.min();
		return j * BITS + data[j].min();
	}

	uint64_t max() const
	{
		assert(has());
		uint64_t j = index.max();
		return j * BITS + data[j].max();
	}

	void set(uint64_t i)
	{
		uint64_t j = i / BITS;
		data[j].set(i % BITS);
		index.set(j);
	}

	void clr(uint64_t i)
	{
		uint64_t j = i / BITS;
		if (data[j].clr(i % BITS) == 0)
			index.clr(j);
	}

	void clear()
	{
		while (index.has()) {
			uint64_t i = index.min();
			index.clr(i);
			data[i].clear();
		}
	}
};

template <size_t N, class MASK_TYPE>
struct BitSet<N, MASK_TYPE, true> : public SimpleBitSet<MASK_TYPE> {
	static_assert(N <= 8 * sizeof(MASK_TYPE));
};

template <size_t N, class COUNT_TYPE = uint32_t, class MASK_TYPE = uint64_t>
struct MultiBitSet {
	static constexpr size_t BITS = sizeof(MASK_TYPE) * 8;
	BitSet<N, MASK_TYPE> index;
	COUNT_TYPE counts[((N + BITS - 1) / BITS) * BITS] = {};

	bool test(uint64_t i) const { return index.test(i); }
	bool has() const { return index.has(); }
	uint64_t min() const { return index.min(); }
	uint64_t max() const { return index.max(); }
	void add(uint64_t i) { counts[i]++; index.set(i); }
	void del(uint64_t i) { if (--counts[i] == 0) index.clr(i); }

	void clear()
	{
		while (index.has()) {
			uint64_t i = index.index.min();
			index.index.clr(i);
			index.data[i].clear();
			memset(counts + i * BITS, 0, BITS * sizeof(counts[0]));
		}
	}
};

void check(bool expr, const char *str_expr, const char *file, size_t line)
{
	if (!expr) {
		std::cerr << "FAILED " << str_expr
			  << " at " << file << ":" << line << std::endl;
		abort();
	}
}

#define CHECK(expr) check(expr, #expr, __FILE__, __LINE__)

static void
BitSetTest()
{
	constexpr size_t N = 1000;
	constexpr size_t M = 300000;
	BitSet<N, uint16_t> test;
	std::set<uint64_t> ref;
	for (size_t i = 0; i < M; i++) {
		CHECK(test.has() != ref.empty());
		uint64_t x = rand() % N;
		CHECK(test.test(x) == ref.count(x));
		if (test.test(x)) {
			test.clr(x);
			ref.erase(x);
		} else {
			test.set(x);
			ref.insert(x);
		}
		if (!ref.empty()) {
			CHECK(test.min() == *ref.begin());
			CHECK(test.max() == *ref.rbegin());
		}
		if (rand() % (10 * N) == 0) {
			test.clear();
			ref.clear();
		}
	}
}

static void
MultiBitSetTest()
{
	constexpr size_t N = 100;
	constexpr size_t M = 300000;
	MultiBitSet<N, uint64_t, uint16_t> test;
	std::multiset<uint64_t> ref;
	for (size_t i = 0; i < M; i++) {
		CHECK(test.has() != ref.empty());
		uint64_t x = rand() % N;
		CHECK(test.test(x) == (ref.count(x) != 0));
		if (!test.test(x) || (rand() & 1)) {
			test.add(x);
			ref.insert(x);
		} else {
			test.del(x);
			ref.erase(ref.find(x));
		}
		if (!ref.empty()) {
			CHECK(test.min() == *ref.begin());
			CHECK(test.max() == *ref.rbegin());
		}
		if (rand() % (100 * N) == 0) {
			test.clear();
			ref.clear();
		}
	}
}

int main()
{
	BitSetTest();
	MultiBitSetTest();
}
