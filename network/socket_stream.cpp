#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <socket_stream.h>

SocketStreamMgr::~SocketStreamMgr()
{
	for (it_ = mgr_.begin(); it_ != mgr_.end(); ++it_)
	{
		delete it_->second;
	}
	mgr_.clear();
}

SocketStream* SocketStreamMgr::Add(int fd)
{
	SocketStream* ss = new SocketStream(fd);
	mgr_[fd] = ss;
	std::cout << __func__ << " mgr_.size = " << mgr_.size() <<std::endl;
	return ss;
}

void SocketStreamMgr::Remove(int fd)
{
	delete mgr_[fd];
	mgr_.erase(fd);
}

SocketStream* SocketStreamMgr::Get(int fd)
{
	it_ = mgr_.find(fd);
	if (it_ != mgr_.end())
		return it_->second;
	return NULL;
}

SocketStream::~SocketStream()
{
	if (buffer_event_)
		bufferevent_free(buffer_event_);
	buffer_event_ = NULL;
	close(socket_fd_);
}

void SocketStream::OnReadCb(bufferevent* e, void* arg)
{
	SocketStream* ss = static_cast<SocketStream*>(arg);
	ss->OnReadCb(e);
}

void SocketStream::OnReadCb( bufferevent* e)
{
	struct evbuffer* eb = e->input;
	if (!eb)
		return ;
	int len = evbuffer_get_length(eb);
	std::string infomation;
	unsigned char* pl = evbuffer_pullup(eb, len);
	infomation.assign(pl, pl + len);
	std::cout << infomation << std::endl;
	evbuffer_drain(eb, len);
	std::string msg = "I'm gaoyongxin I love you";
	char mmsg[10];
	sprintf(mmsg, "%d", msg.size());
	std::string str = "HTTP/1.1 200 OK\r\n";
	str += "Content-Type: text/plain; charset=UTF-8\r\n";
	str += "Connection: Close\r\n";
	//str += "Content-Length: 14\r\n";
	str += "Content-Length: " + std::string(mmsg) + "\r\n";
	str += "\r\n";
	str += "I'm gaoyongxin I love you";
	bufferevent_write(buffer_event_, &str[0], str.size());
	send_length_ += str.size(); 

}

void SocketStream::OnWriteCb(bufferevent* e, void* arg)
{
	SocketStream* ss = static_cast<SocketStream*>(arg);
	ss->OnWriteCb(e);
}

void SocketStream::OnWriteCb(bufferevent* e)
{
	struct evbuffer* eb = e->input;
	if (!eb)
		return ;
	int len = evbuffer_get_length(eb);
	std::string infomation;
	unsigned char* pl = evbuffer_pullup(eb, len);
	infomation.assign(pl, pl + len);
	std::cout << __func__<<" "<<infomation << std::endl;
	//OnErrorCb(e, 0,this);
}

void SocketStream::OnErrorCb(bufferevent* e, short ev, void* arg)
{
	SocketStream* ss  = static_cast<SocketStream*>(arg);
	SocketStreamMgr::Instance().Remove(ss->socket_fd_);
	std::cout << __func__ << std::endl;
}


void SocketStream::Connected()
{
	//std::string str = "HTTP/1.1 200 OK\r\n";
	//str += "Content-Type: text/plain; charset=UTF-8\r\n";
	//str += "Connection: Close\r\n";
	//str += "\r\n";
	//str += "I'm gaoyongxin I love you";
	//std::cout<< __func__ <<std::endl;
	//bufferevent_write(buffer_event_, &str[0], str.size());
}
