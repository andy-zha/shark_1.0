/*****************************************************
 copyright (C), Anhui University of Tecnology
 File name：main.cpp
 Author: Andy  Version:0.1 Date: 2019-05-10 23:00
 Description：主程序入口
 Funcion List: 
*****************************************************/
#include "capture.h"

int32_t main()
{
    capture::getinstance().read_packet();

    return RET::SUC;
}
