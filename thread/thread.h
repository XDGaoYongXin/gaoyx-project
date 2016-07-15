#ifndef __THREAD_H_
#define __THREAD_H_
#include <pthread.h>
#include <unistd.h>
#include <string>
using namespace std;
class Thread
{
public:
	Thread():flag_(0){}
	pthread_t CreateThread(string name);
	static void* Create(void* arg);
	void Create();
	void PthreadJoin();
	virtual void OnInit() = 0;
	virtual void LoopNoDelay() = 0;
	virtual void Loop(long cur_time){};
	void OnLoop();
	void OnStop();
	virtual void SetStop();
private:
	pthread_t tid_;
	string name_;
	bool flag_;
};

#endif
