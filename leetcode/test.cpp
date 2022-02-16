#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class Solution {
public:
	string getPermutation(int n, int k) {
		k--;
		string res = "123456789";
		string src = res;
		res.resize(n);
		int p[9];
		for (int i = 0; i < n; i++) {
			p[i] = k % (i + 1);
			k /= (i + 1);
		}
		for (int i = 0; i < n; i++) {
			int d = p[n - 1 - i];
			res[i] = src[d];
			src.erase(d, 1);
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
