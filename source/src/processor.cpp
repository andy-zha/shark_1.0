/*****************************************************
copyright (C), Anhui University of Tecnology
File name：processor.cpp
Author: Andy  Version:0.1 Date: 2019-05-12 18:49
Description：
Funcion List: 
*****************************************************/
#include <unistd.h>
#include "processor.h"
#include "parsermgr.h"

processor::processor()
{
}

processor::~processor()
{
}

//初始化接口
int32_t processor::init()
{
    //解析器管理器初始化
    if (RET::SUC != parsermgr::init())
	{
	    printf("error:parser manager init failed!\n");
	    return RET::FAIL;
	}

	return RET::SUC;
}

//启动接口
int32_t processor::start()
{
	if (RET::SUC != threadobject::createthreadfunc((threadobjectFunc)&processor::process,
				SCHED_FIFO, PRIORITY_NORMAL))
	{
	    printf("error:processor module create pthread failed!\n");
		return RET::FAIL;
	}

	threadobject::start();
	return RET::SUC;
}

void processor::process()
{
	//线程启动
	threadobject::getthreadpolicy("Processor");

	//获取主缓冲区
	recyclequeue<cell> *_queue = NULL;
	capture::getinstance().get_queue(_queue);

	while (m_run) 
	{
		//pop出缓冲区
		cell *ce = _queue->pop();

        //结点为空不处理
		if (NULL == ce)
		{
			continue;
		}

        //释放指针
		if (NULL != ce)
		{
		    delete ce;
			ce = NULL;
		}
	}
}
