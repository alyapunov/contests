#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

static constexpr size_t MULT = 1;

static constexpr size_t MAX_SIZE = 1000 * MULT;

struct Info {
	uint16_t b;
	uint16_t c;
	uint16_t e;
};

struct Data {
	std::array<unsigned int, MAX_SIZE+1> buf;
	std::array<Info, 128> info;
	std::array<uint16_t, MAX_SIZE> info_global;
	bool is;
} global;

class Solution {
public:
	int numDistinct(const std::string& s, const std::string& t)
	{
		if (global.is)
			memset(&global, 0, sizeof(global));
		global.is = true;
		global.buf[t.size()] = 1;

		for (size_t j = 0; j < t.size(); j++)
			global.info[(unsigned char)t[j]].e++;
		uint16_t sum = 0;
		for (auto &inf : global.info) {
			inf.b = sum;
			sum += inf.e;
			inf.c = inf.b + inf.e;
			inf.e = inf.b;
		}
		for (size_t j = 0; j < t.size(); j++)
			global.info_global[global.info[(unsigned char)t[j]].e++] = j;

		uint16_t m = t.size();
		for (size_t i = s.size(); i > 0; ) {
			i--;
			auto &inf = global.info[(unsigned char)s[i]];
			while (inf.c > inf.b && global.info_global[inf.c - 1] + 1 >= m)
				inf.c--;
			for (uint16_t j = inf.c; j < inf.e; j++) {
				uint16_t k = global.info_global[j];
//				assert(global.buf[k + 1] != 0);
				if (k > i)
					break;
				global.buf[k] += global.buf[k + 1];
			}
			if (inf.c != inf.e && global.info_global[inf.c] <= i)
				m = std::min(m, global.info_global[inf.c]);
		}
		return global.buf[0];
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
	check("a", "a", 1);
	check("a", "b", 0);
	check("aaaa", "a", 4);
	check("aaaa", "aa", 6);
	check("rabbbit", "rabbit", 3);
	check("babgbag", "bag", 5);
	check("xslledayhxhadmctrliaxqpokyezcfhzaskeykchkmhpyjipxtsuljkwkovmvelvwxzwieeuqnjozrfwmzsylcwvsthnxujvrkszqwtglewkycikdaiocglwzukwovsghkhyidevhbgffoqkpabthmqihcfxxzdejletqjoxmwftlxfcxgxgvpperwbqvhxgsbbkmphyomtbjzdjhcrcsggleiczpbfjcgtpycpmrjnckslrwduqlccqmgrdhxolfjafmsrfdghnatexyanldrdpxvvgujsztuffoymrfteholgonuaqndinadtumnuhkboyzaqguwqijwxxszngextfcozpetyownmyneehdwqmtpjloztswmzzdzqhuoxrblppqvyvsqhnhryvqsqogpnlqfulurexdtovqpqkfxxnqykgscxaskmksivoazlducanrqxynxlgvwonalpsyddqmaemcrrwvrjmjjnygyebwtqxehrclwsxzylbqexnxjcgspeynlbmetlkacnnbhmaizbadynajpibepbuacggxrqavfnwpcwxbzxfymhjcslghmajrirqzjqxpgtgisfjreqrqabssobbadmtmdknmakdigjqyqcruujlwmfoagrckdwyiglviyyrekjealvvigiesnvuumxgsveadrxlpwetioxibtdjblowblqvzpbrmhupyrdophjxvhgzclidzybajuxllacyhyphssvhcffxonysahvzhzbttyeeyiefhunbokiqrpqfcoxdxvefugapeevdoakxwzykmhbdytjbhigffkmbqmqxsoaiomgmmgwapzdosorcxxhejvgajyzdmzlcntqbapbpofdjtulstuzdrffafedufqwsknumcxbschdybosxkrabyfdejgyozwillcxpcaiehlelczioskqtptzaczobvyojdlyflilvwqgyrqmjaeepydrcchfyftjighntqzoo", "rwmimatmhydhbujebqehjprrwfkoebcxxqfktayaaeheys", 543744000);
	std::string s(1000 * MULT, 'a');
	std::string t(1000 * MULT, 'a');
	check(s, t, 1);
	for (char &c : s)
		c = (rand() & 1) ? 'a' : 'b';
	for (char &c : t)
		c = (rand() & 1) ? 'a' : 'b';
	check(s, t, 0);
}
