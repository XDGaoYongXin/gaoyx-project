#include <iostream>
#include <thread.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/prctl.h>
#include <cstdio>
#include <sys/time.h>

long GetCurTime()
{
	struct timeval val;
	gettimeofday(&val, NULL);
	return val.tv_sec * 1000 + val.tv_usec / 1000;
}

void Thread::PthreadJoin()
{
	if (tid_ != 0)
		pthread_join(tid_, 0);
}

pthread_t Thread::CreateThread(string name)
{
	name_ = name;
	pthread_create(&tid_, 0, Create, this);
	return tid_;
}

void* Thread::Create(void* arg)
{
	Thread* th = static_cast<Thread*>(arg);
	th->Create();
        return 0;
}

void Thread::Create()
{
	prctl(PR_SET_NAME, name_.c_str());
	OnInit();
	OnLoop();
	OnStop();
}

void Thread::OnLoop()
{
	long old_time = GetCurTime();
	long tick_time = old_time;
	while(!flag_)
	{
		LoopNoDelay(); //data service
		long cur_time = GetCurTime();
		if (cur_time - tick_time > 100)
		{
			Loop(cur_time);  //logical service
			tick_time = cur_time;
		}
		if (cur_time - old_time >= 10)
		{
			usleep(0);
			old_time = cur_time;
		}else
			usleep((10 -(cur_time - old_time)) * 1000);
	}
}

void Thread::OnStop()
{
	tid_ = 0;
	flag_ = 0;
	pthread_detach(pthread_self());
}

void Thread::SetStop()
{
	flag_ = 1;
}
