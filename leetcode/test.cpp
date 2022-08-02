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


struct Info {
	uint16_t b;
	uint16_t e;
};

struct Data {
	std::array<unsigned int, MAX_SIZE+1> buf;
	std::array<Info, 128> info;
	std::array<uint16_t, MAX_SIZE> info_data;
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
			data.info[(unsigned char)t[j]].e++;
		uint16_t sum = 0;
		for (auto &inf : data.info) {
			inf.b = sum;
			sum += inf.e;
			inf.e = inf.b;
		}
		for (size_t j = 0; j < t.size(); j++)
			data.info_data[data.info[(unsigned char)t[j]].e++] = j;

		for (size_t i = s.size(); i > 0; ) {
			i--;
			auto &inf = data.info[(unsigned char)s[i]];
			for (uint16_t j = inf.b; j < inf.e; j++) {
				uint16_t k = data.info_data[j];
				data.buf[k] += data.buf[k + 1];
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
