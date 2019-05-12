/*****************************************************
copyright (C), Anhui University of Tecnology
File name：capture.cpp
Author: Andy  Version:0.1 Date: 2019-05-04 15:07
Description：抓包模块
Funcion List: 
*****************************************************/

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <list>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "capture.h"

capture::capture()
{

}

capture::~capture()
{
}

//抓包模块初始化
int32_t capture::init(int16_t mode)
{
    //设置抓包模式
    switch (mode)
	{
	    //读包模式
	    case ns_capture::em_read_capfile_mode:
		{
		    _mode.mode = ns_capture::em_read_capfile_mode;
			break;
		}  
		//网口抓包
		case ns_capture::em_capture_from_wan:
		{
			_mode.mode = ns_capture::em_capture_from_wan;
			_mode.dev = (char*)_MEM_NEW_(10);
			sprintf(_mode.dev, "lo");
			_mode.snaplen = ns_capture::cap_content_max_len;
			_mode.promisc = 1;    
			_mode.to_ms = 0;
			break;
		}
		default:
		{
			return RET::FAIL;
		}
	}

	//初始化主缓冲区
	uint64_t u_size = 12;
	if (RET::SUC != _queue.init(u_size))
	{
		printf("error:capture init queue failed!\n");
		return RET::FAIL;
	}

	return RET::SUC;	
}

//抓包模块启动
int32_t capture::start()
{
	int32_t i_ret = RET::FAIL;
	if (ns_capture::em_read_capfile_mode == _mode.mode)
	{
		i_ret = threadobject::createthreadfunc((threadobjectFunc)&capture::read_packet,
				SCHED_FIFO, PRIORITY_NORMAL);
	}
	else if (ns_capture::em_capture_from_wan == _mode.mode)
	{
		i_ret = threadobject::createthreadfunc((threadobjectFunc)&capture::capture_packet,
				SCHED_FIFO, PRIORITY_NORMAL);
	}

	if (RET::SUC != i_ret)
	{
	    printf("error:capture module create pthread failed!\n");
		return RET::FAIL;
	}

    threadobject::start();
	return RET::SUC;	
}

//网口抓包
void capture::capture_packet()
{
	//线程启动
	threadobject::getthreadpolicy("Capture");

	while(m_run)	
	{

		//错误信息数组    
		char errbuf[1024];    
		//打开网络设备 
		pcap_t *handle = pcap_open_live(_mode.dev, _mode.snaplen, _mode.promisc, _mode.to_ms, errbuf);
		if (NULL == handle){
			std::cout<<"capture error:open dev failed:"<<errbuf<<std::endl;
			continue;	    
		}

		while (true)
		{
			//获取数据报
			const unsigned char *p_packet_content = NULL;
			struct pcap_pkthdr protocol_header;
			p_packet_content = pcap_next(handle, &protocol_header); 	    
			uint16_t ulen = strlen((const char*)p_packet_content);
			if (NULL == p_packet_content || 0 == ulen){
				continue;	    
			}

			std::cout<<p_packet_content<<std::endl;
		}

		//关闭设备
		pcap_close(handle);
	}
}

//读包接口
void capture::read_packet()
{
	//线程启动
	threadobject::getthreadpolicy("Capture");

	DIR *dir;
	std::list<std::string> names;
	while (true)
	{
		//打开读包路径    
		dir = opendir(ns_capture::read_packet_path);
		if (NULL == dir)
		{
			std::cout<<"capture error:dir null!"<<std::endl;

#ifdef _DEBUG_VERSION_	
			assert(NULL != dir);	    
#endif

		}

		//读指定路径下的文件
		struct dirent *pFilename = readdir(dir);
		while (NULL != pFilename)
		{
			if (0 != strlen(pFilename->d_name) && '.' != pFilename->d_name[0]
					&& !strcmp(pFilename->d_name + (strlen(pFilename->d_name) - strlen("pcap")), "pcap")
					|| !strcmp(pFilename->d_name + (strlen(pFilename->d_name) - strlen("cap")), "cap"))
			{
				names.push_back(pFilename->d_name);    
			}
			pFilename = readdir(dir);
		}
		//关闭读包路径
		closedir(dir);

		//读包模式开始
		std::string filename;
		while (!names.empty())
		{
			//读取文件名	
			filename = names.front();
			char *pfile = const_cast<char*>(filename.c_str());
			u_file_totals++;

			//构建删除文件名指令
			char cmd[100];
			snprintf(cmd, 100, "rm -rf %s", pfile);

			//打开文件
			FILE *fp = fopen(pfile, "r");
			if (NULL== fp)
			{
			    u_read_file_fail++;
				continue;    
			}

			//读取文件头
			ns_pcap::file_header fileheader;
			if (fread((void*)&fileheader, 1, sizeof(fileheader), fp) <= 0)
			{
			    u_read_file_fail++;
				continue;    
			}
			//魔术数异常标记
			bool bInvalidMagic = false;
			//数据是否反转
			bool bSwap = false;
			switch (fileheader.magic)
			{
				case 0xa1b2c3d4:
					break;
				case 0x1a2b3c4d:
					break;
				case 0xd4c3b2a1:
					bSwap = true;
					break;
				default:
					bInvalidMagic = true;
					break;
			}

			//异常魔术数
			if (bInvalidMagic)
			{
			    u_abnormal_file++;
				std::cout<<"Invalid Magic!"<<std::endl;
				continue;
			}


			while (true)
			{
			    //日志统计
			    u_pkt_totals++;

				//构建包体结构
				cell *ce = NULL;
				try
				{
					ce = new cell();
				}
				catch(std::bad_alloc)
				{
					continue;
				}
				//获取链路层协议
				setlinktype(fileheader.LinkType, ce);

				//读取包头
				ns_pcap::packet_header pkt_header;    
				if (fread((void*)&pkt_header, 1, sizeof(pkt_header), fp) <= 0)	    
				{
				    u_read_pkt_fail++;
					if (NULL == ce)
					{
						delete ce;
					}
					break;	
				}

				//pdu超过1514不处理
				if (ns_capture::cap_content_max_len < pkt_header.caplen)
				{
					fseek(fp, pkt_header.caplen, SEEK_CUR);
					printf("packet length over max processing length,ignore the packet!\n");
					u_abnormal_pkt++;
					if (NULL == ce)
					{
						delete ce;
					}
					continue;
				}

				//读取报文内容
				char cap_content[ns_capture::cap_content_max_len];
				if (fread((void*)cap_content, 1, pkt_header.caplen, fp) <= 0)
				{
				    u_read_pkt_fail++;
					if (NULL == ce)
					{
						delete ce;
					}
					break;	
				}

                //日志统计
                u_read_pkt_suc++;

				//拷贝数据体
				ce->p_str = (char*)_MEM_NEW_(pkt_header.caplen + 1);
				ce->u_len = pkt_header.caplen;
				_MEM_CPY(ce->p_str, cap_content, ce->u_len);
				_MEM_ZERO_(ce->p_str, ce->u_len + 1, ce->u_len);

				//入队
				if (RET::SUC != _queue.push(ce))
				{
					if (NULL != ce)
					{
						delete ce;
						ce = NULL;
					}
				}
			}

			//日志统计
			u_read_file_suc++;

			//关闭文件
			fclose(fp);

			//删除文件
			system(cmd);

			//删除首节点文件名
			names.pop_front();
			sleep(1);
		}
		sleep(1);
	}
}

//设置以太协议类型
void capture::setlinktype(uint32_t linktype, cell *ce)
{
	switch (linktype)	
	{
	    //BSD loopback devices, except for later OpenBSD 
		case ns_capture::bsd_linktype:    
			ce->u_protocoltype = ns_protocol::em_bsd_protocol;
			break;
		//Ethernet, and Linux loopback devices	
		case ns_capture::eth_linktype:
			ce->u_protocoltype = ns_protocol::em_ethernet_protocol;
			break;
		//802.5 Token Ring 	
		case ns_capture::token_ring_linktype:
		    ce->u_protocoltype = ns_protocol::em_token_protocol;
			break;
		//ARCnet	
		case ns_capture::arcnet_linktype:
		    ce->u_protocoltype = ns_protocol::em_arcnet_protocol;
			break;
		//SLIP	
		case ns_capture::slip_linktype:
		    ce->u_protocoltype = ns_protocol::em_slip_protocol;
			break;
		//PPP 	
		case ns_capture::ppp_linktype:
		    ce->u_protocoltype = ns_protocol::em_ppp_protocol;
			break;
		//FDDI	
		case ns_capture::fddi_linktype:
		    ce->u_protocoltype = ns_protocol::em_fddi_protocol;
			break;
		//LLC/SNAP-encapsulated ATM 	
		case ns_capture::llc_linktype:
		    ce->u_protocoltype = ns_protocol::em_llc_protocol;
			break;
		//“raw IP”, with no link	
		case ns_capture::rawip_linktype:
		    ce->u_protocoltype = ns_protocol::em_rawip_protocol;
			break;
		//BSD/OS SLIP	
		case ns_capture::bsd_slip_linktype:
		    ce->u_protocoltype = ns_protocol::em_bsd_slip_protocol;
			break;
		//BSD/OS PPP	
		case ns_capture::bsd_ppp_linktype:
		    ce->u_protocoltype = ns_protocol::em_bsd_ppp_protocol;
			break;
		//Cisco HDLC 	
		case ns_capture::cisco_linktype:
		    ce->u_protocoltype = ns_protocol::em_cisco_protocol;
			break;
		//802.11 	
		case ns_capture::eleven_linktype:
		    ce->u_protocoltype = ns_protocol::em_eleven_protocol;
			break;
		//later OpenBSD loopback devices (with the AF_value in network byte order) 	
		case ns_capture::openbsd_linktype:
		    ce->u_protocoltype = ns_protocol::em_openbsd_protocol;
			break;
		//special Linux “cooked” capture 	
		case ns_capture::linux_cooked_linktype:
		    ce->u_protocoltype = ns_protocol::em_linux_cooked_protocol;
			break;
        //LocalTalk 
		case ns_capture::localtalk_linktype:
		    ce->u_protocoltype = ns_protocol::em_localtalk_protocol;
			break;
		default:
			ce->u_protocoltype = ns_protocol::em_unknow_protocol;
			break;	    
	}
}

//获取主缓冲区
recyclequeue<cell> capture::get_queue()
{
	return _queue;
}

//写日志函数
void capture::write_log()
{
	uint64_t u_push_fail = _queue.lost();
    char *p_log = (char*)_MEM_NEW_(1024);
	snprintf(p_log, 1024, "CAPTURE MODULE STATISTICS:\n\
	read_file_mode:\n\
	read_file_totals:%d; read_file_suc:%d; read_file_fail:%d; abnormal_file_num:%d\n\
	read_pkt_tptals:%d; read_pkt_suc:%d; read_pkt_fail:%d; abnormal_pkt_num:%d\n\
	push_queue_fail:%d", u_file_totals, u_read_file_suc, u_read_file_fail, u_abnormal_file,
   	u_pkt_totals, u_read_pkt_suc, u_read_pkt_fail, u_abnormal_pkt, u_push_fail);

	std::cout<<p_log<<std::endl;					   
}
