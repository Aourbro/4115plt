#include <cassert>
#include <algorithm>

using namespace std;

int Gcd(const vector<int>& nums)
{
    assert("Error: gcd has more than 2 args" && nums.size() >= 2);

    int res = nums[0];
    int len = nums.size();
    for (int i = 1; i < len; ++i) {
        res = __gcd(res, nums[i]);
    }

    return res;
}

int Lcm(const vector<int>& nums)
{
    assert("Error: lcm has more than 2 args" && nums.size() >= 2);

    int res = nums[0];
    int len = nums.size();
    for (int i = 1; i < len; ++i) {
        res = res / __gcd(res, nums[i]) * nums[i];
    }

    return res;
}