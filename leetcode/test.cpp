#include <cassert>
#include <iostream>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <climits>

using namespace std;

constexpr size_t K = 7;
constexpr size_t N = 1 << K;

class Solution {
public:
	uint32_t data[N];
	int firstMissingPositive(const vector<int>& nums, int from, int count) {
		size_t l = (count - 1) / 32 + 1;
		for (size_t i = 0; i < l; i++)
			data[i] = 0;
		for (int i : nums) {
			if (i >= from && i < int(32 * N) + from)
				data[(i - from) / 32] |= 1u << ((i - from) % 32);
		}
		for (size_t i = 0; i < l; i++) {
			if (data[i] != UINT32_MAX)
				return from + i * 32 + __builtin_ctz(~data[i]);
		}
		return -1;
	}
	int firstMissingPositive(const vector<int>& nums) {
		int from = 1;
		if (nums.size() < 32 * N)
			return firstMissingPositive(nums, from, nums.size());
		assert(nums.size() < 32 * N * N);
		size_t l = nums.size() / (32 * N) + 1;
		for (size_t i = 0; i < l; i++)
			data[i] = 0;
		for (int i : nums) {
			if (i >= from && i < int(32 * N * N) + from)
				data[(i - from) / (32 * N)]++;
		}
		for (size_t i = 0; i < l; i++) {
			if (data[i] != 32 * N)
				return firstMissingPositive(nums, from + i * 32 * N, 32 * N);
		}
		return -1;
	}
};

void
check(const vector<int>& nums, int res)
{
	Solution s;
	int my = s.firstMissingPositive(nums);
	if (my != res) {
		std::cout << "expected " << res << ", got " << my << "\n";
		abort();
	}
}

int main()
{
	check({1,2,0}, 3);
	check({3,4,-1,1}, 2);
	check({7,8,9,11,12}, 1);
	std::vector<int> test;
	int n = 500000;
	test.reserve(n);
	for (int i = 1; i <= n; i++)
		test.push_back(i);
	check(test, n + 1);
	test.clear();
	for (int i = 1; i <= n; i++)
		test.push_back(i + 1);
	check(test, 1);
}
