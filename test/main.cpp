#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <sys/prctl.h>
#include <signal.h>
#include "test.h"

Test* test = NULL;
void PRO_STOP(int signo)
{
	std::cout << std::endl;
#ifndef _DEBUG  
	if (test)
		test->SetStop();
#endif
}

int main(int argc, char* argv[])
{
	prctl(PR_SET_NAME, "TEST_HTTP");
	strcpy(argv[0], "TEST_HTTP");
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, PRO_STOP);
	test = new Test();
	test->CreateThread("TEST_HTTP");
	test->PthreadJoin();
	delete test;
}
