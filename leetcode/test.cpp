#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

using conv_t = uint32_t;
using word_id_t = uint16_t;
constexpr size_t MAX_LETTERS = 5;
constexpr conv_t MAX_WORDS = 500;

struct WordInfo {
	conv_t conv;
	word_id_t in_list;
	word_id_t dist;
};

size_t num_words;
WordInfo words[MAX_WORDS];

conv_t path[MAX_WORDS + 1];

std::unordered_multimap<conv_t, word_id_t> maps[MAX_LETTERS];

class Solution {
public:
	size_t N;
	conv_t convert(const string &word)
	{
		conv_t res = 0;
		for (size_t i = 0; i < N; i++)
			res = (res << 5) | (word[i] + 1 - 'a');
		return res;
	}

	void collectRes(std::vector<std::vector<std::string>> &res,
			word_id_t dist,
			std::vector<word_id_t> &tmp,
			const std::vector<std::string> &wordList)
	{
		if (dist == 0) {
			std::vector<std::string> x;
			for (auto id : tmp)
				x.push_back(wordList[words[id].in_list]);
			res.push_back(std::move(x));
			return;
		}
		word_id_t id = tmp.back();
		conv_t c = words[id].conv;
		for (conv_t j = 0, msk = 0x1F; j < N; j++, msk <<= 5) {
			auto r = maps[j].equal_range(c &~ msk);
			for (auto it = r.first; it != r.second; ++it) {
				word_id_t next = it->second;
				if (words[next].dist != dist)
					continue;
				tmp.push_back(next);
				collectRes(res, dist - 1, tmp, wordList);
				tmp.pop_back();
			}
		}
	}

	std::vector<std::vector<std::string>>
	findLadders(const std::string &beginWord, const std::string &endWord,
		    const std::vector<std::string> &wordList)
	{
		N = beginWord.size();
		for (size_t i = 0; i < N; i++)
			maps[i].clear();
		num_words = 0;

		uint64_t b = convert(beginWord);
		uint64_t e = convert(endWord);

		word_id_t target = std::numeric_limits<word_id_t>::max();
		std::unordered_set<conv_t> inifier;
		for (word_id_t i = 0; i < wordList.size(); i++) {
			conv_t w = convert(wordList[i]);
			if (w == b)
				continue;
			if (inifier.count(w))
				continue;
			inifier.insert(w);

			word_id_t id = num_words++;
			if (w == e)
				target = id;

			words[id].conv = w;
			words[id].in_list = i;
			words[id].dist = 0;
			for (conv_t j = 0, mask = 0x1F; j < N; j++, mask <<= 5)
				maps[j].insert(std::make_pair(w &~ mask, id));
		}
		if (target == std::numeric_limits<word_id_t>::max())
			return {};

		path[0] = b;
		bool target_found = false;
		size_t step_end = 1;
		size_t path_end = 1;
		word_id_t dist = 1;
		for (size_t i = 0; ; ++i) {
			if (i == step_end) {
				if (step_end == path_end)
					return {};
				if (target_found)
					break;
				++dist;
				step_end = path_end;
			}
			for (conv_t j = 0, msk = 0x1F; j < N; j++, msk <<= 5) {
				auto r = maps[j].equal_range(path[i] &~ msk);
				for (auto it = r.first; it != r.second; ++it) {
					word_id_t next = it->second;
					if (next == target)
						target_found = true;
					if (words[next].dist != 0)
						continue;
					path[path_end++] = words[next].conv;
					words[next].dist = dist;
				}
			}
		}
		std::vector<std::vector<std::string>> res;
		std::vector<word_id_t> tmp;
		tmp.push_back(target);
		collectRes(res, dist - 1, tmp, wordList);

		for (auto& v : res) {
			v.push_back(beginWord);
			std::reverse(v.begin(), v.end());
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

void
check(const std::string &beginWord, const std::string &endWord,
      const std::vector<std::string> &wordList,
      const std::vector<std::vector<std::string>> &expected)
{
	Solution sol;
	auto got = sol.findLadders(beginWord, endWord, wordList);
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
	check("a", "b", {"b"}, {{"a","b"}});
	check("hit", "cog", {"hot","dot","dog","lot","log","cog"},
	      {{"hit","hot","lot","log","cog"}, {"hit","hot","dot","dog","cog"}});
	check("hit", "cog", {"hit","hot","dot","dog","lot","log","cog"},
	      {{"hit","hot","lot","log","cog"}, {"hit","hot","dot","dog","cog"}});
	check("hit", "cog", {"hot","dot","dog","lot","log"}, {});
	check("hit", "cog", {"cog"}, {});
	check("hit", "cog", {"hot","hot","hot","hot","hot","hot","dot","dog","lot","log","cog"},
	      {{"hit","hot","lot","log","cog"}, {"hit","hot","dot","dog","cog"}});
}
