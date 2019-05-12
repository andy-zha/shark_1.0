/*****************************************************
copyright (C), Anhui University of Tecnology
File name：recyclequeue.h
Author: Andy  Version:0.1 Date: 2019-05-12 17:41
Description：
Funcion List: 
*****************************************************/
#ifndef _RECYCLEQUEUE_H_
#define _RECYCLEQUEUE_H_

#include "asmlib.h"
#include "memdef.h"

namespace ns_recyclequeue
{
	//缓冲区最小大小
	const uint64_t recyclequeue_min_power = 12;

	//缓冲区最大大小
	const uint64_t recyclequeue_max_power = 17;
}

template<typename Type>
class recyclequeue
{
	public:
		/**
		 * @brief 构造函数
		 */
		recyclequeue():_Qread(0), _Qwrite(0), _Qsize(0), _Qmask(0), _Qlost(0), _Qdata(NULL)
		{
		}

		/**
		 * @brief 析构函数
		 */
		~recyclequeue()
		{
			clear();
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

		/**
		 * @brief 初始化大小
		 *
		 * @prame power 队列大小
		 *
		 * @return RET::SUC 成功;RET::FAIL 失败 
		 */
		int32_t init(uint16_t power)
		{
			if (ns_recyclequeue::recyclequeue_min_power >= power)
			{
				power = ns_recyclequeue::recyclequeue_min_power;
			}
			else
			{
				power = ns_recyclequeue::recyclequeue_max_power >= power 
					? ns_recyclequeue::recyclequeue_max_power : power;
			}

			_Qsize = ((uint64_t)1) << power;
			_Qmask = _Qsize - 1;

			try
			{
				size_t queuesize = _Qsize * sizeof(Type*);
				_Qdata = (Type**)_MEM_NEW_(queuesize);
			}
			catch(std::bad_alloc)
			{
				return RET::FAIL;
			}
			memset(_Qdata, '\0', _Qsize * sizeof(Type*));

			_Qread = 0;
			_Qwrite = 0;
			_Qlost = 0;
			return RET::SUC;  
		}

		/**
		 * @brief 入队
		 *
		 * @prame node 结构体
		 *
		 * return RET::SUC 成功;RET::FAIL 失败
		 */
		int32_t push(Type *node)
		{
#ifndef _DEBUG_VERSION_
			assert(NULL!= node);
#endif
			if (_Qread <= _Qwrite & _Qmask) 
			{
				_Qdata[_Qwrite] = node;
				_Qwrite++;
				return RET::SUC;
			}

			_Qlost++;
			return RET::FAIL;
		}

		/**
		 * @brief 出队
		 *
		 * @return node 结构体
		 */
		Type *pop()
		{
			Type *node = NULL;
			if (_Qread < _Qwrite)
			{
				node = _Qdata[_Qread];
				_Qread++;
				return node;
			}

			return NULL;
		}

		/**
		 * @brief 获取队列大小
		 *
		 * return 队列大小
		 */
		uint64_t size()
		{
			return _Qsize;
		}

		/**
		 * @brief 获取队列丢失节点数
		 *
		 * return 队列丢失节点数
		 */
		uint64_t lost()
		{
			return _Qlost;
		}

		/**
		 * @brief 清队
		 */
		void clear()
		{
			if (NULL != _Qdata)
			{
				_MEM_DEL_(_Qdata);
				_Qdata = NULL;
			}

			_Qread = 0;
			_Qwrite = 0;
			_Qlost = 0;
		}

	private:
		/**
		 * @brief 读指针位置
		 */
		uint64_t _Qwrite;

		/**
		 * @brief 写指针位置
		 */
		uint64_t _Qread;

		/**
		 * @brief 队列大小
		 */
		uint64_t _Qsize;

		/**
		 * @brief 掩码
		 */
		uint64_t _Qmask;

		/**
		 * @brief 丢失节点数目
		 */
		uint64_t _Qlost;

		/**
		 * @brief 指向节点
		 */
		Type **_Qdata;
};

#endif
