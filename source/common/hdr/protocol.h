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
    //协议值
    typedef enum protocol
    {
		em_protocol_start = -1,                 //协议值保护
		em_bsd_protocol = 0,                    //bsd协议
		em_ethernet_protocol = 1,               //eth协议
		em_token_protocol,                      //802.5 Token Ring
		em_arcnet_protocol,                     //ARCnet
		em_slip_protocol,                       //SLIP
        em_ppp_protocol,                        //PPP
		em_fddi_protocol,                       //FDDI
		em_llc_protocol,                        //LLC/SNAP-encapsulated
		em_rawip_protocol,                      //“raw IP”, with no link
		em_bsd_slip_protocol,                   //BSD/OS SLIP
		em_bsd_ppp_protocol,                    //BSD/OS PPP
		em_cisco_protocol,                      //Cisco HDLC
		em_eleven_protocol,                     //802.11
		em_openbsd_protocol,                    //later OpenBSD loopback devices (with the AF_value in network byte order)
		em_linux_cooked_protocol,               //special Linux “cooked” capture
		em_localtalk_protocol,                  //LocalTalk
		em_unknow_protocol,                     //未知协议

		em_protocol_end                         //协议值保护
    }em_protocol;	    
}

#endif
