#include "pub.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <algorithm> 
 
			
#define ERR_EXIT(m)                     \
        do                              \
        {                               \
                perror(m);              \
                exit(EXIT_FAILURE);     \
        }while(0);    
 
//聊天室成员列表
USER_LIST client_list;
 
void chat_server(int server_fd);
void do_login(MESSAGE msg,int sock,struct sockaddr_in *clientaddr);
void do_logout(MESSAGE msg,int sock,struct sockaddr_in *clientaddr);
void do_online_user(int sock,struct sockaddr_in *clientaddr);
 
int main(void)
{
	int sock;
	struct sockaddr_in servaddr;
 
	if ((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		ERR_EXIT("socket");
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9999);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
	if (bind(sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	chat_server(sock);
 
	return 0;
}
 
void chat_server(int server_fd)
{
        struct sockaddr_in clientaddr;
        socklen_t clientlen;
        int n;
        MESSAGE msg;
        while (1)
        {
                memset(&msg,0,sizeof(msg));
                clientlen = sizeof(clientaddr);
                n = recvfrom(server_fd,&msg,sizeof(msg),0,(struct sockaddr*)&clientaddr,&clientlen);
                if (n < 0)
                {
                        if (errno == EINTR)
                                continue;
                        ERR_EXIT("recvfrom");
                }
                int cmd = ntohl(msg.cmd);
                switch (cmd)
                {
                        case C2S_LOGIN:
                                do_login(msg,server_fd,&clientaddr);
                                break;
			case C2S_LOGOUT:
				do_logout(msg,server_fd,&clientaddr);
                                break;
			case C2S_ONLINE_USER:
				do_online_user(server_fd,&clientaddr);
				break;
			default:
				break;
       
                }
 
        }
}
 
void do_login(MESSAGE msg,int sock,struct sockaddr_in *clientaddr)
{
	USER_INFO user;
	strcpy(user.username,msg.body);
	user.ip = clientaddr->sin_addr.s_addr;
	user.port = clientaddr->sin_port;
	
	/*查找用户*/
	USER_LIST::iterator it;
	for (it=client_list.begin(); it!=client_list.end(); ++it)
	{
		if (strcmp(it->username,msg.body) == 0)
		{
			break;
		}
	}
	
	if (it == client_list.end())	//没找到用户
	{
		printf("has a user login:%s <-> %s:%d\n",msg.body,inet_ntoa(clientaddr->sin_addr),ntohs(clientaddr->sin_port));
		client_list.push_back(user);
	
		//登录成功应答
		MESSAGE reply_msg;
		memset(&reply_msg,0,sizeof(reply_msg));
		reply_msg.cmd = htonl(S2C_LOGIN_OK);
		sendto(sock,&reply_msg,sizeof(reply_msg),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in));
 
		int count = htonl((int)client_list.size());
		//发送在线人数
		sendto(sock,&count,sizeof(int),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in));
		printf("sending user list information to:%s <-> %s:%d\n",msg.body,inet_ntoa(clientaddr->sin_addr),ntohs(clientaddr->sin_port));
		//发送在线列表
		for (it=client_list.begin(); it != client_list.end(); ++it)
		{
			 sendto(sock,&*it,sizeof(USER_INFO),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in));
 
		}
		//向其他用户通知
		for (it=client_list.begin(); it != client_list.end(); ++it)
		{
			if (strcmp(it->username,msg.body) == 0)
				continue;
			struct sockaddr_in peeraddr;
			memset(&peeraddr,0,sizeof(peeraddr));
			peeraddr.sin_family = AF_INET;
			peeraddr.sin_port = it->port;
			peeraddr.sin_addr.s_addr = it->ip;
 
			msg.cmd = htonl(S2C_SOMEONE_LOGIN);
			memcpy(msg.body,&user,sizeof(user));
 
			if ( sendto(sock,&msg,sizeof(msg),0,(struct sockaddr*)&peeraddr,sizeof(struct sockaddr_in)) )
			{
				printf("sending user list information to:%s <-> %s:%d\n",it->username,inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
			}
				
		}		
	}
	else
	{
		printf("user %s has already logined\n",msg.body);
 
		MESSAGE reply_msg;
		memset(&reply_msg,0,sizeof(reply_msg));
		reply_msg.cmd = htonl(S2C_ALREADY_LOGINED);
		sendto(sock,&reply_msg,sizeof(reply_msg),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in));
	}
}
 
void do_logout(MESSAGE msg,int sock,struct sockaddr_in *clientaddr)
{	
	USER_INFO user;
 
	strcpy(user.username,msg.body);
	user.ip = clientaddr->sin_addr.s_addr;
	user.port = clientaddr->sin_port;
	
	/*查找用户*/
	USER_LIST::iterator it;
	for (it=client_list.begin(); it!=client_list.end(); ++it)
	{
		if (!strcmp(it->username,msg.body))
		{
			printf("client %s logout server.\n",msg.body);
			client_list.erase(it); 
			break;
		}
	}
	/*向其他用户通知*/
	for (it=client_list.begin(); it != client_list.end(); ++it)
	{
		struct sockaddr_in peeraddr;
		memset(&peeraddr,0,sizeof(peeraddr));
		peeraddr.sin_family = AF_INET;
		peeraddr.sin_port = it->port;
		peeraddr.sin_addr.s_addr = it->ip;
 
		msg.cmd = htonl(S2C_SOMEONE_LOGOUT);
		memcpy(msg.body,&user,sizeof(user));
 
		sendto(sock,&msg,sizeof(msg),0,(struct sockaddr*)&peeraddr,sizeof(struct sockaddr_in));
	}		
}
 
void do_online_user(int sock,struct sockaddr_in *clientaddr)
{
	MESSAGE msg;
	memset(&msg,0,sizeof(msg));
 
	msg.cmd = htonl(S2C_ONLINE_USER);
	
	if ( sendto(sock,(const char*)&msg,sizeof(msg),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in)) )
	{
		  printf("sending user list information to: <-> %s:%d\n",inet_ntoa(clientaddr->sin_addr),ntohs(clientaddr->sin_port));
	}
	USER_LIST::iterator it;
 
 
        int count = htonl((int)client_list.size());
        //发送在线人数
        sendto(sock,&count,sizeof(int),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in)); 
        //发送在线列表
        for (it=client_list.begin(); it != client_list.end(); ++it)
        {
            sendto(sock,&*it,sizeof(USER_INFO),0,(struct sockaddr*)clientaddr,sizeof(struct sockaddr_in));
 
        }
 
}

