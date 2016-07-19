#ifndef __SOCKET_MSG_H_
#define __SOCKET_MSG_H_
#include <vector>
#include <pthread.h>
#include <cstring>
#include <iostream>

#pragma pack(push)
#pragma pack(1) 
struct MsgHeader
{
	unsigned long ip;
	int id;
};

#pragma pack(pop)

class Message
{
public:
	Message():msg_(NULL), len_(0), read_len_(0), capacity_(0){}
	~Message(){delete msg_; msg_ = NULL;}
	bool CreateCapacity(int len);
	bool WriteIn(const unsigned char* data, int len);
	template <typename T>
	bool WriteOut(T& msg)
	{
		int len = sizeof(T);
		if (read_len_ + len > len_)
			return false;
		memcpy(&msg, msg_ + read_len_, len);
		read_len_ += len;
		return true;
	}
	unsigned char* Data(){return msg_ + read_len_;};
	int Len(){return len_ - read_len_;};
	unsigned char* msg_;
	int len_;
	int capacity_;
	int read_len_;
};

class Lock
{
public:
	Lock(pthread_mutex_t* lock):lock_(lock){pthread_mutex_lock(lock);}
	~Lock(){pthread_mutex_unlock(lock_);}
	pthread_mutex_t* lock_;
};

class MessageQueue
{
public:
	MessageQueue():flag_(0){pthread_mutex_init(&lock_, 0);}
	~MessageQueue(){pthread_mutex_destroy(&lock_);}
	std::vector<Message*>* GetMessage();
	bool AddMessage(Message* message);
	int flag_;
	std::vector<Message*> message_[2];
	pthread_mutex_t lock_;
	
};
#endif