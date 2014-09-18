#include "stdafx.h"
#include "QNode.h"
#include "Airplane.h"

CQNode::CQNode(void)
{
}


CQNode::~CQNode(void)
{
}

CQNode::CQNode(CAirplane &d)
{
    data = d;
	next = NULL;
}