#include <cassert>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <string_view>

#include <iostream>

template <size_t MAX_LENGTH, class OFF_T>
struct KMP {
	OFF_T prefix[MAX_LENGTH];
	const char *str;
	OFF_T size;
	OFF_T pos;
	void init(const char *str_, OFF_T size_)
	{
		str = str_;
		size = size_;
		prefix[0] = 0;
		for (OFF_T i = 1, j = 0; i < size; i++) {
			while (true) {
				if (str[i] == str[j]) {
					prefix[i] = ++j;
					break;
				} else if (j == 0) {
					prefix[i] = 0;
					break;
				} else {
					j = prefix[j - 1];
				}
			}
		}
		pos = 0;
	}
	bool next(char c)
	{
		while (true) {
			if (c == str[pos]) {
				pos++;
				if (pos == size) {
					pos = prefix[pos - 1];
					return true;
				} else {
					return false;
				}
			} else if (pos == 0) {
				return false;
			} else {
				pos = prefix[pos - 1];
			}
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

int main()
{
	KMP<100, int> test;
	test.init("abacabab", 8);
	CHECK(test.prefix[0] == 0);
	CHECK(test.prefix[1] == 0);
	CHECK(test.prefix[2] == 1);
	CHECK(test.prefix[3] == 0);
	CHECK(test.prefix[4] == 1);
	CHECK(test.prefix[5] == 2);
	CHECK(test.prefix[6] == 3);
	CHECK(test.prefix[7] == 2);

	test.init("aaab", 4);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == true);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == true);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == true);

	test.init("abab", 4);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == false);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == true);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == true);
	CHECK(test.next('a') == false);
	CHECK(test.next('b') == true);
}
