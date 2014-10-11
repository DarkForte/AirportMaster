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
	int color_num;
	static int now_color;
public:
	CAirplane(void);
	~CAirplane(void);

	CAirplane(string _id, bool _a, int _t, int _f);
};

