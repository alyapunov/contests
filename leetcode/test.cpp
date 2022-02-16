#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
	int longestValidParentheses(const string& s) {
		int longest_yard = 0;
		const char *p = s.data();
		int i,j,sum = 0;
		for (i = 0, j = 0; p[i]; i++) {
			sum += p[i] == '(' ? 1 : -1;
			if (sum == 0) {
				int l = i - j + 1;
				if (longest_yard < l)
					longest_yard = l;
			} else if (sum < 0) {
				j = i + 1;
				sum = 0;
			}
		}
		sum = 0;
		int m = j;
		j = s.size() - 1;
		for (i = j; i > m; i--) {
			sum += p[i] == ')' ? 1 : -1;
			if (sum == 0) {
				int l = j - i + 1;
				if (longest_yard < l)
					longest_yard = l;
			} else if (sum < 0) {
				j = i - 1;
				sum = 0;
			}
		}
		return longest_yard;
	}
};

void
check(const string& s, int expected)
{
	Solution sol;
	int got = sol.longestValidParentheses(s);
	if (got != expected) {
		std::cout << s << "\n";
		std::cout << "Expected " << expected << ", got " <<  got << "\n";
		abort();
	}
}

int main()
{
	check("", 0);
	check("(()", 2);
	check(")()())", 4);
	check(")))(())()(())(((", 10);
	check(")))((((((()))))))()((((((()))))))(((", 30);
	check(")(())(", 4);
	check(")(()))", 4);
	check("((())(", 4);
	check("((((((", 0);
	check("))))))", 0);
}
