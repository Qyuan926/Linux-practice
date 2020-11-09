#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>
void sigchild(int signo)
{
	printf("sigchild signal\n");
	exit(1);
}

int main()
{
   int pid=fork();
   signal(SIGCHLD,sigchild);
   if(pid<0)
   {
     perror("fork");	   
   }
   else if(pid==0)
   {
    sleep(2);//先沉睡，让父进程先执行处理任务，子进程后执行逻辑退出，可以观察到现象
    //子进程不沉睡，可能先于父进程启动前，执行完任务退出，无法观察到现象
    printf("this is child process:%d,parent:%d\n",getpid(),getppid());
    //printf("child:%d\n",pid);
    sleep(5);
   }
   else
   {
     //僵尸进程的处理方式：
     //1、父进程进行进程等待，等待子进程退出之后再处理自身业务
     //2.改变SIGCHID信号的处理方式，SIGCHID的处理方式为忽略处理SIGING
     //3.比较莽夫的做法就是：杀死父进程，让子进程称为孤儿进程
     //int status;
     //wait(&status);
     //printf("%x\n",(status>>8)&0xff);
     printf("parent:%d\n",pid);
     while(1)
    // printf("this is parent process:%d\n",getpid());	 
     sleep(5);
   }


   return 0;
}
