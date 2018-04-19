#include "Simulink.h"
#include <stdlib.h>

Rubbish::Rubbish()
{
	static unsigned int count = 1;	//�������Ծ�̬���������������ڿɼ���������Ϊ�������򳤶�
	id = count;
	srand((unsigned)time(NULL));
	position.x = rand() % 90;		//ˮƽ��Χ[0,89]
	position.y = rand() % 90;		//��ֱ��Χ[0,89]
	create_time = clock();
	process_time = 0;
	finish_time = 0;
	count++;
}

Rubbish::Rubbish(int x)
{
	id = 0;
	position.x = -1;
	position.y = -1;		
	create_time = 0;
	process_time = 0;
	finish_time = 0;
}

bool rubbish_cmp(const Rubbish& a, const Rubbish& b)
{
	return a.id < b.id;
}