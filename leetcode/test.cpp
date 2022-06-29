#include <alya.h>
#include <string>


unsigned int buf[1024];

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
}
