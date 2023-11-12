#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <set>

//using namespace std;

struct border {
	int coord;
	int height;
	bool operator<(const border& that) const { return coord < that.coord; }
};

border left_border[10001];
border right_border[10001];

class Solution {
public:
	std::vector<std::vector<int>> getSkyline(const std::vector<std::vector<int>>& buildings) {
		size_t n = 0;
		for (const auto &b : buildings) {
			left_border[n].coord = b[0];
			left_border[n].height = b[2];
			right_border[n].coord = b[1];
			right_border[n].height = b[2];
			n++;
		}
		std::sort(right_border, right_border + n);
		left_border[n].coord = -1;
		right_border[n].coord = -1;

		std::vector<std::vector<int>> res;
		size_t i = 0, j = 0;
		int x, h = 0;
		std::multiset<int> heights;
		while (i < n) {
			if (left_border[i].coord < right_border[j].coord) {
				x = left_border[i].coord;
				do {
					heights.insert(left_border[i++].height);
				} while (x == left_border[i].coord);
			} else if (left_border[i].coord > right_border[j].coord) {
				x = right_border[j].coord;
				do {
					heights.erase(heights.find(right_border[j++].height));
				} while (x == right_border[j].coord);
			} else {
				x = left_border[i].coord;
				do {
					heights.insert(left_border[i++].height);
				} while (x == left_border[i].coord);
				do {
					heights.erase(heights.find(right_border[j++].height));
				} while (x == right_border[j].coord);
			}
			int nh = heights.empty() ? 0 : *heights.rbegin();
			if (nh != h) {
				res.push_back({x, nh});
				h = nh;
			}
		}
		while (j < n) {
			x = right_border[j].coord;
			do {
				heights.erase(heights.find(right_border[j++].height));
			} while (x == right_border[j].coord);

			int nh = heights.empty() ? 0 : *heights.rbegin();
			if (nh != h) {
				res.push_back({x, nh});
				h = nh;
			}
		}
		return res;
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

template <class T>
bool
any_order_equal(const T& a, const T& b)
{
	return a == b;
}

template <class T>
bool
any_order_equal(const std::vector<T>& a, const std::vector<T>& b)
{
	if (a.size() != b.size())
		return false;
	for (size_t i = 0; i < a.size(); i++) {
		const T& x = a[i];
		size_t c1 = 0, c2 = 0;
		for (const auto &y : a)
			if (any_order_equal(x, y))
				c1++;
		for (const auto &y : b)
			if (any_order_equal(x, y))
				c2++;
		if (c1 != c2)
			return false;
	}
	return true;
}

void
check(const std::vector<std::vector<int>>& data, const std::vector<std::vector<int>>& expected)
{
	Solution sol;
	auto got = sol.getSkyline(data);
	if (got != expected) {
	//if (!any_order_equal(got, expected)) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check({{2,9,10},{3,7,15},{5,12,12},{15,20,10},{19,24,8}}, {{2,10},{3,15},{7,12},{12,0},{15,10},{20,8},{24,0}});
	check({{0,2,3},{2,5,3}}, {{0,3},{5,0}});
}
