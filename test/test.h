#ifndef __TEST_H_
#define __TEST_H_

#include <eventloop.h>
#include <socket_listen.h>
#include <socket_msg.h>
#include <map>
#include <iostream>

class Test:public EventLoop
{
public:
	Test():sl_(false, queue_){}
	~Test();
	virtual void OnInit();
	virtual void SetStop();
	virtual void Loop(long cur_time);
private:
	void OndispatchMsg(Message* msg, unsigned long ip, int id);
	void GetParams(std::map<string, string>& params, string& infomation);
	void SendMsg(string& path, unsigned long ip, int id);
	void Send404(unsigned long ip, int id);
	MessageQueue queue_;
	SocketListen sl_;
};



#endif
