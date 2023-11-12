#include <iostream>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <algorithm>

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

	size_t num_tasks;
	std::cin >> num_tasks;

	if (num_tasks % 2) {
		std::cout << "-1\n";
		return 0;
	}

	std::vector<uint32_t> tasks;
	tasks.resize(num_tasks);
	for (size_t i = 0; i < num_tasks; i++)
		std::cin >> tasks[i];

	std::sort(tasks.begin(), tasks.end());
	size_t half = num_tasks / 2;

	uint32_t sum = tasks.front() + tasks.back();
	for (size_t i = 1, j = num_tasks - 2; i < half; i++, j--) {
		if (tasks[i] + tasks[j] != sum) {
			std::cout << "-1\n";
			return 0;
		}
	}

	std::cout << sum << std::endl;


	//std::string str;
	//while (std::getline(std::cin, str)) {
	//}
}