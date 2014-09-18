#pragma once

#include "Airplane.h"
using namespace std;

class CLane
{
public:
	CAirplane now_plane;
	int leave_time;
	bool idle;

	CLane(void);
	~CLane(void);

	void assign(CAirplane &plane, int leave_t);
	void clear();
	bool empty();
};

