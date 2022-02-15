#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <unordered_map>

using namespace std;

/*
	1 <= s.length <= 10000
	s consists of lower-case English letters.
	1 <= words.length <= 5000
	1 <= words[i].length <= 30
	words[i] consists of lower-case English letters.
 */

struct StrInfo {
	const string *str;
	uint64_t req;
	bool operator <(const StrInfo &a) const
	{
		return *this->str < *a.str;
	}
};

StrInfo info[5000];

size_t group(size_t count)
{
	std::sort(info, info + count);
	for (size_t i = 0; i < count; i++)
		info[i].req = 1;
	size_t new_count = 0;
	for (size_t i = 1; i < count; ) {
		if (*info[new_count].str == *info[i].str) {
			info[new_count].req++;
		} else {
			new_count++;
			info[new_count] = info[i];
		}
		i++;
	}
	return new_count + 1;
}

struct CircleHasher {
	uint64_t R = 1009;
	uint64_t Q = INT32_MAX;
	size_t L;
	uint64_t K;
	CircleHasher(size_t l) {
		L = l;
		l--;
		K = 1;
		uint64_t p = R;
		while (l) {
			if (l & 1)
				K = K * p % Q;
			p = p * p % Q;
			l >>= 1;
		}
		K = Q - K;
	}

	uint64_t get(const char *str)
	{
		uint64_t h = 0;
		for (size_t i = 0; i < L; i++)
			h = (h * R + (uint8_t)(*str++)) % Q;
		return h;
	}

	uint64_t next(uint64_t h, const char *str)
	{
		return ((h + (uint8_t)str[0] * K) * R + (uint8_t)str[L]) % Q;
	}
};

uint64_t match[10000];

struct Hits {
	uint64_t hit[5000];
	Hits(uint64_t c) : total_hits(0), wcount(c) {
		fill(hit, hit + wcount, 0);
	}
	uint64_t total_hits;
	uint64_t wcount;
	bool match(uint64_t w)
	{
		if (w == UINT64_MAX)
			return false;
		if (hit[w] == info[w].req)
			total_hits--;
		if (++hit[w] == info[w].req)
			total_hits++;
		return total_hits == wcount;
	}
	void unmatch(uint64_t w)
	{
		if (w == UINT64_MAX)
			return;
		if (hit[w] == info[w].req)
			total_hits--;
		if (--hit[w] == info[w].req)
			total_hits++;
	}
};

class Solution {
public:
	vector<int> findSubstring(const string& s, const vector<string>& words)
	{
		if (words.size() * words[0].size() > s.size())
			return {};
		size_t wcount = 0;
		for (const auto& str : words)
			info[wcount++].str = &str;
		size_t icount = group(wcount);
		size_t l = words[0].size();
		CircleHasher hasher(l);
		std::unordered_map<uint64_t, size_t> by_hash;
		by_hash.reserve(icount);
		for (size_t i = 0; i < icount; i++) {
			uint64_t h = hasher.get(info[i].str->data());
			by_hash.emplace(h, i);
		}
		const char *str = s.data();
		uint64_t h;
		for (size_t i = 0; i <= s.size() - l; i++) {
			match[i] = UINT64_MAX;
			h = i == 0 ? hasher.get(str) : hasher.next(h, str++);
			auto p = by_hash.equal_range(h);
			for (auto itr = p.first; itr != p.second; ++itr) {
				auto &inf = info[itr->second];
				if (memcmp(inf.str->data(), str, l) == 0)
					match[i] = itr->second;
			}
		}

		vector<int> res;
		for (size_t i = 0; i < l; i++) {
			if (i + (wcount - 1) * l > s.size() - l)
				break;
			Hits hits(icount);
			bool last;
			for (size_t j = 0; j < wcount; j++)
				last = hits.match(match[i + j * l]);
			if (last)
				res.push_back(i);

			for (size_t j = 0; i + (j + wcount) * l <= s.size() - l; j++) {
				hits.unmatch(match[i + j * l]);
				if (hits.match(match[i + (j + wcount) * l]))
					res.push_back(i + (j + 1) * l);
			}
		}

		return res;
	}
};

void
check(const string& str, const vector<string>& words, const vector<int>& res)
{

	Solution s;
	auto got = s.findSubstring(str, words);
	std::sort(got.begin(), got.end());
	if (got != res) {
		std::cout << "Wrong result! expected:\n";
		for (auto i : res)
			std::cout << i << " ";
		std::cout << "\nGot:\n";
		for (auto i : got)
			std::cout << i << " ";
		std::cout << "\n";
		abort();
	}
}

int main()
{
	check("barfoothefoobarman", {"foo","bar"}, {0, 9});
	check("wordgoodgoodgoodbestword", {"word","good","best","word"}, {});
	check("barfoofoobarthefoobarman", {"bar","foo","the"}, {6, 9, 12});
	check("aaaaaaaaaaaa", {"aaa","aaa","aaa"}, {0, 1, 2, 3});
	check("aaaaabaaaaaa", {"aaa","aba","aaa"}, {1});
	check("acccabaaaaaa", {"aaa","aba","aaa"}, {});
	check("aaaaaa", {"aaa","aaa"}, {0});
}
