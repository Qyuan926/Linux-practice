#include"udp.h"

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
       printf("发生错误 使用：./client [server ip]  [srever port]");
       return 0;
    }
    std::string svr_ip=argv[1];
    uint16_t  svr_port=atoi(argv[2]);
    udp svr;
    if(!svr.CreateSocket())
    {
         return 0;
    }
    //组织服务端的信息
    struct sockaddr_in destaddr;
    destaddr.sin_family=AF_INET;
    destaddr.sin_port=htons(svr_port);
    destaddr.sin_addr.s_addr=inet_addr(svr_ip.c_str());

    while(1)
    {
       printf("I  send:" );
       fflush(stdout);
       std::string buf;
       std::cin>>buf;
       int flag=svr.Send(buf,&destaddr);
       if(!flag)
       {
	       std::cout<<"数据发送失败"<<std::endl;
	       return 0;
       }
       struct sockaddr_in addr;
       svr.Recv(&buf,&addr);
       printf("he says: %s",buf.c_str());


    }
    svr.Close();
    return 0;
}
