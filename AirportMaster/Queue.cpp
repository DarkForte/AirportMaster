#include "stdafx.h"
#include "Queue.h"
#include "QNode.h"
#include "Airplane.h"

#include <string>
using namespace std;

CQueue::CQueue(void)
{
	head = tail = NULL;
	cnt = 0;
}


CQueue::~CQueue(void)
{
}

void CQueue::pop()
{
	CQNode *p = head;
	head = head->next;
	delete(p);
	cnt--;
}

bool CQueue::empty()
{
	return head==NULL;
}

void CQueue::push(CAirplane &a)
{
	CQNode *p = new CQNode(a);
	if(empty())
	{
		head = tail = p;
		cnt++;
		return;
	}
	tail->next = p;
	tail=tail->next;
	cnt++;
	return;
}

CAirplane CQueue::front()
{
	return head->data;
}

void CQueue::scan(int alert, CQueue &q, string list[], int &p_list)
{
	CQNode *p;
	CQNode *p_pre=NULL;
	for(p = head; p!=NULL; p=p->next)
	{
		CAirplane plane = p->data;
		plane.fuel -= DFUEL;

		if(plane.fuel <= alert)
		{
			list[++p_list] = plane.id;

			q.push(plane);

			if(p == head)//第一个元素就要删除
			{
				head = head->next;
			}
			else if(p == tail)//队尾指针得移动
			{
				tail = p_pre;
			}
			else
			{
				p_pre->next = p->next;
			}

			cnt--;
			delete(p);
		}

		p_pre = p;
	}
	return;
}

int CQueue::size()
{
	return cnt;
}

void CQueue::Fill(CAirplane list[])
{
	if(empty())
		return;
	int p_list=0;
	CQNode* p = head;
	while(p != tail)
	{
		list[++p_list] = p->data;
		p = p->next;
	}
	list[++p_list] = p->data;
	return;
}