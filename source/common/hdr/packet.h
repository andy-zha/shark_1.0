/*****************************************************
copyright (C), Anhui University of Tecnology
File name：packet.h
Author: Andy  Version:0.1 Date: 2019-05-04 22:06
Description：
Funcion List: 
*****************************************************/
#ifndef _PACKET_H_
#define _PACKET_H_

#include "asmlib.h"
#include "memdef.h"

namespace ns_pcap
{
    //包头
    class packet_header
    {
	public:
            /**
	     * @brief 构造函数
	     */
	    packet_header()
	    {
		seconds = 0;
	        u_seconds = 0;
	        caplen = 0;
	        len = 0;	
	    }	    

            /**
	     * @brief 析构函数
	     */
	    ~packet_header()
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
	    //秒数
	    uint32_t seconds;    
	    //毫秒数
	    uint32_t u_seconds;
	    //数据包长度
	    uint32_t caplen;
	    //文件数据包长度
	    uint32_t len;
    };
    
    //文件头
    class file_header
    {
        public:
	    /**
	     * @brief 构造函数
	     */
	    file_header()	
	    {
	        magic = 0;
		major_version = 0;
		minor_version = 0;
		ThisZone = 0;
		SigFigs = 0;
		Snaplen = 0;
		LinkType = 0;
	    }

	    /**
	     * @brief 析构函数
	     */
	    ~file_header()
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
	    //魔术数
	    uint32_t magic;
	    //当前文件的主要版本号
	    uint16_t major_version;
	    //当前文件的次要版本号
	    uint16_t minor_version;
	    //当地的标准时间，如果用的是GMT则全零
	    int32_t ThisZone;
	    //时间戳的精度
	    uint32_t SigFigs;
	    //最大的存储长度，最大长度为ffff0000
	    uint32_t Snaplen;
	    //链路类型 
	    uint32_t LinkType;    
    };
}

#endif
