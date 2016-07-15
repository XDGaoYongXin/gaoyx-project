#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <socket_listen.h>
#include <socket_stream.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

SocketListen::SocketListen(bool is_short_connect):fd_listen_(-1)
{
	is_short_connnect_ = is_short_connect;
}

SocketListen::~SocketListen()
{
	if (fd_listen_ != INVALID_SOCKET && fd_listen_ != SOCKET_ERROR)
	{
		close(fd_listen_);
	}
}

bool SocketListen::socket_listen(string ip, int port)
{
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_ANY;

	int fd = socket(AF_INET, SOCK_STREAM, 0);
    fd_listen_ = fd;
	if (fd == INVALID_SOCKET || fd == SOCKET_ERROR)
	{
		std::cout << __func__ << " create socket error" << std::endl;
		return false;
	}

	int reuseaddr = 1; 
	int key = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseaddr, sizeof(reuseaddr));
	if (key != 0)
	{
		std::cout << __func__<< " setsockopt error errno = "<< errno << " key = "<< key <<std::endl;
		return false;
	}

	if ( bind(fd, (struct sockaddr*)&sin, sizeof(sin) ) != 0)
	{
		std::cout << __func__<< " bind error errno = "<< errno << std::endl;
		return false;
	}
	if (listen(fd, 65535) != 0)
	{
		std::cout << __func__<< " listen error errno = "<< errno << std::endl;
		return false;
	}
	evutil_make_socket_nonblocking(fd);
	event_set(&ev_, fd, EV_READ | EV_PERSIST, OnAccept, this);
	event_base_set(base_, &ev_);
	event_add(&ev_, 0);
	char info[100];
	sprintf(info, "Listen Port %d", port);
	CreateThread(info);
}

void SocketListen::OnAccept(int fd, short ev, void* arg)
{
	SocketListen* sl = static_cast<SocketListen*>(arg);
	sl->OnAccept(fd);
}

void SocketListen::OnAccept(int fd)
{
	struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
	int socket_fd = accept(fd, (struct sockaddr*)&sin, &len);
	string ip = inet_ntoa(sin.sin_addr);
	std::cout << __func__ <<" "<<ip<<" "<<"port: "<<sin.sin_port <<" socket_fd = "<<socket_fd<< std::endl;
	if (socket_fd == INVALID_SOCKET || socket_fd == SOCKET_ERROR)
	{
		std::cout << __func__<< " listen error errno = "<< errno << std::endl;
		return ;
	}
	evutil_make_socket_nonblocking(socket_fd);

	SocketStream* ss = SocketStreamMgr::Instance().Add(socket_fd);
	if (ss == NULL)
	{
		SocketStreamMgr::Instance().Remove(socket_fd);
		return ;
	}
	ss->SetIsShortConnect(is_short_connnect_);
	ss->buffer_event_ = bufferevent_new(socket_fd, SocketStream::OnReadCb, SocketStream::OnWriteCb, SocketStream::OnErrorCb,ss);
	bufferevent_base_set(base_, ss->buffer_event_);
	bufferevent_enable(ss->buffer_event_, EV_READ | EV_WRITE);
	ss->Connected();
}
