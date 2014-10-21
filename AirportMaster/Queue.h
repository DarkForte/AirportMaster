#pragma once
#include "Airplane.h"
#include "QNode.h"
#include <string>
using namespace std;

class CQueue
{
public:
	CQNode *head, *tail;
	int cnt;
public:
	CQueue(void);
	~CQueue(void);

	void pop();
	void push(CAirplane &a);
	CAirplane front();
	void scan(int alert, CAirplane list[], int &p_list);
	void drop();
	bool empty();
	int size();

	void Fill(CAirplane list[]);
};

