#include<iostream>
#include<queue>
#include<vector>

using namespace std;

bool isMaxHeap(vector<int>& arr)
{
	vector<int> aLevel;
	int levelNum = 1, flag = 0;

	if (!arr.empty())
	{
		while (flag < arr.size())
		{
			int endFlag = flag + levelNum;
			while (flag < endFlag&&flag<arr.size())
			{
				aLevel.push_back(arr[flag]);
				flag++;
			}

			int tempFlag = flag;
			while (!aLevel.empty() && flag < arr.size())
			{
				int temp = aLevel[0];
				aLevel.erase(aLevel.begin());
				if (flag < arr.size() && arr[flag] < temp)
					flag++;
				else
					return false;

				if (flag == arr.size())
					return true;
				if (flag < arr.size() && arr[flag] < temp)
					flag++;
				else
					return false;
			}
			flag = tempFlag;
			levelNum *= 2;
		}
	}
	return true;
}

void max_heapify(vector<int> & arr, int flag)
{
	int left = flag * 2 - 1;
	int right = flag * 2;
	int largest = 0;
	if (left<arr.size() && arr[left]>arr[flag - 1])
	{
		largest = left;
	}
	else
		largest = flag - 1;
	if (right<arr.size() && arr[right]>arr[flag - 1])
	{
		largest = right;
	}
	if (largest != flag - 1)
	{
		swap(arr[largest], arr[flag - 1]);
		max_heapify(arr, largest + 1);
	}

}

void min_heapify(vector<int>  & arr, int flag)
{
	int left = flag * 2 - 1;
	int right = flag * 2;
	int least = 0;
	if (left < arr.size() && arr[left] < arr[flag - 1])
		least = left;
	else
		least = flag - 1;
	if (left < arr.size() && arr[right] < arr[least])
		least = right;

	if (least != flag - 1)
	{
		swap(arr[least], arr[flag - 1]);
		min_heapify(arr, least + 1);
	}
}

int main()
{
	cout << isMaxHeap(vector<int>{23, 17, 14, 6, 13, 10, 1, 5, 4, 99});
	vector<int> a{ 6,7,3,8,9 };
	min_heapify(a, 1);
	for (size_t i = 0; i < a.size(); i++)
	{
		cout << a[i] << " ";
	}
	getchar();
	getchar();
	return 0;
}


