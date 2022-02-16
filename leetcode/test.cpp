#include <iostream>
#include <algorithm>
#include <string>
#include <utility>

using namespace std;

template <int n, int ... I>
void split(int k, int* p, std::integer_sequence<int, I...>)
{
	((p[n - (I + 1)] = k % (I + 1), k /= (I + 1)), ...);
}

template <int n>
void split(int k, int* p)
{
	split<n>(k, p, std::make_integer_sequence<int, n>{});
}

//template <int n>
//void split(int k, int* p)
//{
//	p[n - 1] = 0;
//	for (int i = 2; i < n; i++) {
//		p[n - i] = k % i;
//		k /= i;
//	}
//	p[0] = k % n;
//}

typedef void (*split_t)(int, int*);
split_t splits[10] = {split<1>, split<1>, split<2>, split<3>, split<4>, split<5>, split<6>, split<7>, split<8>, split<9>};

class Solution {
public:
	string getPermutation(int n, int k) {
		k--;
		uint64_t src = 0x987654321;
		string res(n, '0');
		int p[9];
		splits[n](k, p);
		for (int i = 0; i < n; i++) {
			int d = p[i] * 4;
			res[i] = '0' + ((src >> d) & 0xf);
			src = (src & ((1ull << d) - 1)) | (src >> (d + 4) << d);
		}
		return res;
	}
};

void
check(int n, int k, const string& res)
{
	Solution sol;
	auto my = sol.getPermutation(n, k);
	if (my != res)
	{
		std::cout << "Expected " << res << " got " << my << "\n";
		abort();
	}
}

int main()
{
	check(3, 1, "123");
	check(3, 2, "132");
	check(3, 3, "213");
	check(3, 4, "231");
	check(3, 5, "312");
	check(3, 6, "321");

	check(4, 1, "1234");
	check(4, 2, "1243");
	check(4, 3, "1324");
	check(4, 4, "1342");
	check(4, 5, "1423");
	check(4, 6, "1432");
	check(4, 7, "2134");
}
