#include <eventloop.h>

EventLoop::EventLoop()
{
	base_ = event_base_new();
}

EventLoop::~EventLoop()
{
	event_base_free(base_);
}

void EventLoop::LoopNoDelay()
{
	event_base_loop(base_, EVLOOP_NONBLOCK);
	SendData();
}
