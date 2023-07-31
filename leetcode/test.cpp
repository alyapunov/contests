#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <optional>
#include <set>
#include <vector>

//using namespace std;

struct Node {
	int v, n, p;
};

Node arr[1000];

struct Pos {
	int p;
	bool operator<(const Pos &a) const {
		assert(p >= 0);
		assert(a.p >= 0);
		return std::tie(arr[p].v, p) < std::tie(arr[a.p].v, a.p);
	}
};

class Solution {
public:
	int maxProfit(int k, const std::vector<int> &pr)
	{
		int num = 0;
		int t = 0;
		size_t s = pr.size();
		{
			size_t i = 0;
			while (i + 1 < s && pr[i + 1] <= pr[i]) i++;
			if (i + 1 == s)
				return 0;

			while (true) {
				int a = pr[i];

				while (i + 1 < s && pr[i + 1] >= pr[i]) i++;
				int b = pr[i];
				arr[num++].v = b - a;
				t += b - a;
				if (i + 1 == s)
					break;

				while (i + 1 < s && pr[i + 1] <= pr[i]) i++;
				int c = pr[i];
				if (i + 1 == s)
					break;
				arr[num++].v = b - c;
			}
		}
		//assert((num & 1) == 1);
		if ((num + 1) / 2 <= k)
			return t;

		for (int i = num - 1; i > 0; i--)
			arr[i].p = i - 1;
		arr[0].p = -1;

		for (int i = 0; i + 1 < num; i++)
			arr[i].n = i + 1;
		arr[num - 1].n = -1;

		std::set<Pos> set;
		for (int i = 0; i < num; i++)
			set.insert(Pos{i});

		num -= num / 2;

		do {
			auto itr = set.begin();
			int i = itr->p;
			int p = arr[i].p;
			int n = arr[i].n;
			set.erase(itr);
			t -= arr[i].v;
			if (p == -1) {
				assert(n >= 0);
				assert(n < 1000);
				set.erase(Pos{n});
				arr[arr[n].n].p = -1;
			} else if (n == -1) {
				set.erase(Pos{p});
				arr[arr[p].p].n = -1;
			} else {
				set.erase(Pos{n});
				set.erase(Pos{p});
				arr[p].v -= arr[i].v;
				arr[p].v += arr[n].v;
				arr[p].n = arr[n].n;
				if (arr[p].n >= 0)
					arr[arr[p].n].p = p;
				set.insert(Pos{p});
			}
		} while (--num > k);

		return t;
	}
};

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::optional<T>& opt)
{
	if (opt.has_value())
		return strm << opt.value();
	else
		return strm << "null";
}

template<class T>
std::ostream& operator<<(std::ostream& strm, const std::vector<T>& vec)
{
	strm << '[';
	bool first = true;
	for (const auto& x : vec) {
		strm << (first ? "" : ", ") << x;
		first = false;
	}
	return strm << ']';
}

void
check(int k, const std::vector<int>& data, int expected)
{
	Solution sol;
	auto got = sol.maxProfit(k, data);
	if (got != expected) {
		std::cout << k << ": " << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check(2, {2}, 0);
	check(2, {2, 1}, 0);
	check(2, {1, 2}, 1);
	check(2, {2,4,2}, 2);
	check(2, {3,2,6,5,0,3}, 7);
	check(2, {3,2,6,5,0,3,1,2}, 7);
	check(2, {1,2,4,2,5,7,2,4,9,0,9}, 17);
	check(2, {6,5,4,8,6,8,7,8,9,4,5}, 7);



}
