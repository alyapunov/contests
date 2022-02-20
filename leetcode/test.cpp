#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <string_view>
#include <utility>

using namespace std;

constexpr char spaces[101] = "                                                  "
			     "                                                  ";

constexpr std::string_view get_spaces(size_t count)
{
	return {spaces, count};
}

class Solution {
public:
	vector<string> fullJustify(const vector<string>& words, int maxWidth_) {
		size_t maxWidth = maxWidth_;
		vector<string> res;
		res.reserve(words.size());
		for (size_t i = 0, taken = 0; i < words.size(); i += taken) {
			res.emplace_back(words[i]);
			res.back().reserve(maxWidth);
			taken = 1;
			size_t len = words[i].size();
			while (true) {
				if (i + taken == words.size()) {
					for (size_t j = 1; j < taken; j++) {
						res.back() += get_spaces(1);
						res.back() += words[i + j];
					}
					res.back() += get_spaces(maxWidth - res.back().size());
					return res;
				}
				if (len + words[i + taken].size() + 1 > maxWidth)
					break;
				len += words[i + taken].size() + 1;
				taken++;
			}
			size_t more_spaces = maxWidth - len;
			if (taken == 1) {
				res.back() += get_spaces(more_spaces);
				continue;
			}
			size_t add = more_spaces / (taken - 1);
			size_t rnd = more_spaces % (taken - 1);
			for (size_t j = 1; j < taken; j++) {
				res.back() += get_spaces(1 + add + (j <= rnd));
				res.back() += words[i + j];
			}
		}
		return res;
	}
};

void
check(const vector<string>& words, int maxWidth, const vector<string>& res)
{
	Solution sol;
	auto my = sol.fullJustify(words, maxWidth);
	if (my != res)
	{
		std::cout << "Expected:" << "\n";
		for (const auto& s : res)
			std::cout << "\"" << s << "\"\n";
		std::cout << "Got:" << "\n";
		for (const auto& s : my)
			std::cout << "\"" << s << "\"\n";
		abort();
	}
}

int main()
{
	check({"This", "is", "an", "example", "of", "text", "justification."},
	      16, {"This    is    an",
		   "example  of text",
		   "justification.  "});
	check({"What","must","be","acknowledgment","shall","be"},
	      16, {"What   must   be",
		   "acknowledgment  ",
		   "shall be        "});
	check({"Science","is","what","we","understand","well","enough","to","explain","to","a","computer.","Art","is","everything","else","we","do"},
	      20, {"Science  is  what we",
		   "understand      well",
		   "enough to explain to",
		   "a  computer.  Art is",
		   "everything  else  we",
		   "do                  "});
}