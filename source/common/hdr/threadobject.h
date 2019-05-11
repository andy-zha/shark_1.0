/*************************************************************
*********************
*   Copyright(C++),2018,Demo
*   filename: threadobject.h
*   Author: chzha
*   Version: 1.0.0
*   Date: 2018/7/4
*   Description: define threadobject.h
*************************************************************/
#ifndef _THREADOBJECT_H_
#define _THREADOBJECT_H_
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <semaphore.h>
#include "threaddetail.h"

#define THREAD_MAX_NUM 64

class threadobject;

typedef void* (threadobject::*threadobjectFunc)(void*);

class threadobject
{
    public:
        /*
         * @brief 构造函数
         */
        threadobject();
        
        /*
         * @brief 析构函数
         */
        virtual ~threadobject();

	    /**
	     * @brief 重载 内存分配
	     *
	     * @prame size 分配大小
	     *
	     * @return 分配地址
	     */
	    static void * operator new(size_t size)
	    {
		    void *p = (void*)_MEM_NEW_(size);
		    return p;
	    }

	    /**
	     * @brief 重载 内存释放
	     *
	     * @prame p 释放指针
	     */
	    static void operator delete(void *p)
	    {
		    _MEM_DEL_(p);
	    }

        /*
         * @brief 创建线程
		 *
		 * @prame threadfunc 线程处理函数;schedtype 线程策略;priority 优先级;
		 *
		 * return RET::SUCCESS 成功;RET::FAIL 失败
         */
        virtual int32_t createthreadfunc(threadobjectFunc threadfunc, 
                              int32_t schedtype = SCHED_FIFO, 
                              int32_t priority = PRIORITY_NORMAL);

        /*
         * @brief 线程启动函数
		 *
		 * return RET::SUCCESS 成功; RET::FAIL 失败;
         */
        virtual int32_t start();

        /*
         * @brief 线程停止函数
		 *
		 * return RET::SUCCESS 成功; RET::FAIL 失败;
         */
        virtual int32_t stop();
        
        /*
         * @brief 获取线程信息
		 *
		 * @prame pthreadID 线程id
         */
        virtual void getcurthreaddetail(pthread_t &pthreadID);

        /*
         * @brief 获取线程优先级
		 *
		 * @prame pthread 模块名称; uthreadnum 线程id;
         */
        virtual void getthreadpolicy(const char *pthread);

    public:
	    /**
		 * @brief 线程运行状态
		 */
	    bool m_run;

    protected:
        /*
         * @brief 线程信息
         */
        threaddetail *pthreaddetail;

		/**
		 * @brief 信号量
		 */
		sem_t m_sem;
};

#endif
