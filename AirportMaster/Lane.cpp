#include "stdafx.h"
#include "Lane.h"


CLane::CLane(void)
{
	idle = true;
	leave_time = INF;
}


CLane::~CLane(void)
{
}

void CLane::assign(CAirplane &plane, int leave_t)
{
	now_plane = plane;
	leave_time = leave_t;
	idle = false;
}

void CLane::clear()
{
	leave_time = INF;
	idle = true;
}

bool CLane:: empty()
{
	return idle;
}
