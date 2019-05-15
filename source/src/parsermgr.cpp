#include "parsermgr.h"

parserbase **parsermgr::_parser = NULL;

parsermgr::parsermgr()
{}

parsermgr::~parsermgr()
{
    if (NULL != _parser)
	{
	    delete [] _parser;
		_parser = NULL;
	}
}

int32_t parsermgr::init()
{
    try
	{
        _parser = new parserbase* [ns_protocol::em_unknow_protocol];
	}
	catch(std::bad_alloc)
	{
	    return RET::FAIL;
	}

	//以太2协议解析器实例化
	eth2_parser *eth2 = new eth2_parser();
	_parser[ns_protocol::em_ethernet_protocol] = eth2;
	if (RET::SUC != _parser[ns_protocol::em_ethernet_protocol]->init())
	{
	    return RET::FAIL;
	}

    return RET::SUC;
}
