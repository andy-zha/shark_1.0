/*************************************************************
*********************
*   Copyright(C++),2018,Demo
*   filename: threaddetail.h
*   Author: Jobs
*   Version: 1.0.0
*   Date: 2018/7/6
*   Description: define threaddetail.h
*************************************************************/
#include <pthread.h>
#include "dlist.h"

#ifndef _THREADDETAIL_H_
#define _THREADDETAIL_H_

#define PRIORITY_HIGH 99
#define PRIORITY_NORMAL 50
#define PRIORITY_LOW 0

typedef void* (*pThreadFunction)(void*);
class threaddetail
{
    public:
        /*
         * @brief 构造函数
         */
        threaddetail():m_uthreadindex(0),m_pfunc(NULL)
        {
        }

        /*
         * @brief 析构函数
         */
        virtual ~threaddetail()
        {
        }

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

    public:
        /*
         * @brief 同一类线程序号 
         */
        uint32_t m_uthreadindex;

        /*
         * @brief 线程函数
         */
        pThreadFunction m_pfunc;

        /*
         * @brief 线程ID
         */
        pthread_t m_threadID;
        
        /*
         * @brief 线程属性
         */
        pthread_attr_t *m_pattr;
};

#endif
