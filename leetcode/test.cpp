#include <iostream>
#include <vector>

struct S {
	int i;
	int h;
};

S s[100 * 1024];

class Solution {
public:
	int largestRectangleArea(const std::vector<int>& heights) {
		int n = 0;
		int m = 0;
		s[0].i = 0;
		s[0].h = 0;
		int hs = heights.size();
		for (int i = 0; i < hs; i++) {
			if (heights[i] > s[n].h) {
				n++;
				s[n].i = i;
				s[n].h = heights[i];
			} else if (heights[i] == s[n].h) {
			} else {
				while (heights[i] < s[n].h) {
					if (s[n].h * (i - s[n].i) > m)
						m = s[n].h * (i - s[n].i);
					n--;
				}
				n++;
				s[n].h = heights[i];
			}
		}
		while (0 < s[n].h) {
			if (s[n].h * (hs - s[n].i) > m)
				m = s[n].h * (hs - s[n].i);
			n--;
		}
		return m;
	}
};

template <class T>
std::ostream& operator<<(std::ostream& strm, const std::vector<T>& vec)
{
	int i = 0;
	strm << '[';
	for (const T& v : vec)
		strm << (i++ ? ", " : "") << v;
	return strm << ']';
}

void
check(const std::vector<int>& heights, int res)
{
	std::cout << heights << ": ";
	Solution sol;
	auto my = sol.largestRectangleArea(heights);
	if (my != res) {
		std::cout << "expected " << res << " got " << my << std::endl;
		abort();
	}
	std::cout << "OK" << std::endl;
}

int main()
{
	check({2,1,5,6,2,3}, 10);
	check({2,4}, 4);
	check({2,1,2}, 3);
	check({2,0,2,4}, 4);
	check({1,2,3,50,3,2,1}, 50);
	check({1,1,1,1,1,4,1,1,1,1,1}, 11);
	check({1,1,1,1,1,7,1,1,1,1,1}, 11);
	check({2,0,4,2}, 4);
	check({0}, 0);
	check({}, 0);
}
