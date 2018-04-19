/* ������ͷ�ļ� */

#pragma once
#include "Point.h"
#include <time.h>

class Rubbish
{
public:
	Rubbish();	//Ĭ�Ϲ��캯��
	Rubbish(int x);	//���캯��
	const unsigned int GetID() { return id; }	//��������ID
	const clock_t GetCreateTime() { return create_time; }		//������������ʱ��
	const Point GetPosition() { return position; }	//��������λ��
	void SetProcessTime(clock_t t) { process_time = t; }	//���ÿ�ʼ����ʱ��
	void SetFinishTime(clock_t t) { finish_time = t; }	//������ɴ���ʱ��
	const clock_t GetProcessTime() { return process_time; }	//���ؿ�ʼ����ʱ��
	const clock_t GetFinishTime() { return finish_time; }	//������ɴ���ʱ��
	friend bool rubbish_cmp(const Rubbish& a, const Rubbish& b);	//����˳��ȽϺ���
private:
	unsigned int id;	//����ID
	Point position;		//����λ��
	clock_t create_time;	//��������ʱ��
	clock_t process_time;	//������ʼ����ʱ��
	clock_t finish_time;	//�����������ʱ��
};