/*****************************************************
copyright (C), Anhui University of Tecnology
File name：capture.h
Author: Andy  Version:0.1 Date: 2019-05-04 14:29
Description：
Funcion List: 
*****************************************************/

#include "asmlib.h"
#include "memdef.h"
#include "packet.h"
#include "protocol.h"
#include "pcap.h"
#include "parserbase.h"

namespace ns_capture
{
    //读包路径
    const char * const read_packet_path = ".";

    //pdu最大长度    
    const uint16_t cap_content_max_len = 1514;

    //读包模式结构体	
    class _CAPTURE_MODE
    {
        public:
	    /**
	     * @brief
	     */
	    _CAPTURE_MODE()
	    {
		dev = NULL;
	        snaplen = 0;
	        promisc = 0;	
		to_ms = 0;
	    }	    

	    /**
	     * @brief
	     */	
	    ~_CAPTURE_MODE()
	    {
		free_ptr(dev);
	    }
	public:
	    //抓包模式
	    char *dev;
	    //抓包长度1514
	    uint32_t snaplen;
	    //”1“代表混杂模式，其他值代表非混杂模式
	    uint8_t promisc;
	    //指定需要等地啊的毫秒数，超过这个时间后，获得数据包的函数会立即返回，0表示一直等待直到有数据包到来
	    uint8_t to_ms;
    };	    
}

class capture
{
    public:	
	/**
	 * @brief 构造函数
	 */
	capture();

	/**
	 * @brief 析构函数
	 */
	~capture();

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
	static capture& getinstance()
	{
	    static capture value;
	    return value;
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
	 * @brief 线程处理接口(从网口抓包)
	 */
	void capture_packet();

	/**
	 * @brief 线程处理接口(读包)
	 */
	void read_packet();

	/**
	 * @brief 设置链路层协议
	 *
	 * @prame linktype 文件头协议值;ce 包体传递
	 */
	void setlinktype(uint32_t linktype, cell &ce);

    private:
        /**
	 * @brief 抓包模式
	 */
        ns_capture::_CAPTURE_MODE _mode;	
};
