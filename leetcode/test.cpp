#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

int count1[26];
int count2[26];
int shift[26];

struct BitMatrix {
	size_t s1,s2,s3;
	uint64_t data[(32*32*32+63)/64];
	void init(size_t a1, size_t a2, size_t a3)
	{
		s1 = a1;
		s2 = a2;
		s3 = a3;
	}
	void set(size_t i1, size_t i2, size_t i3, bool value = true)
	{
		size_t off = (i1 * s2 + i2) * s3 + i3;
		uint64_t mask = 1ull << (off % 64);
		if (value) {
			data[off / 64] |= mask;
		} else {
			data[off / 64] &= ~mask;
		}
	}
	bool test(size_t i1, size_t i2, size_t i3) const
	{
		size_t off = (i1 * s2 + i2) * s3 + i3;
		uint64_t mask = 1ull << (off % 64);
		return data[off / 64] & mask;
	}
};

BitMatrix m;

uint64_t stat(char c)
{
	return 1ull << shift[c - 'a'];
}

struct Window {
	uint64_t pos = 0;
	uint64_t st = 0;
	Window(const std::string &str, size_t size)
	{
		for (size_t i = 0; i < size; i++)
			st += stat(str[i]);
	}
	bool cont(const std::string &str, size_t size) const
	{
		return pos + size <= str.size();
	}
	void next(const std::string &str, size_t size)
	{
		st -= stat(str[pos]);
		if (pos + size < str.size())
			st += stat(str[pos + size]);
		pos++;
	}
};

class Solution {
public:
	bool isScramble(const std::string &s1, const std::string &s2)
	{
		memset(count1, 0, sizeof(count1));
		memset(count2, 0, sizeof(count2));
		memset(shift, 0, sizeof(shift));
		for (char c : s1)
			count1[c - 'a']++;
		for (char c : s2)
			count2[c - 'a']++;
		if (memcmp(count1, count2, sizeof(count1)) != 0)
			return false;
		int sum = 0;
		for (size_t i = 0; i < 26; i++) {
			shift[i] = sum;
			sum += count1[i];
		}

		m.init(s1.size(), s1.size(), s1.size());

		for (size_t i = 0; i < s1.size(); i++) {
			for (size_t j = 0; j < s2.size(); j++) {
				m.set(i, j, 0, s1[i] == s2[j]);
			}
		}

		for (size_t s = 2; s <= s1.size(); s++) {
			for (Window w1(s1, s); w1.cont(s1, s); w1.next(s1, s)) {
				for (Window w2(s2, s); w2.cont(s2, s); w2.next(s2, s)) {
					bool is = false;
					if (w1.st == w2.st) {
						for (size_t k = 1; k < s; k++) {
							size_t l = s - k;
							if (m.test(w1.pos, w2.pos, k - 1) &&
							    m.test(w1.pos + k, w2.pos + k, l - 1)) {
								is = true;
								break;
							}
							if (m.test(w1.pos, w2.pos + l, k - 1) &&
							    m.test(w1.pos + k, w2.pos, l - 1)) {
								is = true;
								break;
							}
						}
					}
					m.set(w1.pos, w2.pos, s - 1, is);
					//std::cout << s1.substr(w1.pos, s) << " " << s2.substr(w2.pos, s) << " " << is << std::endl;
				}
			}
		}

		return m.test(0, 0, s1.size() - 1);

	}
};

void
check(const std::string &s1, const std::string &s2, bool exp)
{
	Solution sol;
	bool res = sol.isScramble(s1, s2);
	if (res != exp) {
		std::cout << s1 << " and " << s2 << " : "
			  << "expected " << exp << " got " << res << std::endl;
		abort();
	}
}


int main()
{
	check("great", "rgeat", true);
	check("abcde", "caebd", false);
	check("a", "a", true);
}
