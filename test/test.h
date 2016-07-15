#ifndef __TEST_H_
#define __TEST_H_

#include <eventloop.h>
#include <socket_listen.h>
class Test:public EventLoop
{
public:
	Test():sl_(false){}
	virtual void OnInit();
	virtual void SetStop();
	virtual void Loop(long cur_time);
	~Test();
	SocketListen sl_;
};



#endif
