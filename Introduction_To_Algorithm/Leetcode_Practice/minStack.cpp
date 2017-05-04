#include<iostream>
#include<stack>

using namespace std;

// use two stack
class MinStack {
private:
	stack<int> allS, minS;
public:
	/** initialize your data structure here. */
	MinStack() {

	}

	void push(int x) {
		allS.push(x);
		if (minS.empty())
		{
			minS.push(x);
		}
		else if(x <= minS.top())
		{
			minS.push(x);
		}
	}

	void pop() {
		if (allS.empty()) 
			return;
		if (allS.top() == getMin())
		{
			minS.pop();
		}
		allS.pop();
	}

	int top() {
		if (allS.empty())
			return INT_MIN;
		else
			return allS.top();
	}

	int getMin() {
		if (minS.empty())
			return INT_MIN;
		else
			return minS.top();
	}
};


// use one stack 
class MinStackOne {
private:
	stack<long> allS;
	long minV;
public:
	/** initialize your data structure here. */
	MinStackOne() {

	}

	void push(int x) {
		if (allS.empty())
		{
			allS.push(0L);
			minV = x;
		}
		else
		{
			allS.push(x - minV);
			if (x < minV) minV = x;
		}
	}

	void pop() {
		if (allS.empty()) return;
		long popV = allS.top();
		allS.pop();
		if (popV < 0) minV = minV - popV;
	}

	int top() {
		if (allS.top() > 0)
		{
			return (int)(allS.top() + minV);
		}
		else
		{
			return (int)(minV);
		}
	}

	int getMin() {
		return (int)minV;
	}
};
int main()
{

	MinStackOne* obj = new MinStackOne();
	obj->push(-2);
	obj->push(0);
	obj->push(-3);

	obj->pop();
	int param_3 = obj->top();
	int param_4 = obj->getMin();
	return 0;
}
