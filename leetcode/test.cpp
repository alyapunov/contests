#include <iostream>
#include <string>

static constexpr size_t MULT = 1;

unsigned int buf[1000 * MULT + 1];

class Solution {
public:
	int numDistinct(const std::string& s, const std::string& t)
	{
		for (size_t i = 0; i < t.size(); i++)
			buf[i] = 0;
		buf[t.size()] = 1;
		for (size_t i = s.size(); i > 0; ) {
			i--;
			for (size_t j = 0; j < t.size(); j++) {
				if (t[j] == s[i])
					buf[j] += buf[j + 1];
			}
		}
		return buf[0];
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
