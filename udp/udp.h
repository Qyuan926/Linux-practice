#pragma once
#include<iostream>
#include<unistd.h>
#include<cstdio>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
class udp{
  public:
       udp()
     {
        _sock=-1;   
     }   
     //创建套接字
     bool CreateSocket()
      {
	      _sock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	      //_sock小于0，则创建套接字失败
	      if(_sock<0)
	      {
               perror("socket");
	       return false;
	      }
	      return true;
      }
     //绑定地址信息
     bool Bind(std::string ip,uint16_t port)
     {
        struct sockaddr_in addr;
	//网络协议为ipv4
	addr.sin_family=AF_INET;
	//将本地字节序转换为网络字节序
	addr.sin_port=htons(port);
        //将ip地址转换为点分十进制
	addr.sin_addr.s_addr=inet_addr(ip.c_str());
        int ret=bind(_sock,(struct sockaddr*)&addr,sizeof(addr));
	if(ret<0)
         {
           perror("bind");
	   return false;
	 }
	return true;

     }
     //发送数据
     bool Send(std::string& data,struct sockaddr_in* destaddr)
     {
         int sendsize=sendto(_sock,data.c_str(),data.size(),0,(struct sockaddr*)destaddr,sizeof(struct sockaddr_in));
          if(sendsize<0)
	  {

		  perror("sento");
		  return false;
	  }
	  return true;
     }
     //接收数据
     //buf是一个出参，让调用者获取接收到的信息
     //srcaddr出参，获取源端主机的地址信息
     bool Recv(std::string* buf,struct sockaddr_in* srcaddr )
     {
         char arr[1024];
	 memset(arr,'\0',sizeof(arr));
	 socklen_t  addrlen=sizeof(struct sockaddr_in);
	 int recvsize=recvfrom(_sock,arr,sizeof(arr)-1,0,(struct sockaddr*)srcaddr,&addrlen);
         if(recvsize<0)
	 {
		 perror("recvfrom");
		 return false;
	 }
	 (*buf).assign(arr,recvsize);
         return true;
     }
     void Close()
     {
        close(_sock);
	_sock=-1;

     }

  private:
     int _sock;
 };

