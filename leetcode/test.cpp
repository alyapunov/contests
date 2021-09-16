#include <alya.h>
#include <vector>

using namespace std;

//  Definition for singly-linked list.
struct ListNode {
	int val;
	ListNode *next;
};


class Solution {
public:
	ListNode* reverseKGroup(ListNode* head, int k) {
	}
};

void
check(std::initializer_list<int> orig, int k, std::initializer_list<int> res)
{
	std::vector<ListNode> nodes(orig.size());
	{
		size_t i = 0;
		for (int val : orig) {
			auto &n = nodes[i++];
			n.val = val;
			n.next = i == orig.size() ? nullptr : &nodes[i];
		}
	}
	Solution sol;
	ListNode* r = sol.reverseKGroup(&nodes[0], k);
	{
		size_t i = 0;
		for (int val : res) {
			if (val != r->val)
				abort();
			r = r->next;
		}
		if (r != nullptr)
			abort();
	}
}

int main()
{
	check({1,2,3,4,5}, 2, {2,1,4,3,5});
	check({1,2,3,4,5}, 3, {1,2,3,4,5});
	check({1,2,3,4,5}, 1, {1,2,3,4,5});
	check({1}, 1, {1});
}
