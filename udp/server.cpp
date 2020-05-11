#include"udp.h"


int main(int argc,char*  argv[])
{
   if(argc!=3)
   {
     printf("发生错误，使用：./server [client ip]  [client port]");
     return 0;
   }
   std::string cli_ip=argv[1];
   uint16_t cli_port=atoi(argv[2]);
   udp cli;
   if(!cli.CreateSocket())
	   return 0;
   if(!cli.Bind(cli_ip,cli_port))
	   return 0;
   while(1)
   {
     std::string buf;
     struct sockaddr_in addr;
    int flag= cli.Recv(&buf,&addr);
    if(flag)
    {
     printf("client says:%s ",buf.c_str() );
    }
    else
    {

	    printf("本次接受数据失败\n");
    }
    printf("server says:");
    std::cin>>buf;
    cli.Send(buf,&addr);
   }
   cli.Close();
   return 0;
}
