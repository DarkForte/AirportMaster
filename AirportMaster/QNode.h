#pragma once
#include "Airplane.h"
class CQNode
{
public:
	CAirplane data;
	CQNode *next;

public:
	CQNode(void);
	~CQNode(void);

	CQNode(CAirplane &d);
};

