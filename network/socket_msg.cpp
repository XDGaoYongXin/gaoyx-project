#include <socket_msg.h>

bool Message::CreateCapacity(int len)
{
	int need_capacity = len + len_;
	if (capacity_ >= need_capacity)
		return true;
	int capacity = 1;
	while(capacity < need_capacity)
		capacity *= 2;
	if (capacity < 0)
		return false;
	unsigned char* pt = new (std::nothrow) unsigned char[capacity];
	if (pt == NULL)
		return false;
	memcpy(pt, msg_, len_);
	delete msg_;
	msg_ = pt;
	capacity_ = capacity;
	return true;
}

bool Message::WriteIn(const unsigned char* data, int len)
{
	if (!data || !CreateCapacity(len))
		return false;
	memcpy(msg_ + len_, data, len);
	len_ += len;
	return true;
}



std::vector<Message*>* MessageQueue::GetMessage()
{
	int flag = 0;
	{
		Lock lock(&lock_);
		flag = flag_;
		flag_ ^= 1;
	}
	return &message_[flag];
}

bool MessageQueue::AddMessage(Message* message)
{
	Lock lock(&lock_);
	message_[flag_].push_back(message);
	return true;
}