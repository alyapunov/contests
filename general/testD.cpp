#include <iostream>
#include <vector>
#include <string>


constexpr uint64_t MOD = 1000000007;

void sub(char *a, size_t s)
{
	for (char *p = a + s - 1; p >= a; --p) {
		if (*p != '0') {
			--*p;
			return;
		}
		*p = '9';
	}
}

void sub(char *a, size_t s, uint64_t* pwr)
{
	char *p = a + s - 1;
	for (; p >= a; --p) {
		if (*p != '0') {
			--*p;
			break;
		}
		*p = '9';
	}
	if (p >= a) {
		size_t off = p - a;
		size_t t = off == 0 ? 1 : pwr[off-1];
		while (off < s) {
			pwr[off] = t * (a[off] - '0' + 1) % MOD;
			t = pwr[off];
			++off;
		}
	}
}

uint64_t calc(char *a, char *b, size_t s, uint64_t* pwr)
{
	if (s == 1) {
		return std::min(*a - '0' + 1, *b - '0' + 1);
	}

	char l = b[s - 1];
	if (*a <= l)
		return (calc(a + 1, b, s - 1, pwr) +
			(*a - '0') * pwr[s - 2]) % MOD;

	uint64_t more1 = (l - '0' + 1) * pwr[s - 2];
	sub(b, s - 1, pwr);
	uint64_t more2 = (*a - l - 1) * pwr[s - 2];
	return (more1 + more2 + calc(a + 1, b, s - 1, pwr)) % MOD;
}

int main(int argn, const char** args)
{
#ifdef MY
	if (argn >= 2)
		freopen(args[1], "r", stdin);
	else if (freopen("./data1.txt", "r", stdin) == NULL)
		freopen("../data1.txt", "r", stdin);
#else
	(void)argn;
	(void)args;
#endif
	std::ios::sync_with_stdio(false);

	std::string a;
	std::string b;
	std::cin >> a >> b;

	sub(a.data(), a.size());
	sub(b.data(), b.size());
	if (a[0] == '0')
		a = a.erase(0, 1);
	if (b[0] == '0')
		b = b.erase(0, 1);

	if (a.size() > b.size())
		b.erase(a.size());
	if (b.size() > a.size())
		a.erase(b.size());

	std::vector<uint64_t> pwr;
	pwr.resize(b.size());
	uint64_t t = 1;
	for (size_t i = 0; i < b.size(); i++) {
		pwr[i] = t * (b[i] - '0' + 1) % MOD;
		t = pwr[i];
	}

	uint64_t res = calc(a.data(), b.data(), a.size(), pwr.data());
	std::cout << res << std::endl;
	//std::string str;
	//while (std::getline(std::cin, str)) {
	//}
}