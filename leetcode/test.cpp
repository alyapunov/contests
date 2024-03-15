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

enum rpt_type {
	RPT_OPEN,
	RPT_CLOSE,
	RPT_OTHER,
};

rpt_type get_type(char c)
{
	if (c == '(')
		return RPT_OPEN;
	if (c == '(')

}

struct Grp {
	size_t remove_count;
	size_t open_count;
	size_t close_count;
};

struct Rpt {
	rpt_type type;

	size_t orig_count;
	size_t min_count;
	size_t max_count;
	size_t count;

	const char *begin;

	Grp *grp;

	void init(const char &c)
	{
		type = get_type(c);
		orig_count = 1;
		count = 1;
		begin = &c;
	}
	bool is_appendable(char c)
	{
		return type == get_type(c);
	}
	void append()
	{
		orig_count++;
		count++;
	}
	void build(std::string& res)
	{
		res += std::string_view(begin, count);
	}
	void unbuild(std::string& res)
	{
		res.erase(res.size() - count, count);
	}
};

class Solution {
public:
	std::vector<std::string> removeInvalidParentheses(const std::string& s)
	{
		if (s.empty())
			return {""};

		Rpt rpt[25];
		size_t num_rpt = 0;
		rpt[num_rpt].init(s[0]);
		for (size_t i = 1; i < s.size(); i++) {
			if (rpt[num_rpt].is_appendable(s[i]))
				rpt[num_rpt].append();
			else
				rpt[++num_rpt].init(s[i]);
		}
		++num_rpt;
		Rpt *rpt_begin = rpt[0].type == RPT_CLOSE ? &rpt[1] : &rpt[0];
		Rpt *rpt_end =
			rpt[num_rpt - 1].type == RPT_OPEN ? &rpt[num_rpt - 1]
							  : &rpt[num_rpt];
		if (rpt_begin >= rpt_end)
			return {""};

		Grp grp[25];
		size_t num_grp = 0;
		grp[num_grp].open_count = grp[num_grp].close_count = 0;
		Rpt *rpt_mid = rpt_begin;
		ssize_t sum = 0;
		size_t remove_closed_total = 0;
		for(Rpt *r = rpt_begin; r < rpt_end; r++) {
			r->grp = &grp[num_grp];
			if (r->type == RPT_OPEN) {
				grp[num_grp].open_count += r->count;
				sum += r->count;
			} else if (r->type == RPT_CLOSE) {
				grp[num_grp].close_count += r->count;
				sum -= r->count;
				if (sum <= 0) {
					grp[num_grp].remove_count = -sum;
					remove_closed_total += -sum;
					num_grp++;
					sum = 0;
					rpt_mid = r + 1;
				}
			}
		}


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
check(const std::string& data, const std::vector<std::string>& expected)
{
	Solution sol;
	auto got = sol.removeInvalidParentheses(data);
	//if (got != expected) {
	if (!any_order_equal(got, expected)) {
		std::cout << data
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check("()())()", {"(())()","()()()"});
	check("(a)())()", {"(a())()","(a)()()"});
	check(")(", {""});
	check(")", {""});
	check("(", {""});
	check("((((((((((((((((((((aaaaa", {"aaaaa"});
	check("))))))))))))))))))))aaaaa", {"aaaaa"});
}
