#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>

#include <iostream>

template <size_t MAX_LENGTH>
struct RollingHash {
	static constexpr uint64_t pow(uint64_t a, uint64_t p)
	{
		uint64_t res = 1;
		uint64_t x = a;
		for (int i = 0;; i++) {
			if (p & (1ull << i))
				res = res * x % MOD;
			if ((p >> i) <= 1)
				break;
			x = x * x % MOD;
		}
		return res;
	}

	static constexpr uint64_t build_q()
	{
		return pow(R, MOD - 2);
	}

	template <size_t...I>
	static constexpr std::array<uint64_t, MAX_LENGTH> build_pow(std::index_sequence<I...>)
	{
//		return {pow(R, I)...};
		uint64_t x = 1;
		return {(I == 0 ? 1 : x = x * R % MOD)...};
	}

	static constexpr std::array<uint64_t, MAX_LENGTH> build_pow()
	{
		return build_pow(std::make_index_sequence<MAX_LENGTH>{});
	}

	static constexpr uint64_t MOD = 0x7FFFFFFF;
	static constexpr uint64_t R = 1009;
	static constexpr uint64_t Q = build_q();
	static_assert(R * Q % MOD == 1);

	static constexpr std::array<uint64_t, MAX_LENGTH> POW = build_pow();
	static_assert(POW[MAX_LENGTH - 1] == pow(R, MAX_LENGTH - 1));

	uint64_t h = 0;
	uint64_t l = 0;

	uint64_t get() const
	{
		return h;
	}

	uint64_t push_back(char c)
	{
		unsigned char u = c;
		l++;
		return h = (h * R + u) % MOD;
	}

	uint64_t pop_back(char c)
	{
		unsigned char u = c;
		l--;
		return h = (h + MOD - u) * Q % MOD;
	}

	uint64_t push_front(char c)
	{
		unsigned char u = c;
		l++;
		assert(l < MAX_LENGTH);
		return h = (h + u * POW[l - 1]) % MOD;
	}

	uint64_t pop_front(char c)
	{
		unsigned char u = c;
		l--;
		return h = (h + u * (MOD - POW[l])) % MOD;
	}

	uint64_t shift(char add, char drop)
	{
		pop_front(drop);
		return push_back(add);
	}

	uint64_t shift_rev(char add, char drop)
	{
		pop_back(drop);
		return push_front(add);
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

int main()
{
	RollingHash<100000> rh1, rh2;
	for (size_t i = 0; i < 10; i++)
		std::cout << i << " " << rh1.POW[i] << "\n";

	CHECK(rh1.get() == 0);
	CHECK(rh1.push_back('a') == rh2.push_back('a'));
	CHECK(rh2.pop_back('a') == 0);
	CHECK(rh1.get() == rh2.push_back('a'));
	CHECK(rh2.pop_back('a') == 0);
	CHECK(rh1.get() != rh2.push_back('b'));
	CHECK(rh1.push_back('b') == rh2.push_front('a'));
	CHECK(rh1.push_back('c') != rh2.get());
	CHECK(rh1.push_back('a') != rh2.get());
	CHECK(rh1.push_back('b') != rh2.get());
	CHECK(rh1.pop_front('a') != rh2.get());
	CHECK(rh1.pop_front('b') != rh2.get());
	CHECK(rh1.pop_front('c') == rh2.get());

	CHECK(rh1.get() != rh2.push_back('b'));
	CHECK(rh1.get() == rh2.pop_back('b'));
	CHECK(rh1.get() != rh2.push_back('c'));
	CHECK(rh1.get() == rh2.pop_back('c'));
	CHECK(rh1.get() != rh2.pop_back('b'));
	CHECK(rh1.get() == rh2.push_back('b'));

	CHECK(rh1.get() != rh2.push_front('a'));
	CHECK(rh1.get() == rh2.pop_front('a'));
	CHECK(rh1.get() != rh2.push_front('c'));
	CHECK(rh1.get() == rh2.pop_front('c'));
	CHECK(rh1.get() != rh2.pop_front('a'));
	CHECK(rh1.get() == rh2.push_front('a'));
}
