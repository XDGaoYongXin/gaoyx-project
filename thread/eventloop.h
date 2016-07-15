#ifndef __EVENTLOOP_H_
#define __EVENTLOOP_H_
#include <thread.h>
#include <event.h>
class EventLoop: public Thread
{
public:
	EventLoop();
	~EventLoop();
	virtual void LoopNoDelay();
	virtual void SendData(){};
	event_base* base_;
};


#endif
