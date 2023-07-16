#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

class Solution {
public:
	int findMin(const std::vector<int>& nums) {
		const int *f = nums.data();
		const int *l = f + nums.size() - 1;
		if (f == l)
			return *f;
		if (*f == *l) {
			const int *t1 = f;
			while (true) {
				const int *t2 = t1 + 64;
				if (t2 >= l)
					break;
				if (*t2 != *f) {
					f = t1;
					break;
				}
				t1 = t2;
			}
			while (*f == *l) {
				if (f == --l)
					return *f;
			}
		}
		if (*f < *l)
			return *f;
		while (l - f > 32) {
			const int *m = f + (l - f) / 2;
			if (*m >= *f)
				f = m;
			else
				l = m;
		}
		do {
			++f;
		} while (*f > *l);
		return *f;
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
check(const std::vector<int>& data, int expected)
{
	Solution sol;
	auto got = sol.findMin(data);
	if (got != expected) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check({1}, 1);
	check({1, 1}, 1);
	check({1, 1, 1}, 1);
	check({1, 3, 5}, 1);
	check({1, 2, 2, 2, 0, 1, 1}, 0);
	check({7, 8, 9, 2, 3, 4, 5, 6}, 2);
	check({7, 8, 9, 3, 3, 4, 5, 6}, 3);
	check({1, 1, 1, 2, 2, 2}, 1);
	check({2, 1, 1, 1, 2, 2}, 1);
	check({2, 2, 1, 1, 1, 2}, 1);
	check({2, 2, 2, 1, 1, 1}, 1);
	check({1, 2, 2, 2, 1, 1}, 1);
	check({1, 1, 2, 2, 2, 1}, 1);
}
