#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>
#include<stdlib.h>

#define THREADCOUNT 4



//线程池 = 线程安全队列 + 一大堆的线程
////线程安全队列     元素 = 数据 + 处理数据的函数地址
typedef  void* (*handler_t)(int);
class threadTask{
public:
     threadTask(int data,handler_t  handler)
   {
     Data_=data;
     handler_=handler; 
   }
   ~threadTask()
   {}
   void  Run()
    {
      handler_(Data_);
    }

private:
   int Data_;
   handler_t  handler_; 

};



class threadPool{
public: 
      threadPool()
      {
           capacity_=7;
           threadcount_=THREADCOUNT;
           pthread_mutex_init(&lock_,NULL);
           pthread_cond_init(&cond_,NULL);
           
           curThreadcount_=threadcount_;
           
           isexit_=false;
           bool is_createthread=ThreadCreate();
            if(!is_createthread)
            {
              printf("ThreadPool:It is failed to create thread\n");
              exit(1);
            } 

          
      }
      ~threadPool()
       {}
      bool  push(threadTask* data)
      {
           pthread_mutex_lock(&lock_);
           if(isexit_)
            {
              pthread_mutex_unlock(&lock_);
              return false; 
            }
           this->que_.push(data);
           pthread_mutex_unlock(&lock_);
           
           pthread_cond_signal(&cond_);
           return true;   
      }
      bool  pop(threadTask** data )
      {
        // pthread_mutex_lock(&lock_); 
         *data=this->que_.front();
         this->que_.pop();
        // pthread_mutex_unlock(&lock_);
         return true; 
      }
      
      void threadJoin()
      { 
       for(size_t i=0; i<threadcount_;++i)
        {
            pthread_join(tid_[i],NULL);
        }
      } 

      void  ThreadPoolclear()
       {
             pthread_mutex_lock(&lock_);
             isexit_=true;
             pthread_mutex_unlock(&lock_);

             pthread_cond_broadcast(&cond_); 
      
       }

private:
       static void* Threadstart(void* arg)
       {
           threadPool* tp=(threadPool*)arg;
           while(1)
            {

              //获取队列当中的数据，按特定的指令进行消费
              pthread_mutex_lock(&tp->lock_);
              threadTask* tt;
              while(tp->que_.empty())
              {
                //如果没有数据处理，且不会再获取到数据，线程退出
                if(tp->isexit_)
                {
                  tp->curThreadcount_--;
                  pthread_mutex_unlock(&tp->lock_);
                  pthread_exit(NULL);
                }
                //如果队列中暂时没有数据，调用条件变量等待接口
                 pthread_cond_wait(&tp->cond_,&tp->lock_);
              } 
              tp->pop(&tt);
              pthread_mutex_unlock(&tp->lock_);
              tt->Run(); 
              delete  tt;
            }  
       }
       bool  ThreadCreate()
      {
         for(size_t i=0;i<threadcount_;++i)
           {
             int ret =pthread_create(&tid_[i],NULL,Threadstart,(void*) this);
             if(ret!=0)
               {
                  perror("pthread_create");
                  return false; 
               }   
            }
         return true;
      }

private :
   std::queue<threadTask* > que_;
   size_t  capacity_;
   //互斥锁、条件变量
   pthread_mutex_t lock_;
   pthread_cond_t  cond_;
   //线程数量
   size_t  threadcount_;
   pthread_t  tid_[THREADCOUNT];
   //判断该线程是否退出
   bool  isexit_;

   size_t   curThreadcount_;
};
 void*  DealData(int data)
  {
     printf("I am deal the data  of [ %d ]\n",data);
     return NULL;
  }

int main()
{
  threadPool* tp=new threadPool () ;
  for(int i=1; i<=50;++i)
    {
      threadTask* tt=new threadTask(i,DealData);
      tp->push(tt);  
    }

    sleep(17);
    tp->ThreadPoolclear();
    tp->threadJoin(); 
   delete tp;
   return 0;
}

