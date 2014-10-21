#include "stdafx.h"
#include "Airplane.h"
#include <string.h>
using namespace std;

int CAirplane::now_color =1;

CAirplane::CAirplane(void)
{
}


CAirplane::~CAirplane(void)
{
}

CAirplane::CAirplane(string _id, bool _a, int _t, int _f)
{
		id = _id;
		arrive = _a;
		time = _t;
		fuel = _f;
		color_num = now_color;
		now_color = now_color % PLANE_ICON_NUM +1;
}