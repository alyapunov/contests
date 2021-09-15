#include <alya.h>
#include <vector>

using namespace std;

class Solution {
public:
	int maxTurbulenceSize(vector<int>& arr) {
		(void)arr;
		return 0;
	}
};

int main()
{
	{
		Solution sol;
		vector<int> vec = {9,4,2,10,7,8,8,1,9};
		int res = sol.maxTurbulenceSize(vec);
		if (res != 5)
			abort();
	}
	{
		Solution sol;
		vector<int> vec = {4,8,12,16};
		int res = sol.maxTurbulenceSize(vec);
		if (res != 2)
			abort();
	}
	{
		Solution sol;
		vector<int> vec = {100};
		int res = sol.maxTurbulenceSize(vec);
		if (res != 1)
			abort();
	}
}
