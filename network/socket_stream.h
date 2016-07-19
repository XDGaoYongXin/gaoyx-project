#ifndef __SOCKET_STREAM_H_
#define __SOCKET_STREAM_H_

#include <event.h>
#include <map>
#include <cstdio>
#include <socket_msg.h>

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
	SocketStream* Add(unsigned long ip, MessageQueue& queue);
	void Remove(unsigned long ip, int id);
	SocketStream* Get(unsigned long ip, int id);
private:
	std::map<unsigned long, int> ip_addr_;
	std::map<std::pair<unsigned long, int>, SocketStream*> mgr_;
	std::map<std::pair<unsigned long, int>, SocketStream*>::iterator it_;
};

class SocketStream
{
public:
	~SocketStream();
	SocketStream(unsigned long ip, int id, MessageQueue& queue):ip_(ip), id_(id), queue_(queue), is_short_connect_(false), send_length_(0){}
	static void OnReadCb(bufferevent* e, void* arg);
	void OnReadCb(bufferevent* e);
	static void OnWriteCb(bufferevent* e, void* arg);
	void OnWriteCb(bufferevent* e);
	static void OnErrorCb(bufferevent* e, short ev, void* arg);
	void Connected();
	void SetIsShortConnect(bool is_short_connect){is_short_connect_ = is_short_connect;};
	void SetSocketFd(int socket_fd){socket_fd_ = socket_fd;};
	void BufferEventWrite(unsigned char*data, int len);
	bufferevent* buffer_event_;
private:
	unsigned long ip_;
	int id_;
	bool is_short_connect_;
	int send_length_;
	int socket_fd_;
	MessageQueue& queue_;
};

#endif