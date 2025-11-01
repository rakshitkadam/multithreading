
#include <bits/stdc++.h>

using namespace std;


class Queues 
{
	public: 
	
	int capac;
	int fa,ra;
	int fb, rb;
	int freeHead;
	struct Node 
	{
		int value;
		int next;
	};
	vector<Node>buffer;

	Queues(int limit) 
	{
		capac = limit;
		fa = ra = -1;
		fb = rb = -1;
		freeHead = 0;
		buffer.resize(capac);
		for(int i=0;i<capac-1;i++) 
		{
			buffer[i].next = i+1;
		}
		buffer[capac-1].next = -1;
	}
	
	int getNode() 
	{
		if(freeHead == -1) 
		{
			return -1;
		}
		int freeNode = freeHead;
		freeHead = buffer[freeNode].next;
		buffer[freeNode].next = -1;
		return freeNode;
	}
	
	void releaseNode(int ind) 
	{
		buffer[ind].next = freeHead;
		freeHead = ind;
	}
	
	bool enqueue1(int val) 
	{
		int nextNode = getNode();
		if(nextNode == -1) return false;
		buffer[nextNode].value = val;
		if(fa == -1)
		{
			fa = nextNode;
			ra = nextNode;
		} else 
		{
			buffer[ra].next = nextNode;
			ra = nextNode;
		}
		return true;
	}
	
	int dequeue1() 
	{
		if(fa == -1) 
		{
			return -1;
		}
		int retValue = buffer[fa].value;
		int previousFront = fa;
		fa = buffer[fa].next;
		if(fa == -1) ra = -1;
		releaseNode(previousFront);
		return retValue;
	}
};


int main() 
{
	return 0;
}