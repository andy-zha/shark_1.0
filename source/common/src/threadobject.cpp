/*************************************************************
*********************
*   Copyright(C++),2018,Demo
*   filename: threadobject.cpp
*   Author: Jobs
*   Version: 1.0.0
*   Date: 2018/7/4
*   Description: define threadobject.cpp
*************************************************************/

#include "threadobject.h"
#include <semaphore.h>
#include <string.h>

//构造函数
threadobject::threadobject()
{
    sem_init(&m_sem,0,0);
    pthreaddetail = NULL;
}

//析构函数
threadobject::~threadobject()
{
    sem_destroy(&m_sem);
    if (NULL != pthreaddetail)
	{
	    delete pthreaddetail;
		pthreaddetail = NULL;
	}
}

//创建线程
int32_t threadobject::createthreadfunc(threadobjectFunc threadfunc, int32_t schedtype, int32_t priority)
{
    union{
        pThreadFunction pfunc;
        void* (threadobject::*ThreadFunc)(void*);
    }proc;

    proc.ThreadFunc = threadfunc;

	//线程信息结构点
	pthreaddetail = new threaddetail();

	pthreaddetail->m_pfunc = proc.pfunc;

	//设置线程优先级begin
	pthread_attr_t * pattr = new pthread_attr_t(); //创建线程属性
	pthread_attr_init(pattr); //必须初始化
	int rs = 0;
	rs = pthread_attr_setscope(pattr,PTHREAD_SCOPE_SYSTEM);  //设置系统竞争（PTHREAD_SCOPE_SYSTEM线程在进程内竞争）
	assert(0 == rs); 
	rs = pthread_attr_setinheritsched(pattr,PTHREAD_EXPLICIT_SCHED); //设置不继承父进程的调度策略（PTHREAD_INHERIT_SCHED表示继承父进程的调度策略）
	assert(0 == rs);
	(void)rs;

	if(SCHED_FIFO == schedtype  //实时调度策略，一直占用CPU，除非被高优先级抢断或者自己放弃
			|| SCHED_RR == schedtype /*实时调度策略，时间片轮转，同优先级轮换执行 */)
	{
		int32_t priority_max = sched_get_priority_max(schedtype);
		assert(-1 != priority_max);
		int32_t priority_min = sched_get_priority_min(schedtype);
		assert(-1 != priority_min);
		int32_t rs = pthread_attr_setschedpolicy(pattr,schedtype); //设置调度策略
		assert(0 == rs);
		(void)rs;

		struct sched_param sched;
		switch(priority)
		{
			case PRIORITY_HIGH:
				sched.sched_priority = priority_max;  //高优先级
				break;
			case PRIORITY_NORMAL:
				sched.sched_priority = ( priority_max + priority_min ) / 2; //普通优先级
				break;
			case PRIORITY_LOW:
				sched.sched_priority = priority_min; //低优先级
			default:
				break;
		}

		rs = pthread_attr_setschedparam(pattr,&sched); //设置优先级
		assert(0 == rs);
	}
	else if(SCHED_OTHER == schedtype) //分时调度，不做处理
	{

	}
	else //其他情况，不做处理
	{

	}
        
	pthreaddetail->m_pattr = pattr;

    return RET::SUC;
}

//线程启动函数
int32_t threadobject::start()
{
    m_run = true;

    //创建线程
	int32_t rs = pthread_create(&(pthreaddetail->m_threadID),
			(pthreaddetail->m_pattr),
			(pthreaddetail->m_pfunc), this);
	if(0 != rs)
	{
		printf("thread start error[%d]，%s!\n",rs,strerror(rs));
		return RET::FAIL;
	}
	sem_wait(&m_sem);

    return RET::SUC;
}

int32_t threadobject::stop()
{
    return RET::SUC;
}

void threadobject::getcurthreaddetail(pthread_t &pthreadID)
{
    pthreadID = pthreaddetail->m_threadID;
}

void threadobject::getthreadpolicy(const char* pthread)
{
    sem_post(&m_sem);

    int32_t rs = 0;
    int policy;
    struct sched_param param;
    rs = pthread_getschedparam(pthread_self(), &policy, &param);
    assert(0 == rs);
    
    printf("%s: SchedType:%s Policy:%s\n",
            pthread,
            policy == SCHED_FIFO ? "SCHED_FIFO" :
            policy == SCHED_RR ? "SCHED_RR" :
            policy == SCHED_OTHER ? "SCHED_OTHER":
            "UNKNOW",
            param.sched_priority == PRIORITY_HIGH ? "PRIORITY_HIGH" :
			param.sched_priority == PRIORITY_NORMAL ? "PRIORITY_NORMAL" :
			param.sched_priority == PRIORITY_LOW ? "PRIORITY_LOW" :
			"UNKNOW");
    return;
}
