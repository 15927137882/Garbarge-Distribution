#pragma once
#include "Point.h"
#include <time.h>
#include <list>

class Worker
{
public:
	Worker(Rubbish r);	//构造函数
	const unsigned int GetWorkerID() { return worker_id; }	//返回环卫工ID
	void SetWorkerID(unsigned int x) { worker_id = x; }	//设置环卫工ID
	const bool GetState() { return busy; }	//返回环卫工工作状态
	void SetState(bool state) { busy = state; }	//设置环卫工工作状态
	void Allocate(Rubbish r) { now_rubbish = r; }	//分配垃圾
	Rubbish & FlushRubbish() { return now_rubbish; }	//更新垃圾信息
	Rubbish GetRubbish() { return now_rubbish; }	//返回正在处理垃圾
	const Point GetRubbishPosition() { return now_rubbish.GetPosition(); }	//返回正在处理垃圾的位置
	const unsigned int GetRubbishID() { return now_rubbish.GetID(); }	//返回环卫工正在处理垃圾ID
	const Point GetPosition() { return position; }		//返回环卫工位置信息
	void ShowPosition() const;	//显示环卫工位置
	void SetPosition(int x, int y);	//设置环卫工位置
	void SetPosition(const Point p);	//设置环卫工位置
	void SetEachWorkTime(clock_t t) { each_work_time = t; }	//设置每次处理的时间
	const clock_t GetEachWorkTime() { return each_work_time; }	//返回每次处理的时间
	const int GetTreatedNum() { return treated_num; }	//返回环卫工已经处理垃圾数
	void FlushTreatedNum() { treated_num++; }	//刷新环卫工已经处理的垃圾数
	void FlushTreatedID(int x);	//刷新环卫工已经处理的垃圾ID
	void FlushTotalWorkTime() { total_work_time += each_work_time; }	//刷新环卫工工作总时间
	const clock_t GetTotalWorkTime() { return total_work_time; }	//返回环卫工工作总时间
	void SetStartMovingTime() { start_moving_time = clock(); }	//设置开始移动环卫工的时间
	const clock_t GetStartMovingTime() { return start_moving_time; }
	void ShowTreatedID();		//显示环卫工处理所有垃圾的ID
	friend bool worker_cmp(const Worker& a, const Worker& b);	//环卫工顺序比较函数
	const int GetMinX(void) { return min_x; }
	const int GetMaxX(void) { return max_x; }
	const int GetMinY(void) { return min_y; }
	const int GetMaxY(void) { return max_y; }
	void SetRange(int x1, int y1, int x2, int y2);
private:
	unsigned int worker_id;		//工人ID
	bool busy;	//工作状态
	Rubbish now_rubbish;		//现在正在处理的垃圾
	Point position;	//工人当前位置
	int treated_num;	//已处理的垃圾数
	std::list<int> treated_rubbish_id;	//已处理的垃圾ID
	clock_t start_moving_time;	//环卫工开始移动的时间
	clock_t each_work_time;		//处理每个垃圾的工作时间
	clock_t total_work_time;	//处理垃圾的总共工作时间
	int min_x, max_x;		//横坐标范围
	int min_y, max_y;		//纵坐标范围
};

void InitWorker();
int abs_distance(const Point a, const Point b);