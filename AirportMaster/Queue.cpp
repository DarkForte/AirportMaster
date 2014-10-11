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

void CQueue::scan(int alert, CAirplane list[], int &p_list)
{
	CQNode *p = head;
	CQNode *p_pre=NULL;
	while(p!=NULL)
	{
		p->data.fuel -= DFUEL;
		CAirplane plane = p->data;
		
		if(plane.fuel <= alert)
		{
			list[++p_list] = plane;

			if(p == head)//第一个元素就要删除
			{
				head = head->next;
				delete(p);
				p = head;
			}
			else if(p == tail)//队尾指针得移动
			{
				p_pre->next = NULL;
				tail = p_pre;
				delete(p);
				p=NULL;
			}
			else
			{
				p_pre->next = p->next;
				delete(p);
				p = p_pre->next;
			}
			cnt--;
		}
		else
		{
			p_pre = p;
			p=p->next;
		}
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
	while(p != NULL)
	{
		list[++p_list] = p->data;
		p = p->next;
	}
	return;
}