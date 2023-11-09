#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

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

class Solution {
public:
	static bool are_equal(const char *a1, const char *a2,
			      const char *, const char *b2)
	{
		while (a1 != a2)
			if (*a1++ != *--b2)
				return false;
		return true;
	}

	std::string prepareRes(const std::string& s, size_t palisize)
	{
		std::string res;
		size_t to_append = s.size() - palisize;
		res.reserve(to_append + s.size());
		res.resize(to_append);
		char *dst = res.data();
		const char *dst_end = dst + to_append;
		const char *src = s.data() + s.size();
		while (dst != dst_end)
			*dst++ = *--src;
		res += s;
		return res;
	}

	std::string shortestPalindrome(const std::string& s)
	{
		RollingHash<25001> h1, h2;
		const char *a1 = s.data();
		const char *a2 = s.data() + s.size() / 2;
		const char *b1 = s.data() + (s.size() + 1) / 2;
		const char *b2 = s.data() + s.size();
		for (const char *p = a1; p != a2; ++p)
			h1.push_back(*p);
		for (const char *p = b1; p != b2; ++p)
			h2.push_front(*p);
		if (h1.get() == h2.get() && are_equal(a1, a2, b2, b2))
			return s;
		if (s.size() % 2 != 0) {
			h2.pop_front(*--b2);
			h2.push_back(*--b1);
			if (h1.get() == h2.get() && are_equal(a1, a2, b2, b2))
				return prepareRes(s, s.size() - 1);
		}
		while (a1 != a2) {
			h1.pop_back(*--a2);
			h2.pop_front(*--b2);
			if (h1.get() == h2.get() && are_equal(a1, a2, b2, b2))
				return prepareRes(s, (a2 - a1) * 2 + 1);
			h2.pop_front(*--b2);
			h2.push_back(*--b1);
			if (h1.get() == h2.get() && are_equal(a1, a2, b2, b2))
				return prepareRes(s, (a2 - a1) * 2);
		}
		return prepareRes(s, 0);
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
check(const std::string& data, const std::string& expected)
{
	Solution sol;
	auto got = sol.shortestPalindrome(data);
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
	check("", "");
	check("a", "a");
	check("aacecaaa", "aaacecaaa");
	check("abcd", "dcbabcd");
	check("abbacd", "dcabbacd");
	check("abc", "cbabc");
	std::string test, exp = "b";
	for (size_t i = 0; i < 25000; i++) {
		test += "ab";
		exp += "ab";
	}
	check(test, exp);
}
