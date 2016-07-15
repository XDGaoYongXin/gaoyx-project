#ifndef __SOCKET_LISTEN_H_
#define __SOCKET_LISTEN_H_

#include <event.h>
#include <eventloop.h>
class SocketListen: public EventLoop
{
public:
	SocketListen(bool is_short_connnect);
    ~SocketListen();
	bool socket_listen(string ip, int port);
	virtual void OnInit(){}
private:
	static void OnAccept(int fd, short ev, void* arg);
	void OnAccept(int fd);
	event ev_;
	int fd_listen_;
	bool is_short_connnect_;
};



#endif
