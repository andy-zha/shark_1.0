#ifndef _PARSERMGR_H
#define _PARSERMGR_H

#include "asmlib.h"
#include "memdef.h"
#include "parserbase.h"
#include "eth2_parser.h"

class parsermgr
{
    public:
	    /**
		 * @brief 构造函数
		 */
		parsermgr();

		/**
		 * @brief 析构函数
		 */
		~parsermgr();

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
		 * @brief 初始化
		 *
		 * @return RET::SUC 成功;RET::FAIL 失败
		 */
		static int32_t init();

    public:
	    /**
		 * @brief 解析器数组
		 */
		static parserbase **_parser;
};

#endif
