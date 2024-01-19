#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

//using namespace std;

auto init = []()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	return 0;
}();

template <size_t MAX_LENGTH, class OFF_T>
struct KMP {
	OFF_T prefix[MAX_LENGTH];
	const char *str;
	OFF_T size;
	OFF_T pos;
	void init(const char *str_, OFF_T size_)
	{
		str = str_;
		size = size_;
		prefix[0] = 0;
		for (OFF_T i = 1, j = 0; i < size; i++) {
			while (true) {
				if (str[i] == str[j]) {
					prefix[i] = ++j;
					break;
				} else if (j == 0) {
					prefix[i] = 0;
					break;
				} else {
					j = prefix[j - 1];
				}
			}
		}
		pos = 0;
	}
	bool next(char c)
	{
		while (true) {
			if (c == str[pos]) {
				pos++;
				if (pos == size) {
					pos = prefix[pos - 1];
					return true;
				} else {
					return false;
				}
			} else if (pos == 0) {
				return false;
			} else {
				pos = prefix[pos - 1];
			}
		}
	}
};

KMP<500000, int> sa, sb;

class Solution {
public:
	std::vector<int>
	beautifulIndices(const std::string& s, const std::string& a,
			 const std::string& b, int k)
	{
		std::vector<int> res;
		int sl = s.size(), al = a.size(), bl = b.size();
		sa.init(a.data(), a.size());
		sb.init(b.data(), b.size());
		int j = 0;
		while (!sb.next(s[j++])) {
			if (j == sl)
				return res;
		}
		int y = j - bl;
		for (int i = 0; i < sl; ) {
			if (!sa.next(s[i++]))
				continue;
			int x = i - al;
			while (true) {
				if (y <= x) {
					if (x - y <= k) {
						res.push_back(x);
						break;
					}
					do {
						if (j == sl)
							return res;
					} while (!sb.next(s[j++]));
					y = j - bl;
				} else {
					if (y - x <= k)
						res.push_back(x);
					break;
				}
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
check(const std::string& s, const std::string& a, const std::string& b, int k,
      const std::vector<int>& expected)
{
	Solution sol;
	auto got = sol.beautifulIndices(s, a, b, k);
	if (got != expected) {
	//if (!any_order_equal(got, expected)) {
		std::cout << s << ", " << a << ", " << b << ", " << k
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check("isawsquirrelnearmysquirrelhouseohmy", "my", "squirrel", 15, {16, 33});
	check("abcd", "a", "a", 4, {0});
}
