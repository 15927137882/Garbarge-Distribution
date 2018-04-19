/* 垃圾类头文件 */

#pragma once
#include "Point.h"
#include <time.h>

class Rubbish
{
public:
	Rubbish();	//默认构造函数
	Rubbish(int x);	//构造函数
	const unsigned int GetID() { return id; }	//返回垃圾ID
	const clock_t GetCreateTime() { return create_time; }		//返回垃圾产生时刻
	const Point GetPosition() { return position; }	//返回垃圾位置
	void SetProcessTime(clock_t t) { process_time = t; }	//设置开始处理时刻
	void SetFinishTime(clock_t t) { finish_time = t; }	//设置完成处理时刻
	const clock_t GetProcessTime() { return process_time; }	//返回开始处理时刻
	const clock_t GetFinishTime() { return finish_time; }	//返回完成处理时刻
	friend bool rubbish_cmp(const Rubbish& a, const Rubbish& b);	//垃圾顺序比较函数
private:
	unsigned int id;	//垃圾ID
	Point position;		//垃圾位置
	clock_t create_time;	//垃圾产生时刻
	clock_t process_time;	//垃圾开始处理时刻
	clock_t finish_time;	//垃圾处理完成时刻
};