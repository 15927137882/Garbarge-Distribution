#pragma once
#include "Point.h"
#include <time.h>
#include <list>

class Worker
{
public:
	Worker(Rubbish r);	//���캯��
	const unsigned int GetWorkerID() { return worker_id; }	//���ػ�����ID
	void SetWorkerID(unsigned int x) { worker_id = x; }	//���û�����ID
	const bool GetState() { return busy; }	//���ػ���������״̬
	void SetState(bool state) { busy = state; }	//���û���������״̬
	void Allocate(Rubbish r) { now_rubbish = r; }	//��������
	Rubbish & FlushRubbish() { return now_rubbish; }	//����������Ϣ
	Rubbish GetRubbish() { return now_rubbish; }	//�������ڴ�������
	const Point GetRubbishPosition() { return now_rubbish.GetPosition(); }	//�������ڴ���������λ��
	const unsigned int GetRubbishID() { return now_rubbish.GetID(); }	//���ػ��������ڴ�������ID
	const Point GetPosition() { return position; }		//���ػ�����λ����Ϣ
	void ShowPosition() const;	//��ʾ������λ��
	void SetPosition(int x, int y);	//���û�����λ��
	void SetPosition(const Point p);	//���û�����λ��
	void SetEachWorkTime(clock_t t) { each_work_time = t; }	//����ÿ�δ����ʱ��
	const clock_t GetEachWorkTime() { return each_work_time; }	//����ÿ�δ����ʱ��
	const int GetTreatedNum() { return treated_num; }	//���ػ������Ѿ�����������
	void FlushTreatedNum() { treated_num++; }	//ˢ�»������Ѿ������������
	void FlushTreatedID(int x);	//ˢ�»������Ѿ����������ID
	void FlushTotalWorkTime() { total_work_time += each_work_time; }	//ˢ�»�����������ʱ��
	const clock_t GetTotalWorkTime() { return total_work_time; }	//���ػ�����������ʱ��
	void SetStartMovingTime() { start_moving_time = clock(); }	//���ÿ�ʼ�ƶ���������ʱ��
	const clock_t GetStartMovingTime() { return start_moving_time; }
	void ShowTreatedID();		//��ʾ��������������������ID
	friend bool worker_cmp(const Worker& a, const Worker& b);	//������˳��ȽϺ���
private:
	unsigned int worker_id;		//����ID
	bool busy;	//����״̬
	Rubbish now_rubbish;		//�������ڴ��������
	Point position;	//���˵�ǰλ��
	int treated_num;	//�Ѵ����������
	std::list<int> treated_rubbish_id;	//�Ѵ��������ID
	clock_t start_moving_time;	//��������ʼ�ƶ���ʱ��
	clock_t each_work_time;		//����ÿ�������Ĺ���ʱ��
	clock_t total_work_time;	//�����������ܹ�����ʱ��
};

void InitWorker();
int abs_distance(const Point a, const Point b);