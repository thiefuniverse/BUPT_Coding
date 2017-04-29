#include<iostream>
#include<vector>

using namespace std;


// left -->    mid    <--  right
int quickPartition(vector<int>&arr, int left, int right)
{
	int tmp = arr[left];
	while (left < right)
	{
		while (left<right&&arr[right]>=tmp) right--;
		arr[left] = arr[right];

		while (left < right&&arr[left] <= tmp)left++;
		arr[right] = arr[left];
	}
	arr[left] = tmp;
	return left;
}

// left    -> right 
int quickPartitionSingle(vector<int>&arr, int left, int right)
{
	int low = left - 1;
	int high = left;
	int flag = arr[right];
	while (high <= right)
	{
		if (arr[high] <=flag)
		{
			low++;
			swap(arr[low], arr[high]);
		}
		high++;
	}
	swap(arr[low + 1], arr[right]);
	return low;
}

void quickSortTwo(vector<int>& arr, int left, int right)
{
	if (left < right)
	{
		int mid = quickPartitionSingle(arr, left, right);
		quickSortTwo(arr, left, mid - 1);
		quickSortTwo(arr, mid + 1,right);
	}
}
int main()
{
	vector<int> th{ 4,6,2,8,3,7 };
	quickSortTwo(th, 0, th.size()-1);
	for (size_t i = 0; i < th.size(); i++)
	{
		cout << th[i] << "  ";
	}
	getchar();
	getchar();

	return 0;
}
