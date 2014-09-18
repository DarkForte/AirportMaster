#include "stdafx.h"
#include "Airplane.h"
#include <string.h>
using namespace std;


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
}