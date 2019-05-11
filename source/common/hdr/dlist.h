/*************************************************************
*********************
*   Copyright(C++),2018,Demo
*   filename: DList.h
*   Author: Jobs
*   Version: 1.0.0
*   Date: 2018/7/5
*   Description: 定义双向链表
*************************************************************/
#ifndef _DLIST_H_
#define _DLIST_H_

#pragma once

#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include "asmlib.h"
#include "memdef.h"


template <typename type>
class dlist
{
	public:
		/*
		 * @breif 构造函数
		 */
		dlist():_dlist_prev(this),_dlist_next(this),_dlist_len(0)
		{
		}

		/*
		 * @breif 构造函数
		 */
		~dlist()
		{
			dlist<type> *pnode = NULL;

			while(RET::SUC == forward_traver(&pnode))
			{
				pop_node(pnode);
			}
		}

		/*
		 * @breif 删除节点
		 */
		int32_t remove_node(dlist<type> *pcurnode)
		{
		    if (RET::SUC == pop_node(pcurnode))
			{
			    if (NULL != pcurnode)
				{
					delete pcurnode;
					pcurnode = NULL;
				}
			}
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

		/*
		 * @breif 弹出节点
		 */
		int32_t pop_node(dlist<type> *pcurnode)
		{
			assert(0 < _dlist_len);
			assert(NULL != pcurnode);

			pcurnode->_dlist_prev->_dlist_next = pcurnode->_dlist_next;
			pcurnode->_dlist_next->_dlist_prev = pcurnode->_dlist_prev;

			pcurnode->_dlist_prev = pcurnode;
			pcurnode->_dlist_next = pcurnode;
 
			_dlist_len--;

			return RET::SUC;
		}
    
		/*
		 * @breif 前向添加新的节点
		 */
		int32_t prev_addnode(dlist<type> *pnewnode,dlist<type> *pcurnode)
		{
			assert(NULL != pnewnode);
			assert(NULL != pcurnode);

			pcurnode->_dlist_prev->_dlist_next = pnewnode;
			pnewnode->_dlist_prev = pcurnode->_dlist_prev;
			pcurnode->_dlist_prev = pnewnode;
			pnewnode->_dlist_next = pcurnode;

			_dlist_len++;

			return RET::SUC;
		}
    
		/*
		 * @breif 后向添加新的节点
		 */
		void next_addnode(dlist<type> *pnewnode,dlist<type> *pcurnode)
		{
			assert(0 < _dlist_len);
			assert(NULL != pnewnode);
			assert(NULL != pcurnode);

			pcurnode->_dlist_next->_dlist_prev = pnewnode;
			pnewnode->_dlist_next = pcurnode->_dlist_next;
			pcurnode->_dlist_next = pnewnode;
			pnewnode->_dlist_prev = pcurnode;

			_dlist_len++;

			return RET::SUC;
		}
    
		/*
		 * @breif 头节点后添加新的节点
		 */
		void head_addnode(dlist<type> *pnewnode)
		{
		    assert(NULL != pnewnode);
			next_addnode(pnewnode,this);
		}
    
		/*
		 * @breif 尾节点后添加新的节点
		 */
		void tail_addnode(dlist<type> *pnewnode)
		{
			assert(NULL != pnewnode);
			prev_addnode(pnewnode,this);
		}

		/*
		 * @breif 后序遍历所有节点
		 */
		int32_t forward_traver(dlist<type> **pcurnode)
		{
			if(NULL == *pcurnode)
			{
				*pcurnode = _dlist_next;
			}
			else
			{
				*pcurnode = (*pcurnode)->_dlist_next;
			}

			return (*pcurnode != this) ? RET::SUC : RET::FAIL; 
		}

		/*
		 * @breif 前向遍历所有节点
		 */
		int32_t backward_traver(dlist<type> **pcurnode)
		{
			if(NULL == pcurnode)
			{
				*pcurnode = _dlist_prev;
			}
			else
			{
				*pcurnode = (*pcurnode)->_dlist_prev;
			}

			return (*pcurnode == this) ? RET::FAIL : RET::SUC; 
		}

		/**
		 * @brief 销毁链表
		 */
		int32_t destroy_list()
		{
		    dlist<type> *pnode = NULL;
			while (RET::SUC == forward_traver(&pnode))
			{
			    remove_node(pnode);
			}
		}

		/*
		 * @breif 获取链表长度
		 */
		int32_t size() const
		{
			return _dlist_len;
		}

	public:
		/*
		 * @breif 公有数据成员
		 */
		type m_dlist_data;

	//private:
	public:
		/*
		 * @breif 前向指针
		 */
		dlist<type> * _dlist_prev;

		/*
		 * @breif 后向指针
		 */
		dlist<type> * _dlist_next;


		/*
		 * @breif 双向链表长度
		 */
		int32_t _dlist_len;
};

#endif
