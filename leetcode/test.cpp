#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <bitset>

constexpr size_t MAX_LEN = 2000;

template <size_t N = MAX_LEN>
class PaliCheck {
public:
	void init(std::string_view s)
	{
		intptr_t n = s.size();
		for (intptr_t i = 0; i < n; i++) {
			intptr_t j;
			for (j = 0;; j++) {
				intptr_t k1 = i - j;
				intptr_t k2 = i + j;
				if (k1 < 0 || k2 >= n)
					break;
				if (s[k1] != s[k2])
					break;
				set(k1, k2 + 1, true);
			}
			for (;; j++) {
				intptr_t k1 = i - j;
				intptr_t k2 = i + j;
				if (k1 < 0 || k2 >= n)
					break;
				set(k1, k2 + 1, false);
			}
			for (j = 0;; j++) {
				intptr_t k1 = i - j;
				intptr_t k2 = i + j + 1;
				if (k1 < 0 || k2 >= n)
					break;
				if (s[k1] != s[k2])
					break;
				set(k1, k2 + 1, true);
			}
			for (;; j++) {
				intptr_t k1 = i - j;
				intptr_t k2 = i + j + 1;
				if (k1 < 0 || k2 >= n)
					break;
				set(k1, k2 + 1, false);
			}
		}
	}
	bool test(size_t i, size_t j)
	{
		return bits.test(i * (N + 1) + j);
	}
private:
	void set(size_t i, size_t j, bool value = true)
	{
		bits.set(i * (N + 1) + j, value);
	}
	std::bitset<(N + 1) * (N + 1)> bits;
};

PaliCheck p;

int cache[MAX_LEN + 1];

class Solution {
public:
	int minCut(const std::string &s) {
		cache[s.size()] = -1;
		p.init(s);
		for (size_t i = s.size() - 1; i + 1 > 0; i--) {
			int m = MAX_LEN;
			for (size_t j = i + 1; j <= s.size(); j++) {
				if (!p.test(i, j))
					continue;
				int n = 1 + cache[j];
				if (m > n)
					m = n;
			}
			cache[i] = m;
		}
		return cache[0];
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

void
check(const std::string& data, int expected)
{
	Solution sol;
	auto got = sol.minCut(data);
	if (got != expected) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check("a", 0);
	check("ab", 1);
	check("aab", 1);
	check("abcbaabdba", 1);
	check("abbaabdbaabeba", 2);
}
