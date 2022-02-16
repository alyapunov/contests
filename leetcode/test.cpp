#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

static struct Field {
	uint64_t data[512];
	size_t size;

	void load(const string& s)
	{
		const char *b = s.data();
		const char *e = s.data() + s.size();
		size_t i = 0;
		while (e - b >= 64) {
			uint64_t w = 0;
			for (size_t j = 0; j < 64; j++)
				w |= (b[j] == '(' ? 1ull : 0ull) << j;
			data[i] = w;
			b += 64;
			i++;
		}
		uint64_t w = 0;
		for (size_t j = 0; j < size_t(e - b); j++)
			w |= (b[j] == '(' ? 1ull : 0ull) << j;
		data[i] = w;
		size = s.size();
	}
	bool is_opened(size_t pos)
	{
		return ((data[pos / 64] >> (pos % 64)) & 1);
	}
	size_t after_closed(size_t pos)
	{
		if (pos >= size)
			return 0;
		uint64_t w = data[pos / 64] >> (pos % 64);
		size_t res = (w == 0) ? 64 - pos % 64 : __builtin_ctzll(w);
		if (pos + res > size)
			res = size - pos;
		return res;
	}
	size_t before_opened(size_t pos)
	{
		if (pos == 0)
			return 0;
		--pos;
		uint64_t w = ~(data[pos / 64] << (63 ^ (pos % 64)));
		return w == 0 ? pos / 64 + 1 : __builtin_clzll(w);
	}
} field;

struct Group {
	size_t begin;
	size_t end;
	Group *next;
};

Group groups[16384];

class Solution {
public:
	int longestValidParentheses(const string& s) {
		field.load(s);
		if (field.size == 0)
			return 0;
		{
			Group *last = nullptr;
			Group *curr = groups;
			for (size_t i = 0; i < field.size - 1; i++) {
				if (field.is_opened(i) && !field.is_opened(i + 1)) {
					last = curr++;
					last->next = curr;
					last->begin = i;
					last->end = i + 2;
				}
			}
			if (last == nullptr)
				return 0;
			last->next = nullptr;
		}
		while (true) {
			bool done = false;
			for (Group *grp = groups; grp != nullptr; grp = grp->next) {
				size_t c1 = field.after_closed(grp->end);
				size_t c2 = field.before_opened(grp->begin);
				size_t c3 = std::min(c1, c2);
				if (c3 != 0) {
					grp->begin -= c3;
					grp->end += c3;
					done = true;
				}
				if (grp->next != nullptr &&
				    grp->next->begin == grp->end) {
					grp->end = grp->next->end;
					grp->next = grp->next->next;
					done = true;
				}
			}
			if (!done)
				break;
		}
		int longest = 0;
		for (Group *grp = groups; grp != nullptr; grp = grp->next) {
			int curr = grp->end - grp->begin;
			if (longest < curr)
				longest = curr;
		}
		return longest;
	}
};

void
check(const string& s, int expected)
{
	Solution sol;
	int got = sol.longestValidParentheses(s);
	if (got != expected) {
		std::cout << s << "\n";
		std::cout << "Expected " << expected << ", got " <<  got << "\n";
		abort();
	}
}

int main()
{
//	check("", 0);
//	check("(()", 2);
//	check(")()())", 4);
//	check(")))(())()(())(((", 10);
	check(")))((((((()))))))()((((((()))))))(((", 30);
//	check(")(())(", 4);
//	check(")(()))", 4);
//	check("((())(", 4);
}
