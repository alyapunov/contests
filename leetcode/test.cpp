#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>

struct Map {
	size_t m;
	size_t n;
	uint16_t bits[14];
	uint8_t cells[14][14];
	void init(const std::vector<std::vector<char>> &board) {
		m = board.size();
		n = board[0].size();
		bits[0] = 0xFFFF;
		bits[m + 1] = 0xFFFF;
		uint16_t b = 1 | (1 << (n + 1));
		for (size_t i = 1; i <= m; i++)
			bits[i] = b;
		for (size_t i = 0; i < m; i++)
			for (size_t j = 0; j < n; j++)
				cells[i + 1][j + 1] = board[i][j] - 'a';
	}
	bool test(size_t i, size_t j) {
		return bits[i] & (1 << j);
	}
	void flip(size_t i, size_t j) {
		bits[i] ^= (1 << j);
	}
} brdmap;

struct PathNode {
	int32_t parent;
	uint32_t parent_route;
	uint32_t fin;
	uint32_t next;
	int32_t route[26];
	void init(int32_t parent_, uint32_t parent_route_)
	{
		parent = parent_;
		parent_route = parent_route_;
		fin = 0;
		next = 0;
	}
};

struct Path {
	int32_t size;
	static constexpr int32_t SIZE = 200000;
	PathNode nodes[SIZE];
	void init() {
		nodes[0].init(-1, 0);
		size = 1;
	}
	void add(const std::string &str) {
		int32_t c = 0;
		size_t l = str.size() - 1;
		for (size_t i = 0; i < l; i++) {
			size_t r = str[i] - 'a';
			uint32_t m = 1 << r;
			if (nodes[c].next & m) {
				c = nodes[c].route[r];
			} else {
				nodes[c].next |= m;
				nodes[c].route[r] = size;
				nodes[size].init(c, r);
				c = size++;
			}
		}
		size_t r = str[l] - 'a';
		uint32_t m = 1 << r;
		nodes[c].fin |= m;
	}
	void drop(int32_t c, uint32_t r, uint32_t m)
	{
		//assert(nodes[c].fin & m);
		nodes[c].fin ^= m;
		if (nodes[c].fin || nodes[c].next)
			return;
		while (c != 0) {
			r = nodes[c].parent_route;
			c = nodes[c].parent;
			m = 1 << r;
			//assert(nodes[c].next & m);
			nodes[c].next ^= m;
			if (nodes[c].fin || nodes[c].next)
				return;
		}
	}
} path;

class Solution{
public:
	std::vector<std::string>
	findWords(const std::vector<std::vector<char>> &board,
		  const std::vector<std::string> &words)
	{
		brdmap.init(board);
		path.init();
		for (const auto& w : words)
			path.add(w);
		std::vector<std::string> res;
		for (size_t i = 1; i <= brdmap.m; i++) {
			for (size_t j = 1; j <= brdmap.n; j++) {
				go(i, j, 0, res);
			}
		}
		return res;
	}

	void go(size_t i, size_t j, int32_t c,
		std::vector<std::string> &res)
	{
		if (brdmap.test(i, j))
			return;
		uint8_t r = brdmap.cells[i][j];
		int32_t m = 1u << r;
		if (path.nodes[c].next & m) {
			brdmap.flip(i, j);
			int32_t nc = path.nodes[c].route[r];
			path.nodes[nc].fin |= 1 << 31;
			go(i + 1, j, nc, res);
			go(i - 1, j, nc, res);
			go(i, j + 1, nc, res);
			go(i, j - 1, nc, res);
			brdmap.flip(i, j);
			path.drop(nc, 31, 1 << 31);
		}
		if (path.nodes[c].fin & m) {
			char buf[12];
			char *e = buf + sizeof(buf);
			char *p = e;
			*--p = 'a' + r;
			int32_t tc = c;
			while (tc != 0) {
				*--p = 'a' + path.nodes[tc].parent_route;
				tc = path.nodes[tc].parent;
			}
			size_t l = e - p;
			//std::cout << "hit " << std::string{p, l} << "\n";
			res.emplace_back(std::string{p, l});
			path.drop(c, r, m);
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
check(const std::vector<std::vector<char>> &board,
      const std::vector<std::string> &words,
      const std::vector<std::string> &expected)
{
	Solution sol;
	auto got = sol.findWords(board, words);
	if (!any_order_equal(got, expected)) {
		std::cout << board
			  << " : "
			  << words
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check({{'a','b'}}, {"a", "b"}, {"a", "b"});
	check({{'a','b'}}, {"ab"}, {"ab"});
	check({{'o', 'a', 'a', 'n'},
	       {'e', 't', 'a', 'e'},
	       {'i', 'h', 'k', 'r'},
	       {'i', 'f', 'l', 'v'}},
	      {"oath", "pea", "eat", "rain"},
	      {"eat", "oath"});
	check({{'a','b'},{'c','d'}}, {"abcb"}, {});

	std::vector<char> row;
	for (size_t i = 0; i < 12; i++)
		row.push_back('a');
	std::vector<std::vector<char>> board;
	for (size_t i = 0; i < 12; i++)
		board.push_back(row);
	std::string word;
	for (size_t i = 0; i < 10; i++)
		word.push_back('a');
	std::vector<std::string> words;
	for (size_t i = 0; i < 30000; i++) {
		size_t c = i + 1;
		size_t p = 9;
		while (c > 0) {
			word[p] = c % 26 + 'a';
			c /= 26;
			p--;
		}
		words.push_back(word);
	}
	check(board, words, {});
}
