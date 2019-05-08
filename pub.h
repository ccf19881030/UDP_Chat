#ifndef _PUB_H_
#define _PUB_H_
 
#include <list>
#include <algorithm>
 
using namespace std;
 
//客户端向服务器发送
#define C2S_LOGIN		0x01
#define C2S_LOGOUT		0x02
#define C2S_ONLINE_USER 	0x03
 
#define MSG_LEN			512
 
//服务器向客户端应答
#define S2C_LOGIN_OK		0x01
#define S2C_ALREADY_LOGINED	0x02
#define S2C_SOMEONE_LOGIN	0x03
#define S2C_SOMEONE_LOGOUT	0x04
#define S2C_ONLINE_USER		0x05
 
//客户端向客户端发送
#define C2C_CHAT		0x10
 
//定义消息结构
typedef struct message
{
	int cmd;
	char body[MSG_LEN];
}MESSAGE;

//用户信息
typedef struct user_info
{
	char username[16];
	unsigned int ip;
	unsigned short port;
}USER_INFO;

//客户端和客户端传递的消息结构
typedef struct chat_msg
{
	char username[16];
	char msg[100];
}CHAT_MSG;
 
typedef list<USER_INFO> USER_LIST;
#endif

