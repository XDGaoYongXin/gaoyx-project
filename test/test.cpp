#include "test.h"

Test::~Test()
{
}

void Test::OnInit()
{
	sl_.socket_listen("0.0.0.0", 8080);
}

void Test::SetStop()
{
	sl_.SetStop();
	EventLoop::SetStop();
}

void Test::Loop(long cur_time)
{
	std::vector<Message*>* queue = queue_.GetMessage();
	for (int i = 0; i < (*queue).size(); ++i)
	{
		Message* msg = (*queue)[i];
		if (!msg)
			continue;
		MsgHeader header;
		msg->WriteOut(header);
		OndispatchMsg(msg, header.ip, header.id);
		delete (*queue)[i];
	}
	(*queue).clear();
}

void Test::OndispatchMsg(Message* msg, unsigned long ip, int id)
{
	bool space_appear = false;
	unsigned char* begin = NULL;
	unsigned char* end = NULL;
	for (int i = 0; i < msg->Len(); ++i)
	{
		unsigned char* c = msg->Data() + i;
		if (*c == ' ' && begin != NULL)
		{
			end = c;
			break;
		}
		if (*c == ' ')
		{
			space_appear = true;
		}else if (space_appear && begin == NULL)
		{
			begin = c;
		}
	}
	if (begin == NULL || end == NULL)
	{
		Send404(ip, id);
		return ;
	}
	std::string infomation;
	infomation.insert(infomation.begin(), begin, end);
	//std::cout << infomation << std::endl;
	int pos = infomation.find('?');
	if (pos == string::npos)
	{
		Send404(ip, id);
		return;
	}
	string path;
	path.insert(path.begin(), &infomation[0], &infomation[0] + pos);
	infomation.erase(0, pos + 1);
	//std::cout << "path = " << path << " infomation = " << infomation <<std::endl;
	std::map<string,string> params;
	GetParams(params, infomation);
	std::map<string,string>::iterator it = params.find("code");
	if (it == params.end())
	{
		Send404(ip, id);
		return ;
	}
	string file = "." + path + "/" + it->second;
	FILE* fp = fopen(file.c_str(), "r");
	if (fp == NULL)
	{
		Send404(ip, id);
		return ;
	}
	SendMsg(file, ip, id);
	fclose(fp);

}

void Test::SendMsg(string& path, unsigned long ip, int id)
{
	
	MsgHeader header;
	header.ip = ip;
	header.id = id;
	Message* msg  = new (std::nothrow) Message();
	if (!msg)
		return ;
	bool check = msg->WriteIn((unsigned char*)(&header), sizeof(header));
	if (!check)
		return ;

	FILE* fp = fopen(path.c_str(), "r");
	if (!fp)
		return ;
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char mmsg[10];
	sprintf(mmsg, "%d", len);
	std::string http_header = "HTTP/1.1 200 OK\r\n";
	if (path.find(".htm") != string::npos)
		http_header += "Content-Type: text/html; charset=UTF-8\r\n";
	else
		http_header += "Content-Type: text/plain; charset=UTF-8\r\n";
	http_header += "Content-Length: " + std::string(mmsg) + "\r\n";
	http_header += "Connection: close\r\n";
	http_header += "\r\n";
	check = msg->WriteIn(reinterpret_cast<const unsigned char*>(http_header.c_str()), http_header.size());
	if (!check)
	{
		delete msg;
		return ;
	}

	unsigned char* file = new(std::nothrow) unsigned char[len];
	if (!file)
	{
		return ;
	}
	fread(file, len ,1, fp);
	fclose(fp);
	check = msg->WriteIn(file, len);
	if (!check)
	{
		delete msg;
		return ;
	}
	sl_.SendMsg(msg);
}

void Test::Send404(unsigned long ip, int id)
{
	MsgHeader header;
	header.ip = ip;
	header.id = id;
	Message* msg  = new (std::nothrow) Message();
	if (!msg)
	{
		delete msg;
		return ;
	}

	bool check = msg->WriteIn((unsigned char*)(&header), sizeof(header));
	std::string http_header = "HTTP/1.1 301 Not Found\r\n";
	http_header += "Server: gaoyx\r\n";
	http_header += "Connection: keep-alive\r\n";
	http_header += "Location: /conf?code=404.html\r\n";
	http_header += "Content-Length: 0\r\n";
	http_header += "\r\n";
    check = msg->WriteIn(reinterpret_cast<const unsigned char*>(http_header.c_str()), http_header.size());
	if (!check)
	{
		delete msg;
		return ;
	}
	sl_.SendMsg(msg);
}

void Test::GetParams(std::map<string, string>& params, string& infomation)
{
	int pos;
	while((pos = infomation.find('&')) != string::npos)
	{
		string msg;
		msg.insert(msg.begin(), &infomation[0], &infomation[0] + pos);
		int pos1;
		if ((pos1 = msg.find('=')) != string::npos)
		{
			string param1 = string(&msg[0], &msg[0] + pos1);
			string param2 = msg.erase(0, pos1 + 1);
			params.insert(std::make_pair(param1, param2));
		}
		infomation.erase(0, pos + 1);
	}
	if ((pos = infomation.find('=')) != string::npos)
	{
		string param1 = string(&infomation[0], &infomation[0] + pos);
		string param2 = infomation.erase(0, pos + 1);
		params.insert(std::make_pair(param1, param2));
	}
}
