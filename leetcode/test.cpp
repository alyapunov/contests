#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

int buf[200];

class Solution {
public:
	int calculateMinimumHP(const std::vector<std::vector<int>>& data)
	{
		size_t m = data.size();
		size_t n = data[0].size();
		{
			auto &row = data[m - 1];
			buf[n - 1] = row[n - 1] < 0 ? 1 - row[n - 1] : 1;
			for (size_t j = n - 1; j-- > 0; ) {
				int next = buf[j + 1];
				if (row[j] >= next)
					buf[j] = 1;
				else
					buf[j] = next - row[j];
			}
		}
		for (size_t i = m - 1; i-- > 0; ) {
			auto &row = data[i];
			int next = buf[n - 1];
			if (row[n - 1] >= next)
				buf[n - 1] = 1;
			else
				buf[n - 1] = next - row[n - 1];
			for (size_t j = n - 1; j-- > 0; ) {
				next = std::min(buf[j], buf[j + 1]);
				if (row[j] >= next)
					buf[j] = 1;
				else
					buf[j] = next - row[j];
			}
		}
		return buf[0];
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
check(const std::vector<std::vector<int>>& data, int expected)
{
	Solution sol;
	auto got = sol.calculateMinimumHP(data);
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
	check({{0}}, 1);
	check({{-2,-3,3},{-5,-10,1},{10,30,-5}}, 7);
}
