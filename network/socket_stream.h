#ifndef __SOCKET_STREAM_H_
#define __SOCKET_STREAM_H_

#include <event.h>
#include <map>
#include <cstdio>
class SocketStream;

class SocketStreamMgr
{
public:
	~SocketStreamMgr();
	static SocketStreamMgr& Instance()
	{
		static SocketStreamMgr ssm;
		return ssm;
	}
	SocketStream* Add(int fd);
	void Remove(int fd);
	SocketStream* Get(int fd);
private:
	std::map<int, SocketStream*> mgr_;
	std::map<int, SocketStream*>::iterator it_;
};

class SocketStream
{
public:
	~SocketStream();
	SocketStream(int fd):socket_fd_(fd), is_short_connect_(false), send_length_(0){}
	static void OnReadCb(bufferevent* e, void* arg);
	void OnReadCb(bufferevent* e);
	static void OnWriteCb(bufferevent* e, void* arg);
	void OnWriteCb(bufferevent* e);
	static void OnErrorCb(bufferevent* e, short ev, void* arg);
	void Connected();
	void SetIsShortConnect(bool is_short_connect){is_short_connect_ = is_short_connect;};
	bufferevent* buffer_event_;
private:
	int socket_fd_;
	bool is_short_connect_;
	int send_length_;
};

#endif