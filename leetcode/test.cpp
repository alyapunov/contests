#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <vector>
#include <string.h>

//using namespace std;

struct Range {
	unsigned b,e;
};

Range presort[26];

unsigned presort_data[1024];

struct CacheItem {
	unsigned word;
	unsigned len;
};

struct CacheEntry {
	unsigned count;
	CacheItem items[10];
};

CacheEntry cache[21];

class Solution {
public:
	static void collect(unsigned *stack, size_t stack_size,
		     size_t string_pos, size_t string_size,
		     const std::vector<std::string> &wordDict,
		     std::vector<std::string> &res)
	{
		if (string_pos == string_size) {
			char data[40];
			size_t data_len = wordDict[stack[0]].size();
			memcpy(data, wordDict[stack[0]].data(), data_len);
			for (size_t i = 1; i < stack_size; i++) {
				data[data_len++] = ' ';
				size_t l = wordDict[stack[i]].size();
				memcpy(data + data_len, wordDict[stack[i]].data(), l);
				data_len += l;
			}
			res.emplace_back(data, data_len);
			return;
		}
		const auto &c = cache[string_pos];
		for (unsigned i = 0; i < c.count; i++) {
			stack[stack_size] = c.items[i].word;
			collect(stack, stack_size + 1, string_pos + c.items[i].len,
				string_size, wordDict, res);
		}
	}

	std::vector<std::string> wordBreak(const std::string &s,
					   const std::vector<std::string> &wordDict)
	{
		for (auto &r : presort)
			r.e = 0;
		for (auto &w : wordDict)
			presort[w[0] - 'a'].e++;
		unsigned sum = 0;
		for (auto &r : presort) {
			r.b = sum;
			sum += r.e;
			r.e = r.b;
		}
		unsigned wds = wordDict.size();
		for (unsigned i = 0; i < wds; i++) {
			presort_data[presort[wordDict[i][0] - 'a'].e++] = i;
		}

		size_t ss = s.size();
		const char *sd = s.data();
		cache[ss].count = 1;
		for (size_t i = ss; i--> 0; ) {
			cache[i].count = 0;
			char c = s[i];
			unsigned b = presort[c - 'a'].b;
			unsigned e = presort[c - 'a'].e;
			for (unsigned j = b; j < e; j++) {
				unsigned wid = presort_data[j];
				const auto &w = wordDict[wid];
				size_t ws = w.size();
				const char *wd = w.data();
				if (i + ws > ss || cache[i + ws].count == 0)
					continue;
				if (memcmp(sd + i + 1, wd + 1, ws - 1) != 0)
					continue;
				cache[i].items[cache[i].count].word = wid;
				cache[i].items[cache[i].count].len = ws;
				cache[i].count++;
			}
		}

		std::vector<std::string> res;
		unsigned stack[20];
		collect(stack, 0, 0, ss, wordDict, res);
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
check(const std::string &s,
      const std::vector<std::string> &wordDict,
      const std::vector<std::string> &expected)
{
	Solution sol;
	auto got = sol.wordBreak(s, wordDict);
	if (got != expected) {
		std::cout << s << ", " << wordDict
			  << " : "
			  << "expected " << expected
			  << " got " << got << std::endl;
		abort();
	}
}

int main()
{
	check("catsandog", {"cats","dog","sand","and","cat"},
	      {});
	check("catsanddog", {"cat", "cats", "and", "sand", "dog"},
	      {"cat sand dog", "cats and dog"});
	check("pineapplepenapple", {"apple","pen","applepen","pine","pineapple"},
	      {"pine apple pen apple","pine applepen apple","pineapple pen apple"});
}
