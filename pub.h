#ifndef _PUB_H_
#define _PUB_H_
 
#include <list>
#include <algorithm>
 
using namespace std;
 
//�ͻ��������������
#define C2S_LOGIN		0x01
#define C2S_LOGOUT		0x02
#define C2S_ONLINE_USER 	0x03
 
#define MSG_LEN			512
 
//��������ͻ���Ӧ��
#define S2C_LOGIN_OK		0x01
#define S2C_ALREADY_LOGINED	0x02
#define S2C_SOMEONE_LOGIN	0x03
#define S2C_SOMEONE_LOGOUT	0x04
#define S2C_ONLINE_USER		0x05
 
//�ͻ�����ͻ��˷���
#define C2C_CHAT		0x10
 
//������Ϣ�ṹ
typedef struct message
{
	int cmd;
	char body[MSG_LEN];
}MESSAGE;

//�û���Ϣ
typedef struct user_info
{
	char username[16];
	unsigned int ip;
	unsigned short port;
}USER_INFO;

//�ͻ��˺Ϳͻ��˴��ݵ���Ϣ�ṹ
typedef struct chat_msg
{
	char username[16];
	char msg[100];
}CHAT_MSG;
 
typedef list<USER_INFO> USER_LIST;
#endif

