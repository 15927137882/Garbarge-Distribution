/* 并行线程函数头文件 */

#pragma once
#include <iostream>

void create_rubbish();	//产生垃圾线程
void allocate_rubbish();	//垃圾调度线程
void treate_rubbish();		//垃圾处理线程
void work_moving();		//工人移动线程
void data_analyze();	//统计分析线程