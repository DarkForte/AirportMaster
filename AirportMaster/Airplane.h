#pragma once
#include <string>
using namespace std;

class CAirplane
{
public:
	string id;
	bool arrive;
	int time;
	int fuel;
public:
	CAirplane(void);
	~CAirplane(void);

	CAirplane(string _id, bool _a, int _t, int _f);
};

