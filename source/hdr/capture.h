/*****************************************************
copyright (C), Anhui University of Tecnology
File name：capture.h
Author: Andy  Version:0.1 Date: 2019-05-04 14:29
Description：抓包模块头文件
Funcion List: 
*****************************************************/
#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include "asmlib.h"
#include "memdef.h"
#include "packet.h"
#include "protocol.h"
#include "pcap.h"
#include "parserbase.h"
#include "threadobject.h"
#include "recyclequeue.h"

namespace ns_capture
{
	//抓包模式枚举值
	typedef enum capture_mode
	{
		em_capture_mode_start = -1,            //枚举保护
		em_read_capfile_mode,                  //读包模式
		em_capture_from_wan,                   //网口模式
		em_capture_mode_end
	}em_capture_mode;

	//读包路径
	const char * const read_packet_path = ".";

	//pdu最大长度    
	const uint16_t cap_content_max_len = 1514;

	//读包模式结构体	
	class _CAPTURE_MODE
	{
		public:
			/**
			 * @brief 构造函数
			 */
			_CAPTURE_MODE()
			{
			    mode = ns_capture::em_capture_mode_start;
				dev = NULL;
				snaplen = 0;
				promisc = 0;	
				to_ms = 0;
			}	    

			/**
			 * @brief 析构函数 
			 */	
			~_CAPTURE_MODE()
			{
				free_ptr(dev);
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
			//抓包模式
			int16_t mode;
			//抓包设备
			char *dev;
			//抓包长度1514
			uint32_t snaplen;
			//”1“代表混杂模式，其他值代表非混杂模式
			uint8_t promisc;
			//指定需要等地啊的毫秒数，超过这个时间后，获得数据包的函数会立即返回，0表示一直等待直到有数据包到来
			uint8_t to_ms;
	};	    

	//链路层协议映射关系
	const int8_t bsd_linktype = 0;
	const int8_t eth_linktype = 1;
	const int8_t token_ring_linktype = 6;
	const int8_t arcnet_linktype = 7;
	const int8_t slip_linktype = 8;
	const int8_t ppp_linktype = 9;
	const int8_t fddi_linktype = 10;
	const int8_t llc_linktype = 100; 
	const int8_t rawip_linktype = 101;
	const int8_t bsd_slip_linktype = 102;
	const int8_t bsd_ppp_linktype = 103;
	const int8_t cisco_linktype = 104; 
	const int8_t eleven_linktype = 105;
	const int8_t openbsd_linktype = 108;
	const int8_t linux_cooked_linktype = 113;
	const int8_t localtalk_linktype = 114;
}

class capture : public threadobject
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
			static capture instance;
			return instance;
		}

		/**
		 * @brief 初始化函数
		 *
		 * @return RET::SUC 成功;RET::FAIL 失败
		 */
		int32_t init(int16_t _mode);

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
		void setlinktype(uint32_t linktype, cell *ce);

		/**
		 * @brief 获取主缓冲区
		 *
		 * return 主缓冲区地址
		 */
		void get_queue(recyclequeue<cell> *&queue);

		/**
		 * @brief 写日志文件
		 */
		void write_log();

	private:
		/**
		 * @brief 抓包模式
		 */
		ns_capture::_CAPTURE_MODE _mode;	

		/**
		 * @brief 主缓冲区
		 */
		recyclequeue<cell> _queue;

		/**
		 * @brief 读文件总数(日志信息)
		 */
		uint32_t u_file_totals;

		/**
		 * @brief 读文件失败总数(日志信息)
		 */
		uint32_t u_read_file_fail;

		/**
		 * @brief 读文件成功总数(日志信息)
		 */
		uint32_t u_read_file_suc;

		/**
		 * @brief 异常文件个数
		 */
		uint32_t u_abnormal_file;

		/**
		 * @brief 读包总数(日志信息)
		 */
		uint32_t u_pkt_totals;

		/**
		 * @brief 读包失败次数(日志信息)
		 */
		uint32_t u_read_pkt_fail;

		/**
		 * @brief 读包成功总数(日志信息)
		 */
		uint32_t u_read_pkt_suc;

		/**
		 * @brief 异常包个数
		 */
		uint32_t u_abnormal_pkt;
};

#endif
