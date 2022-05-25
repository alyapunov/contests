#include <iostream>
#include <string>
#include <vector>

class Solution {
public:
	std::string minWindow(const std::string& s, const std::string& t) {
		int req[256] = {};
		int cur[256] = {};

		size_t reqh = 0;
		for (uint8_t c : t)
			reqh += req[c]++ == 0 ? 1 : 0;

		size_t e = 0;
		size_t h = 0;
		for (e = 0; e < s.size(); e++) {
			uint8_t c = s[e];
			cur[c]++;
			if (cur[c] == req[c])
				h++;
			if (h == reqh)
				break;
		}
		if (h != reqh)
			return "";

		size_t b = 0;
		while (true) {
			uint8_t c = s[b];
			if (cur[c] == req[c])
				break;
			cur[c]--;
			++b;
		}

		size_t bestb = b;
		size_t beste = e;

		for (e++; e < s.size(); e++) {
			{
				uint8_t c = s[e];
				cur[c]++;
			}
			while (true) {
				uint8_t c = s[b];
				if (cur[c] == req[c])
					break;
				cur[c]--;
				++b;
			}
			if (e - b < beste - bestb) {
				bestb = b;
				beste = e;
			}
		}

		return s.substr(bestb, beste - bestb + 1);
	}
};

void
check(const std::string& s, const std::string& t, const std::string& res)
{
	std::cout << s << " and " << t << ": ";
	Solution sol;
	auto my = sol.minWindow(s, t);
	if (my != res) {
		std::cout << "expected " << res << " got " << my << std::endl;
		abort();
	}
	std::cout << "OK" << std::endl;
}

int main()
{
	check("ADOBECODEBANC", "ABC", "BANC");
	check("ADOBECODEBANCAB", "ABC", "CAB");
	check("ADOBECODEBANCAB", "AAB", "BANCA");
	check("a", "a", "a");
	check("a", "aa", "");
}
