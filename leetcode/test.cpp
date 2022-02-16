#include <cassert>
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <climits>
#include <utility>

using namespace std;

constexpr size_t L = 9;

constexpr uint8_t zcount(uint32_t m)
{
	return __builtin_popcount(((1<<L)-1) & ~m) > 7 ? 7 : __builtin_popcount(((1<<L)-1) & ~m);
}

constexpr uint8_t zposi(uint32_t m, uint8_t i)
{
	return i >= zcount(m) ? 0 : i == 0 ? __builtin_ctz(~m) :
	       __builtin_ctz(~(m | ((1u << (zposi(m, i - 1) + 1)) - 1)));
}

constexpr std::array<uint8_t, 7> zpos(uint32_t m)
{
	return {zposi(m, 0), zposi(m, 1), zposi(m, 2), zposi(m, 3), zposi(m, 4), zposi(m, 5), zposi(m, 6)};
}

struct zinfo {
	uint8_t zcount;
	std::array<uint8_t, 7> zpos;
	operator uint8_t() const { return zcount; }
	uint8_t operator[](size_t i) const { return zpos[i]; }

	constexpr zinfo(uint32_t mask) : zcount(::zcount(mask)),
					 zpos(::zpos(mask))
	{}
};

template <size_t ...I>
constexpr std::array<zinfo, 512> get_zinfos(std::index_sequence<I...>)
{
	return {I...};
}

constexpr std::array<zinfo, 512> get_zinfos()
{
	return get_zinfos(std::make_index_sequence<512>{});
}

constexpr std::array<zinfo, 512> zinfos = get_zinfos();

struct Line {
	uint32_t i;
	uint32_t pos;
	uint32_t m;
	uint32_t m1;
	uint32_t m2;
	uint32_t m3;
	void init(Line& prev, uint32_t gm)
	{
		i = UINT32_MAX;
		m = (prev.m1 | prev.m2 | prev.m3 | ~gm) & 511;
	}
	bool done()
	{
		return i >= zinfos[m];
	}
	void setpos(Line& prev)
	{
		pos = zinfos[m][i];
		uint32_t add = 1u << pos;
		m1 = prev.m1 | add;
		m2 = (prev.m2 | add) << 1;
		m3 = (prev.m3 | add) >> 1;
	}
};

class Solution {
public:
	Line board[L + 1];
	int totalNQueens(int n) {
		int total = 0;
		uint32_t gm = (1u << n) - 1;
		for (int i = 0; i < (n + 1) / 2; i++) {
			board[0].i = board[0].pos = i;
			board[0].m1 = 1u << i;
			board[0].m2 = board[0].m1 << 1;
			board[0].m3 = board[0].m1 >> 1;
			board[1].init(board[0], gm);
			int k = 1;
			while (k > 0) {
				if (k == n) {
					total += 1 + (i < n / 2);
					k--;
					continue;
				}
				board[k].i++;
				if (board[k].done()) {
					k--;
					continue;
				}
				board[k].setpos(board[k - 1]);
				k++;
				board[k].init(board[k - 1], gm);
			}
		}
		return total;
	}
};

void
check(int arg, int res)
{
	Solution s;
	auto my = s.totalNQueens(arg);
	if (my != res) {
		std::cout << "expected:\n";
		std::cout << res << "\n";
		std::cout << "got:\n";
		std::cout << my << "\n";
		abort();
	}
}

int main()
{
	check(1, 1);
	check(2, 0);
	check(3, 0);
	check(4, 2);
	//check(5, {});
}
