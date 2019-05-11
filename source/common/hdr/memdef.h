/*****************************************************
copyright (C), Anhui University of Tecnology
File name：memdef.h
Author: Andy  Version:0.1 Date: 2019-05-04 14:37
Description：
Funcion List: 
*****************************************************/
#ifndef _MEMDEF_H_
#define _MEMDEF_H_

#include <iostream>
#include <cstdio>
#include <cstring>

#define _MEM_NEW_(len) _MEM_SYS_::malloc(len)
#define _MEM_DEL_(p) _MEM_SYS_::free(p)
#define _MEM_CPY(des, src, len) _MEM_SYS_::memcpy(des, src, len)
#define _MEM_ZERO_(str, len, position) _MEM_SYS_::memzero(str, len, position)

namespace _MEM_SYS_
{
    //分配内存
    static inline void * malloc (size_t size)
    {
        void *buf;

		try
		{
			buf = ::new char[size];
		}
		catch(...)
		{
			throw std::bad_alloc();
		}

		return buf;
	}

	//释放内存 
	static inline void free (void *buf)
	{
		delete [] (char*)buf;
	}

    //内存拷贝
    static inline void memcpy(void *dest, void *src, uint32_t ulen)
    {
#ifdef _DEBUG_VERSION_
		assert(NULL != dest);
		assert(NULL != src);
#endif
        while (ulen--)		
		{
		    *(char*)dest = *(char*)src;
			dest = (char*)dest + 1;

#ifdef _DEBUG_VERSION_
			assert(NULL != dest);
#endif

			src = (char*)src + 1;

#ifdef _DEBUG_VERSION_
			assert(NULL != src);
#endif
		}
    }

    //封结尾函数
    static inline void memzero(char *str, uint32_t ulen, uint32_t position)
    {
#ifdef _DEBUG_VERSION_
		assert(NULL != str);
		assert(position < ulen);
#endif
		str[position] = '\0';
    }
}

//释放指针
static inline void free_ptr(char *p)
{
    if (NULL != p){
		_MEM_DEL_(p);
		p = NULL;
	}	    
}

#endif
