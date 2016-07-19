#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <socket_stream.h>
using namespace std;

SocketStreamMgr::~SocketStreamMgr()
{
	for (it_ = mgr_.begin(); it_ != mgr_.end(); ++it_)
	{
		delete it_->second;
	}
	mgr_.clear();
}

SocketStream* SocketStreamMgr::Add(unsigned long ip, MessageQueue& queue)
{
	int& id = ip_addr_[1];
	id += 1;
	SocketStream* ss = new SocketStream(ip, id, queue);
	mgr_[std::make_pair(ip, id)] = ss;
	//std::cout << __func__ << " mgr_.size = " << mgr_.size() <<std::endl;
	return ss;
}

void SocketStreamMgr::Remove(unsigned long ip, int id)
{
	delete mgr_[std::make_pair(ip, id)];
	mgr_.erase(std::make_pair(ip, id));
}

SocketStream* SocketStreamMgr::Get(unsigned long ip, int id)
{
	it_ = mgr_.find(std::make_pair(ip, id));
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
	unsigned char* pl = evbuffer_pullup(eb, len);
	MsgHeader msg_header;
	msg_header.ip = ip_;
	msg_header.id = id_;
	Message* msg = new Message();
	msg->WriteIn((unsigned char *)(&msg_header), sizeof(msg_header));
	msg->WriteIn(pl, len);
	queue_.AddMessage(msg);
	evbuffer_drain(eb, len);
}

void SocketStream::OnWriteCb(bufferevent* e, void* arg)
{
	SocketStream* ss = static_cast<SocketStream*>(arg);
	ss->OnWriteCb(e);
}

void SocketStream::OnWriteCb(bufferevent* e)
{
}

void SocketStream::OnErrorCb(bufferevent* e, short ev, void* arg)
{
	SocketStream* ss  = static_cast<SocketStream*>(arg);
	SocketStreamMgr::Instance().Remove(ss->ip_, ss->id_);
	//std::cout << __func__ << std::endl;
}


void SocketStream::Connected()
{
}

void SocketStream::BufferEventWrite(unsigned char*data, int len)
{
	bufferevent_write(buffer_event_, data, len);
}
