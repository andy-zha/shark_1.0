#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include "asmlib.h"
#include "dlist.h"
#include <pthread.h>
#include <stdlib.h>

//槽节点类
template<typename T>
class hashslot
{
    public:
		/**
		 * @brief 构造函数
		 */
		hashslot()
		{
			pthread_mutex_init(&mutex,NULL);
		}

		/**
		 * @brief 析构函数
		 */
		~hashslot()
		{
			pthread_mutex_destroy(&mutex);
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

		/**
		 * @brief 尝试上锁
		 */
		int32_t trylock()
		{
			return pthread_mutex_trylock(&mutex);
		}

		/**
		 * @brief 上锁
		 */
		void lock()
		{
			pthread_mutex_lock(&mutex);
		}

		/**
		 * @brief 去锁
		 */
		void unlock()
		{
			pthread_mutex_unlock(&mutex);
		}

    public:
        /**
         * @brief 业务链  
         */
        dlist<T> m_hash_list;
        
        /**
         * @brief 互斥锁
         */
        pthread_mutex_t mutex;
};

//哈希表操作类
template<typename T>
class hashtable
{
    public:
		/**
		 * @brief 构造函数
		 */
		hashtable()
		{
			m_hashslot = NULL;
		}

		/**
		 * @brief 析构函数
		 */
		~hashtable()
		{
			if (NULL != *m_hashslot)
			{
				delete [] *m_hashslot;
				m_hashslot = NULL;
			}

			if (NULL != m_hashslot)
			{
			    delete [] m_hashslot;
				m_hashslot = NULL;
			}
		}

		/**
		 * 
		 * @brief 哈希表初始化
		 *
		 * @prame uHashSize 哈希表大小
		 *
		 * return RET::SUCCESS 成功/RET::FAIL 失败
		 */
		int32_t hashtableinit(uint16_t num, uint32_t *usize)
		{
		    m_hashsize = new uint32_t [num];

			try
			{
				m_hashslot = (hashslot<T>**) new hashslot<T> [num];
			}
			catch(...)
			{
			    return RET::FAIL;
			}
			
			for (uint16_t uindex = 0; uindex < num; uindex++)
			{
			    m_hashsize[uindex] = usize[uindex];
			    m_hashslot[uindex] = (hashslot<T>*) new hashslot<T> [usize[uindex]];
			}

			printf("hashslot:%p\n", m_hashslot);
			printf("hashslot[0]:%p\n", m_hashslot[0]);
			printf("hashslot->m_hash_list._next:%p\n", m_hashslot[0]->m_hash_list._dlist_next);

			return RET::SUCCESS;
		}

		/**
		 * 
		 * @brief 查找槽节点
		 *
		 * @prame uHashKey 哈希值；pSlot 槽节点指针
		 *
		 * return RET::SUCCESS 成功/RET::FAIL 失败
		 */
		hashslot<T>* findhashtableslot(uint16_t num, uint32_t ukey)
		{
			//检测哈希key大小
			if (m_hashsize[num] < ukey)
			{
				assert(m_hashsize[num] >= ukey);
				return NULL;
			}

			return &m_hashslot[num][ukey];
		}

		/**
		 * 
		 * @brief 查找链
		 *
		 * @prame pSlot 槽节点指针; plist 链指针
		 *
		 * return RET::SUCCESS 成功/RET::FAIL 失败
		 */
		int32_t findhashtablelist(hashslot<T> *pslot, dlist<T> **plist)
		{
		    *plist = &pslot->m_hash_list;

			if (NULL == *plist)
			{
			    return RET::FAIL;
			}

			return RET::SUCCESS;
		}

	private:
		/**
		 * @brief 哈希槽
		 */
		hashslot<T> **m_hashslot;

		/**
		 * @brief 哈希大小
		 */
		uint32_t *m_hashsize;
};

#endif
