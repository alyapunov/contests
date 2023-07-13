#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

struct Direction {
	int x;
	int y;
	void create(const std::vector<int>& origin, const std::vector<int>& point)
	{
		x = point[0] - origin[0];
		y = point[1] - origin[1];
		if (y < 0) {
			x = -x;
			y = -y;
		} else if (y == 0 && x < 0) {
			x = -x;
		}
	}
	bool operator<(const Direction &a) const {
		return x * a.y < a.x * y;
	}
	bool operator==(const Direction &a) const {
		return x * a.y == a.x * y;
	}
};

Direction cache[300];

class Solution {
public:
	int maxPoints(const std::vector<std::vector<int>>& points) {
		int max = 1;
		for (size_t i = 0, ii = 1; ii < points.size(); i++, ii++) {
			size_t n = 0;
			for (size_t j = ii; j < points.size(); j++)
				cache[n++].create(points[i], points[j]);
			std::sort(cache, cache + n);
			int m = 2;
			for (size_t j = 1; j < n; j++) {
				if (cache[j] == cache[j - 1]) {
					m++;
				} else {
					if (max < m)
						max = m;
					m = 2;
				}
			}
			if (max < m)
				max = m;
		}
		return max;
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
	auto got = sol.maxPoints(data);
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
	check({{0,0}}, 1);
	check({{1,1},{0,0}}, 2);
	check({{1,1},{3,2},{5,3},{4,1},{2,3},{1,4}}, 4);
}
