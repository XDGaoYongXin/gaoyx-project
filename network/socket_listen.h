#ifndef __SOCKET_LISTEN_H_
#define __SOCKET_LISTEN_H_

#include <event.h>
#include <eventloop.h>
#include <socket_msg.h>
class SocketListen: public EventLoop
{
public:
	SocketListen(bool is_short_connnect, MessageQueue& queue);
    ~SocketListen();
	bool socket_listen(string ip, int port);
	virtual void OnInit(){}
	virtual void Loop(long cur_time);
	void SendMsg(Message* msg);
private:
	static void OnAccept(int fd, short ev, void* arg);
	void OnAccept(int fd);
	event ev_;
	int fd_listen_;
	bool is_short_connnect_;
	MessageQueue& queue_;
	MessageQueue send_queue_;
};



#endif
