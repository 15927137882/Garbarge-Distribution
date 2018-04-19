#include "Simulink.h"



std::list<Rubbish> untreated_list;		//外部链接性全局变量，未处理完成垃圾链表
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
	
	clock_t start = clock();       //启动计时

	thread thread_create_rubbish(create_rubbish);	//创建垃圾产生线程
	thread thread_allocate_rubbish(allocate_rubbish);	//创建垃圾分配线程
	thread thread_treate_rubbish(treate_rubbish);		//创建垃圾处理线程
	
	thread_create_rubbish.join();
	thread_allocate_rubbish.join();
	thread_treate_rubbish.join();
	
	clock_t stop = clock();         //结束计时
	system_run_time = (stop - start) / (double)CLOCKS_PER_SEC;

	thread thread_stastic_analyze(data_analyze);		//创建统计分析线程
	thread_stastic_analyze.join();


	cout << "\n系统运行时间: " << system_run_time<<endl;


	return 0;
}

