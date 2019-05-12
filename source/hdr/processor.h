/*****************************************************
copyright (C), Anhui University of Tecnology
File name：processor.h
Author: Andy  Version:0.1 Date: 2019-05-12 18:40
Description：
Funcion List: 
*****************************************************/
#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include "threadobject.h"
#include "capture.h"

class processor : public threadobject
{
	public:
		/**
		 * @brief
		 */
		processor();

		/**
		 * @brief
		 */
		~processor();
		
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
		 * @brief 返回单例接口
		 *
		 * @return 返回单例地址
		 */
		static processor& getinstance()
		{
			static processor instance;
			return instance;
		}

		/**
		 * @brief 初始化函数
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

		/**
		 * @brief 线程处理函数
		 */
		void process();
};

#endif
