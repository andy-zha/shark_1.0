/*****************************************************
copyright (C), Anhui University of Tecnology
File name：parserbase.h
Author: Andy  Version:0.1 Date: 2019-05-10 21:39
Description：
Funcion List: 
*****************************************************/

#ifndef _PARSERBASE_H_
#define _PARSERBASE_H_

#include "asmlib.h"
#include "memdef.h"
#include "protocol.h"

#pragma pack(push)
#pragma pack(4)

class cell
{
    public:
        /**
	 * @brief 构造函数
	 */
	cell()
	{
            p_str = NULL;
            u_protocoltype = ns_protocol::em_protocol_start;	    
	}	

        /**
	 * @brief 析构函数
	 */
	~cell()
	{
            free_ptr(p_str);
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
	char *p_str;                                  //数据体
	uint16_t u_protocoltype;                      //当前协议栈类型	
	uint32_t u_len;                               //数据体长度
	uint32_t u_payload;                           //负载长度
	uint32_t u_offset;                            //偏移长度
};

#pragma pack(pop)

//解析器基类
class parserbase
{
    public:
	    /**
		 * @brief 构造函数
		 */
		parserbase()
		{}

		/**
		 * @brief 析构函数
		 */
		virtual ~parserbase()
		{}

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
		virtual int32_t init()
		{
		    return RET::SUC;
		}

		/**
		 * @brief 解析开始接口
		 *
		 * return RET::SUC 成功;RET::FAIL 失败
		 */
		virtual int32_t start(cell *ce)
        {
		    return RET::SUC;
		}  
};

#endif
