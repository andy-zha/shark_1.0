/*****************************************************
copyright (C), Anhui University of Tecnology
File name：asmlib.h
Author: Andy  Version:0.1 Date: 2019-05-04 14:33
Description：
Funcion List: 
*****************************************************/
#ifndef _ASMLIB_H_
#define _ASMLIB_H_

#include <iostream>
#include <cstdio>

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;

namespace RET
{
    //返回成功
    const int32_t SUC = 0;
    //返回失败	
    const int32_t FAIL = -1;
}

#endif
