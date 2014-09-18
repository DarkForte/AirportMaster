#pragma once
#include "Airplane.h"
#include "QNode.h"

class CQueue
{
protected:
	CQNode *head, *tail;
public:
	CQueue(void);
	~CQueue(void);

	void pop();
	void push(CAirplane &a);
	CAirplane front();
	void scan(int alert, CQueue &q);
	bool empty();
};

