/*****************************************************
copyright (C), Anhui University of Tecnology
File name：eth2_parser.h
Author: Andy  Version:0.1 Date: 2019-05-04 14:29
Description：eth2解析器头文件
Funcion List: 
*****************************************************/
#ifndef _ETH2_PARSER_H_
#define _ETH2_PARSER_H_

#include "asmlib.h"
#include "memdef.h"
#include "parserbase.h"

class eth2_parser : public parserbase
{
    public:
	    /**
		 * @brief 构造函数
		 */
	    eth2_parser();

		/**
		 * @brief 析构函数
		 */
		~eth2_parser();

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

		/**
		 * @brief 初始化接口
		 *
		 * @return RET::SUC 成功;RET::FAIL 失败
		 */
		int32_t init();

		/**
		 * @brief 启动接口
		 *
		 * @return RET::SUC 成功;RET::FAIL 失败
		 */
		int32_t start();
};

#endif
