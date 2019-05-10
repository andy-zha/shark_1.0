/*****************************************************
copyright (C), Anhui University of Tecnology
File name：protocol.h
Author: Andy  Version:0.1 Date: 2019-05-04 22:40
Description：
Funcion List: 
*****************************************************/
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <iostream>

namespace ns_protocol
{
    typedef enum protocol
    {
	em_protocol_start = -1,                 //协议值保护
        em_bsd_protocol = 0,                    //bsd协议
	em_ethernet_protocol = 1,               //eth协议
	em_unknow_protocol,                     //未知协议

	em_protocol_end                         //协议值保护
    }em_protocol;	    
}

#endif
