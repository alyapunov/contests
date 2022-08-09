#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

class Solution {
public:
	std::vector<int> method(const std::vector<int>& data)
	{
		return data;
	}
};

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
check(const std::vector<int>& data, const std::vector<int>& expected)
{
	Solution sol;
	auto got = sol.method(data);
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
	check({1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});
}
