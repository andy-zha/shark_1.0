/*****************************************************
copyright (C), Anhui University of Tecnology
File name：capture.cpp
Author: Andy  Version:0.1 Date: 2019-05-04 15:07
Description：
Funcion List: 
*****************************************************/

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <list>
#include <assert.h>
#include "capture.h"

capture::capture()
{

}

capture::~capture()
{
}

int32_t capture::init()
{
    _mode.dev = (char*)_MEM_NEW_(10);
    sprintf(_mode.dev, "lo");
    _mode.snaplen = 1514;
    _mode.promisc = 1;    
    _mode.to_ms = 0;
    return RET::SUC;	
}

int32_t capture::start()
{
    return RET::SUC;	
}

void capture::capture_packet()
{
    while(true)	
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

void capture::read_packet()
{
    DIR *dir;
    std::list<std::string> names;
    while (true)
    {
	//打开读包路径    
        dir = opendir(ns_capture::read_packet_path);
	if (NULL == dir)
	{
	    std::cout<<"capture error:dir null!"<<std::endl;
            assert(NULL != dir);	    
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

            //构建删除文件名指令
	    char cmd[100];
	    snprintf(cmd, 100, "rm -rf %s", pfile);

	    //打开文件
	    FILE *fp = fopen(pfile, "r");
	    if (NULL== fp)
	    {
		continue;    
	    }

	    //读取文件头
	    ns_pcap::file_header fileheader;
	    if (fread((void*)&fileheader, 1, sizeof(fileheader), fp) <= 0)
	    {
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
	    }

	    //异常魔术数
	    if (bInvalidMagic)
	    {
	        std::cout<<"Invalid Magic!"<<std::endl;
		continue;
	    }


	    while (true)
	    {
		//构建包体结构
		cell ce;
		//获取链路层协议
		setlinktype(fileheader.LinkType, ce);

		//读取包头
		ns_pcap::packet_header pkt_header;    
	        if (fread((void*)&pkt_header, 1, sizeof(pkt_header), fp) <= 0)	    
		{
		    break;	
		}

		//pdu超过1514不处理
		if (ns_capture::cap_content_max_len < pkt_header.caplen)
		{
	            fseek(fp, pkt_header.caplen, SEEK_CUR);
		    printf("packet length over maximum processing length,ignore the packet!\n");
		    continue;
		}

		//读取报文内容
		char cap_content[ns_capture::cap_content_max_len];
		if (fread((void*)cap_content, 1, pkt_header.caplen, fp) <= 0)
		{
		    break;	
		}

		//拷贝数据体
		ce.p_str = (char*)_MEM_NEW_(pkt_header.caplen + 1);
		ce.u_len = pkt_header.caplen;
	        _MEM_CPY(ce.p_str, cap_content, ce.u_len);
		_MEM_ZERO_(ce.p_str, ce.u_len + 1, ce.u_len);
	    }

            //关闭文件
	    fclose(fp);

	    //删除文件
	    system(cmd);

	    //删除首节点文件名
	    names.pop_front();
	}
    }
}

void capture::setlinktype(uint32_t linktype, cell &ce)
{
    switch (linktype)	
    {
        case 0:    
	    ce.u_protocoltype = ns_protocol::em_bsd_protocol;
            break;
        case 1:
            ce.u_protocoltype = ns_protocol::em_ethernet_protocol;
            break;
        default:
            ce.u_protocoltype = ns_protocol::em_unknow_protocol;
            break;	    
    }
}
