#include <cassert>
#include <iostream>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <climits>

using namespace std;

class Solution {
public:
	int trap(const vector<int>& height) {
		int total = 0;
		int h = 0;
		int s = 0;
		for (auto itr = height.begin(); itr != height.end(); ++itr) {
			if (*itr >= h) {
				h = *itr;
				total += s;
				s = 0;
			} else {
				s += h - *itr;
			}
		}
		int hmax = h;
		h = 0;
		s = 0;
		for (auto itr = height.rbegin(); itr != height.rend(); ++itr) {
			if (*itr >= h) {
				h = *itr;
				total += s;
				s = 0;
				if (h == hmax)
					break;
			} else {
				s += h - *itr;
			}
		}
		return total;
	}
};

void
check(const vector<int>& nums, int res)
{
	Solution s;
	int my = s.trap(nums);
	if (my != res) {
		std::cout << "expected " << res << ", got " << my << "\n";
		abort();
	}
}

int main()
{
	check({0,1,0,2,1,0,1,3,2,1,2,1}, 6);
	check({4,2,0,3,2,5}, 9);
}
