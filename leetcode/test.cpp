#include <array>
#include <cstring>
#include <iostream>
#include <string>

static constexpr size_t MULT = 1;

static constexpr size_t MAX_SIZE = 1000 * MULT;

static constexpr std::array<size_t, 128> gen_mapper()
{
	std::array<size_t, 128> res{};
	for (char c = 'a'; c <= 'z'; ++c)
		res[c] = c - 'a';
	return res;
}

static constexpr std::array<size_t, 128> mapper = gen_mapper();

static constexpr std::array<std::array<unsigned, 4>, 16> build_masks()
{
	std::array<std::array<unsigned, 4>, 16> res{};
	for (size_t i = 0; i < 16; i++)
		for (size_t j = 0; j < 4; j++)
			res[i][j] = (i & (1u << j)) ? 0xFFFFFFFFu : 0;
	return res;
}

static constexpr std::array<std::array<unsigned, 4>, 16> masks = build_masks();

struct Bitset {
	uint8_t arr[(MAX_SIZE + 7) / 8];
	void set(size_t i) { arr[i / 8] |= (1 << (i % 8)); }
	uint8_t get(size_t i) const { return (arr[i / 8] >> (i % 8)) & 0x1; }
	uint8_t hex(size_t i) const { return (arr[i / 8] >> (i % 8)) & 0xF; }
};

struct Data {
	std::array<unsigned int, MAX_SIZE+1> buf;
	Bitset bits[128];
	bool is;
} data;

class Solution {
public:
	int numDistinct(const std::string& s, const std::string& t)
	{
		if (data.is)
			memset(&data, 0, sizeof(data));
		data.is = true;
		data.buf[t.size()] = 1;
		for (size_t j = 0; j < t.size(); j++)
			data.bits[(unsigned char)t[j]].set(j);

		for (size_t i = s.size(); i > 0; ) {
			i--;
			Bitset& bs = data.bits[(unsigned char)s[i]];
			size_t lim = (t.size() + 3u) & ~3u;
			for (size_t j = 0; j < lim; j += 4) {
				const std::array<unsigned, 4> &m = masks[bs.hex(j)];
				uint64_t a, b, c;

				memcpy(&a, &data.buf[j + 1], sizeof(a));
				memcpy(&b, &m[0], sizeof(b));
				memcpy(&c, &data.buf[j + 0], sizeof(c));
				c = (c + (a & b)) & 0x7FFFFFFF7FFFFFFFull;
				memcpy(&data.buf[j + 0], &c, sizeof(c));

				memcpy(&a, &data.buf[j + 3], sizeof(a));
				memcpy(&b, &m[2], sizeof(b));
				memcpy(&c, &data.buf[j + 2], sizeof(c));
				c = (c + (a & b)) & 0x7FFFFFFF7FFFFFFFull;
				memcpy(&data.buf[j + 2], &c, sizeof(c));
			}
		}
		return data.buf[0];
	}
};

void
check(const std::string& s, const std::string& t, int exp)
{
	Solution sol;
	int res = sol.numDistinct(s, t);
	if (res != exp) {
		std::cout << s << " and " << t << " : "
			  << "expected " << exp << " got " << res << std::endl;
		abort();
	}
}


int main()
{
	check("xslledayhxhadmctrliaxqpokyezcfhzaskeykchkmhpyjipxtsuljkwkovmvelvwxzwieeuqnjozrfwmzsylcwvsthnxujvrkszqwtglewkycikdaiocglwzukwovsghkhyidevhbgffoqkpabthmqihcfxxzdejletqjoxmwftlxfcxgxgvpperwbqvhxgsbbkmphyomtbjzdjhcrcsggleiczpbfjcgtpycpmrjnckslrwduqlccqmgrdhxolfjafmsrfdghnatexyanldrdpxvvgujsztuffoymrfteholgonuaqndinadtumnuhkboyzaqguwqijwxxszngextfcozpetyownmyneehdwqmtpjloztswmzzdzqhuoxrblppqvyvsqhnhryvqsqogpnlqfulurexdtovqpqkfxxnqykgscxaskmksivoazlducanrqxynxlgvwonalpsyddqmaemcrrwvrjmjjnygyebwtqxehrclwsxzylbqexnxjcgspeynlbmetlkacnnbhmaizbadynajpibepbuacggxrqavfnwpcwxbzxfymhjcslghmajrirqzjqxpgtgisfjreqrqabssobbadmtmdknmakdigjqyqcruujlwmfoagrckdwyiglviyyrekjealvvigiesnvuumxgsveadrxlpwetioxibtdjblowblqvzpbrmhupyrdophjxvhgzclidzybajuxllacyhyphssvhcffxonysahvzhzbttyeeyiefhunbokiqrpqfcoxdxvefugapeevdoakxwzykmhbdytjbhigffkmbqmqxsoaiomgmmgwapzdosorcxxhejvgajyzdmzlcntqbapbpofdjtulstuzdrffafedufqwsknumcxbschdybosxkrabyfdejgyozwillcxpcaiehlelczioskqtptzaczobvyojdlyflilvwqgyrqmjaeepydrcchfyftjighntqzoo", "rwmimatmhydhbujebqehjprrwfkoebcxxqfktayaaeheys", 543744000);
	check("a", "a", 1);
	check("a", "b", 0);
	check("aaaa", "a", 4);
	check("aaaa", "aa", 6);
	check("rabbbit", "rabbit", 3);
	check("babgbag", "bag", 5);
	std::string s(1000 * MULT, 'a');
	std::string t(1000 * MULT, 'a');
	check(s, t, 1);
	for (char &c : s)
		c = (rand() & 1) ? 'a' : 'b';
	for (char &c : t)
		c = (rand() & 1) ? 'a' : 'b';
	check(s, t, 0);
}
