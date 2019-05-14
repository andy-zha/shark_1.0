/*****************************************************
 copyright (C), Anhui University of Tecnology
 File name：main.cpp
 Author: Andy  Version:0.1 Date: 2019-05-10 23:00
 Description：主程序入口
 Funcion List: 
*****************************************************/
#include <unistd.h>
#include "capture.h"
#include "processor.h"

#define _GTEST_

#ifdef _GTEST_
#include "gtest/gtest.h"
#endif

void print_usage()
{
}

int32_t main(int argc, char **argv)
{
#ifndef _GTEST_
	if (argc <= 1)
	{
		print_usage();
		return RET::FAIL;
	}

	int32_t ch = -1;
	int16_t i_mode = -1;
	//读取程序启动模式
	while ((ch = getopt(argc, argv, "fchp:")) != -1)
	{
		switch(ch)
		{
			case 'f':
				i_mode = ns_capture::em_read_capfile_mode;
				break;
			case 'c':
			    break;
			default:
				print_usage();
			    break;	
		}
	}

	//主处理初始化
	if (RET::SUC != processor::getinstance().init())
	{
		printf("error:processor init failed!\n");
		return RET::FAIL;
	}

	//抓包模块初始化
	if (RET::SUC != capture::getinstance().init(i_mode))
	{
		printf("error:capture module init failed!\n");
		return RET::FAIL;
	}

	//主处理启动
	if (RET::SUC != processor::getinstance().start())
	{
		printf("error:processor start failed!\n");
		return RET::FAIL;
	}

	//抓包模式启动
	if (RET::SUC != capture::getinstance().start())
	{
		printf("error:capture module start failed!\n");
		return RET::FAIL;
	}

	while (true)
	{
		sleep(1);
	}

    return RET::SUC;
#endif

#ifdef _GTEST_
	printf("Running main() from %s\n", __FILE__);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#endif
}
