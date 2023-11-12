#include <cstddef>
#include <cstdint>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>

struct WorkingMember {
	std::string name;
	int64_t exp_ref;
	bool operator<(const WorkingMember& that) const
	{
		return std::tie(exp_ref, name) <
		       std::tie(that.exp_ref, that.name);
	}
};

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

	size_t n;
	int64_t t = 0;
	int64_t total_exp = 0;
	std::unordered_map<std::string, int64_t> prev_exp;
	std::unordered_map<std::string, int64_t> cur_team;
	std::set<WorkingMember> team_by_exp;

	std::cin >> n;
	for (size_t i = 0; i < n; i++) {
		std::string name;
		int64_t next_t;
		std::cin >> name >> next_t;

		total_exp += cur_team.size() * (next_t - t);
		t = next_t;

		auto itrc = cur_team.find(name);
		if (itrc == cur_team.end()) {
			auto itrp = prev_exp.find(name);
			int64_t exp = 0;
			if (itrp != prev_exp.end()) {
				exp = itrp->second;
				prev_exp.erase(itrp);
			}
			total_exp += exp;
			int64_t exp_ref = t - exp;
			cur_team.emplace(name, exp_ref);
			team_by_exp.insert(WorkingMember{std::move(name), exp_ref});
		} else {
			int64_t exp_ref = itrc->second;
			cur_team.erase(itrc);
			prev_exp.emplace(name, t - exp_ref);
			total_exp -= t - exp_ref;
			team_by_exp.erase(WorkingMember{std::move(name), exp_ref});
		}

		const WorkingMember& m = *team_by_exp.begin();
		int64_t exp = t - m.exp_ref;
		std::cout << m.name << " " << total_exp - 2 * exp << "\n";
	}

	//std::string str;
	//while (std::getline(std::cin, str)) {
	//}
}