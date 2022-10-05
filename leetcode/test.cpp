#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <unordered_map>

using namespace std;

using conv_t = uint64_t;
using word_t = uint16_t;
constexpr size_t MAX_LETTERS = 10;
constexpr conv_t MAX_WORDS = 5001;

struct Bitmask {
	uint64_t arr[MAX_WORDS / 64 + 1];
	void clear(word_t num_words)
	{
		for (word_t i = 0; i < num_words / 64 + 1; i++)
			arr[i] = 0;
	}
	void set(word_t w)
	{
		arr[w / 64] |= 1ull << (w % 64);
	}
	bool has(word_t w)
	{
		return arr[w / 64] & (1ull << (w % 64));
	}
};

struct Node {
	word_t id;
};

conv_t words[MAX_WORDS];
std::unordered_multimap<conv_t, word_t> maps[MAX_LETTERS];
Node front1[MAX_WORDS];
Node front2[MAX_WORDS];
Bitmask bitmask;

class Solution {
public:
	size_t N;
	conv_t convert(const string &word)
	{
		uint64_t res = 0;
		for (size_t i = 0; i < N; i++)
			res = (res << 5) | (word[i] + 1 - 'a');
		return res;
	}
	int
	ladderLength(const std::string &beginWord, const std::string &endWord,
		    const std::vector<std::string> &wordList)
	{
		N = beginWord.size();
		uint64_t b = convert(beginWord);
		uint64_t e = convert(endWord);

		size_t num_words = 0;
		for (size_t i = 0; i < N; i++)
			maps[i].clear();
		bitmask.clear(wordList.size());

		words[num_words++] = b;
		for (conv_t j = 0, mask = 0x1F; j < N; j++, mask <<= 5)
			maps[j].insert(std::make_pair(b &~ mask, 0));

		word_t target = 0;
		for (word_t i = 0; i < wordList.size(); i++) {
			conv_t w = convert(wordList[i]);
			words[num_words++] = w;
			if (w == b)
				continue;
			if (w == e)
				target = i + 1;
			for (conv_t j = 0, mask = 0x1F; j < N; j++, mask <<= 5)
				maps[j].insert(std::make_pair(w &~ mask, i + 1));
		}
		if (target == 0)
			return 0;

		int res = 2;
		front1[0].id = 0;
		bitmask.set(0);
		size_t f1_size = 1, f2_size = 0;
		Node *f1 = front1;
		Node *f2 = front2;

		while (true) {
			f2_size = 0;
			for (size_t i = 0; i < f1_size; i++) {
				conv_t w = words[f1[i].id];
				for (conv_t j = 0, mask = 0x1F; j < N; j++, mask <<= 5) {
					auto r = maps[j].equal_range(w &~ mask);
					for (auto itr = r.first; itr != r.second; ++itr) {
						word_t next = itr->second;
						if (bitmask.has(next))
							continue;
						if (next == target)
							return res;
						bitmask.set(next);
						f2[f2_size++].id = next;
					}
				}
			}

			if (f2_size == 0)
				return 0;

			std::swap(f1, f2);
			f1_size = f2_size;
			res++;
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

void
check(const std::string &beginWord, const std::string &endWord,
      const std::vector<std::string> &wordList, int expected)
{
	Solution sol;
	auto got = sol.ladderLength(beginWord, endWord, wordList);
	if (got != expected) {
		std::cout << beginWord << "->" << endWord << " " << wordList
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check("a", "b", {"b"}, 2);
	check("hit", "cog", {"hot","dot","dog","lot","log","cog"}, 5);
	check("hit", "cog", {"hit","hot","dot","dog","lot","log","cog"}, 5);
	check("hit", "cog", {"hot","dot","dog","lot","log"}, 0);
	check("hit", "cog", {"cog"}, 0);
//	check("a", "b", {"b"}, {{"a","b"}});
//	check("hit", "cog", {"hot","dot","dog","lot","log","cog"},
//	      {{"hit","hot","dot","dog","cog"},{"hit","hot","lot","log","cog"}});
//	check("hit", "cog", {"hit","hot","dot","dog","lot","log","cog"},
//	      {{"hit","hot","dot","dog","cog"},{"hit","hot","lot","log","cog"}});
//	check("hit", "cog", {"hot","dot","dog","lot","log"}, {});
//	check("hit", "cog", {"cog"}, {});
}
