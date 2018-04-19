#include "Simulink.h"



std::list<Rubbish> untreated_list;		//�ⲿ������ȫ�ֱ�����δ���������������
std::list<Rubbish> treating_list;
std::list<Rubbish> treated_list;

std::list<Worker> worker_array;
std::list<Worker> worker_array_moving;

std::mutex mtx;

double system_run_time;

int main()
{
	using namespace std;

	InitWorker();

	/*list<Worker>::iterator it;
	for (it = worker_array.begin(); it != worker_array.end(); it++)
	{
		cout << "[" << (*it).GetMinX() << "," << (*it).GetMinY() << "]-[" << (*it).GetMaxX() << "," << (*it).GetMaxY() << "]" << endl;
	}*/
	
	clock_t start = clock();       //������ʱ

	thread thread_create_rubbish(create_rubbish);	//�������������߳�
	thread thread_allocate_rubbish(allocate_rubbish);	//�������������߳�
	thread thread_treate_rubbish(treate_rubbish);		//�������������߳�
	
	thread_create_rubbish.join();
	thread_allocate_rubbish.join();
	thread_treate_rubbish.join();
	
	clock_t stop = clock();         //������ʱ
	system_run_time = (stop - start) / (double)CLOCKS_PER_SEC;

	thread thread_stastic_analyze(data_analyze);		//����ͳ�Ʒ����߳�
	thread_stastic_analyze.join();


	cout << "\nϵͳ����ʱ��: " << system_run_time<<endl;


	return 0;
}

