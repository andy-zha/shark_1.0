/*****************************************************
copyright (C), Anhui University of Tecnology
File name：processor.cpp
Author: Andy  Version:0.1 Date: 2019-05-12 18:49
Description：
Funcion List: 
*****************************************************/
#include <unistd.h>
#include "processor.h"

processor::processor()
{
}

processor::~processor()
{
}

//初始化接口
int32_t processor::init()
{
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
	recyclequeue<cell> _queue = capture::getinstance().get_queue();

	while (m_run) 
	{
		//pop出缓冲区
		cell *ce = _queue.pop();
		if (NULL == ce)
		{
			continue;
		}

		std::cout<<ce->u_protocoltype<<std::endl;
		sleep(1);
	}
}
