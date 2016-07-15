#include "test.h"
#include <iostream>

Test::~Test()
{
}

void Test::OnInit()
{
	sl_.socket_listen("0.0.0.0", 8080);
}

void Test::SetStop()
{
	sl_.SetStop();
	EventLoop::SetStop();
}

void Test::Loop(long cur_time)
{
	;
}
