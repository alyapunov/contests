#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>

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

	uint32_t n, m;
	std::cin >> n >> m;
	if (n > m)
		std::swap(n, m);
	size_t num_digits = log10(m - 1e-5) + 1;
	uint32_t digits[6] = {};
	uint32_t p[6] = {};
	p[num_digits - 1] = 1;
	for (size_t j = 1; j < num_digits; j++)
		p[num_digits - 1 - j] = p[num_digits - j] * 10;

	uint32_t inv = 0;
	uint32_t res = 0;
	for (uint32_t i = 0; i < n; i++) {
		if (inv < m)
			res++;
		for (size_t j = 0; ; j++) {
			digits[j]++;
			inv += p[j];
			if (digits[j] < 10)
				break;
			inv -= p[j] * 10;
			digits[j] = 0;
		}
	}
	std::cout << res << std::endl;

	//std::string str;
	//while (std::getline(std::cin, str)) {
	//}
}