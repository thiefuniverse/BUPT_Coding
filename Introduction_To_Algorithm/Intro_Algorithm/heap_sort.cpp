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

void max_heapify(vector<int> & arr, int flag,int arrSize)
{
	int left = flag * 2 - 1;
	int right = flag * 2;
	int largest = 0;
	if (left<arrSize && arr[left]>arr[flag - 1])
	{
		largest = left;
	}
	else
		largest = flag - 1;
	if (right<arrSize && arr[right]>arr[largest])
	{
		largest = right;
	}
	if (largest != flag - 1)
	{
		swap(arr[largest], arr[flag - 1]);
		max_heapify(arr, largest + 1,arrSize);
	}

}

void min_heapify(vector<int>  & arr, int flag,int arrSize)
{
	int left = flag * 2 - 1;
	int right = flag * 2;
	int least = 0;
	if (left < arrSize&& arr[left] < arr[flag - 1])
		least = left;
	else
		least = flag - 1;
	if (right < arrSize && arr[right] < arr[least])
		least = right;

	if (least != flag - 1)
	{
		swap(arr[least], arr[flag - 1]);
		min_heapify(arr, least + 1,arrSize);
	}
}

void buildMaxHeap(vector<int>& arr,int arrSize)
{
	int  aLen = arrSize-1;
	int halfLen = aLen / 2;
	for (int i = halfLen; i >= 0; i--)
	{
		max_heapify(arr, i + 1, arrSize);
	}
}

void buildMinHeap(vector<int>& arr,int arrSize)
{
	int arrLen = arrSize - 1;
	int halfLen = arrLen / 2;
	for (int i = halfLen; i >= 0; i--)
	{
		min_heapify(arr, i+1, arrSize);
	}
}

// isDesc  :    true for desc order     false for asc order
void heapSort(vector<int>& arr,bool isDesc)
{
	vector<int> res;
	if(isDesc)
		buildMinHeap(arr, arr.size());
	else
		buildMaxHeap(arr,arr.size());

	for (int i = arr.size()-1; i >= 1; i--)
	{
		swap(arr[0], arr[i]);
		if (isDesc)
			min_heapify(arr, 1, i);
		else
			max_heapify(arr,1, i);
	}
}

int main()
{
	cout << isMaxHeap(vector<int>{23, 17, 14, 6, 13, 10, 1, 5, 4, 99});
	vector<int> a{ 6,7,3,8,9 };
	min_heapify(a, 1, a.size());

	vector<int> b{ 4,1,3,2,16,9,10,14,8,7 };
	//buildMaxHeap(b,b.size());
	//for (size_t i = 0; i < b.size(); i++)
	//{
	//	cout << b[i]<<" ";
	//}
	//cout << "\n";
	//buildMinHeap(b,b.size());
	//for (size_t i = 0; i < b.size(); i++)
	//{
	//	cout << b[i] << " ";
	//}
	cout << "\n";
	heapSort(b, true);
	for (size_t i = 0; i < b.size(); i++)
	{
		cout << b[i] << " ";
	}
	cout << "\n";
	heapSort(b, false);
	for (size_t i = 0; i < b.size(); i++)
	{
		cout << b[i] << " ";
	}
	//for (size_t i = 0; i < a.size(); i++)
	//{
	//	cout << a[i] << " ";
	//}
	getchar();
	getchar();
	return 0;
}


