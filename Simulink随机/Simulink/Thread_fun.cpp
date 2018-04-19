#include "Simulink.h"
#include <list>
#include <thread>
#include <chrono>

extern std::list<Rubbish> untreated_list;
extern std::list<Rubbish> treating_list;
extern std::list<Rubbish> treated_list;

extern std::list<Worker> worker_array;
extern std::list<Worker> worker_array_moving;

extern std::mutex mtx;

using std::cout;
using std::endl;

void create_rubbish()
{
	static unsigned int id = 1;
	while (1)
	{
		{
			if (id>TOTAL_RUBBISH) break;     //检查产生垃圾总数是否达到预设值
			if (id <= TOTAL_RUBBISH)
			{
				Rubbish rubbish;		//产生一个垃圾

				/* 输出产生垃圾的信息 */
				// mtx.lock();
				cout << "A rubbish has been created: ID "
					<< rubbish.GetID() << " create_time:" << rubbish.GetCreateTime() / (double)CLOCKS_PER_SEC
					<< " seconds.\nPosition(" << rubbish.GetPosition().GetX()
					<< ',' << rubbish.GetPosition().GetY() << ')' << endl << endl;
				 //mtx.unlock();

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));     //平均过1秒，垃圾就会被发现，并报告
				
				/* 输出垃圾被发现的信息 */
				//mtx.lock();
				cout << "This rubbish(ID:" << rubbish.GetID() << ") has been found and reported\n\n";
				//mtx.unlock();
				
				untreated_list.push_back(rubbish);    //将产生垃圾加入未处理垃圾链表

				id++;       //垃圾产生数加1
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));     //程序休眠1.5秒，产生下一个垃圾
			}
		}
	}
	//mtx.lock();
	cout << "产生进程结束" << endl << endl;
	//mtx.unlock();
}

void allocate_rubbish()
{	
	std::list<Worker>::iterator it;
	while (1)
	{
		{
			if (treated_list.size() == TOTAL_RUBBISH) break;     //检查处理完成垃圾总数是否达到预设值
			if (untreated_list.size() > 0)
			{
				//随机调度算法
				for (it = worker_array.begin(); it!=worker_array.end();it++)            //遍历工人数组，为该工人匹配垃圾
				{
					srand((unsigned)time(NULL));
					if ((*it).GetState() == false && rand()%2 == 0)      //找到第一个空闲状态的工人
					{
						mtx.lock();
						
						Rubbish temp = untreated_list.front();      //取出一个未处理垃圾
						untreated_list.pop_front();                 //将该垃圾从未处理垃圾链表中删除

						(*it).SetState(true);
						(*it).Allocate(temp);
						(*it).SetStartMovingTime();
						worker_array_moving.push_back((*it));
						cout << "worker[" << (*it).GetWorkerID() << "] moving to " << (*it).GetRubbishID() << " rubbish.\n\n";
						worker_array.erase(it);
						mtx.unlock();
						
						
						static bool moving_thread_flag = false;
						if (moving_thread_flag == false)
						{
							std::thread thread_worker_moving(work_moving);
							thread_worker_moving.detach();
							moving_thread_flag = true;
						}

						break;  //分配成功后，直接退出
					}
				}
			}
		}
	}
	//mtx.lock();
	cout << "分配进程结束" << endl << endl;
	//mtx.unlock();
}


void treate_rubbish()
{
	std::list<Worker>::iterator worker_it;
	std::list<Rubbish>::iterator it;
	while (1)
	{
		if (treated_list.size() == TOTAL_RUBBISH) break;     //检查处理完成垃圾总数是否达到预设值
		if (treating_list.size() > 0)
		{
			for (worker_it = worker_array.begin(); worker_it != worker_array.end(); worker_it++)                    //遍历工人数组
			{
				if ((*worker_it).GetState() == true)    //找到正在工作状态的工人
				{

					for (it = treating_list.begin(); it != treating_list.end();)      //找到该工人正在处理的垃圾
					{
						if ((*it).GetID() == (*worker_it).GetRubbishID())
						{
							if (clock() - (*it).GetProcessTime() >= TIME_NEED_TO_FINISH * CLOCKS_PER_SEC)       //判断处理时间是否达到预设值
							{
								mtx.lock();
								(*worker_it).SetState(false);           //工人状态变为闲
								(*worker_it).FlushTreatedNum();          //该工人处理的垃圾数加1
								(*worker_it).FlushTreatedID((*it).GetID()); //将处理的垃圾id加入该工人已处理垃圾id链表
								(*it).SetFinishTime(clock());            //记录完成时刻
								(*worker_it).SetEachWorkTime((*it).GetFinishTime() - (*worker_it).GetEachWorkTime());    //记录工人处理每个垃圾的时间
								(*worker_it).FlushTotalWorkTime();          //统计每个工人工作总时间
								Rubbish temp = (*it);                   //将该垃圾从正在处理链表中取出
								treated_list.push_back(temp);           //加入已处理完成链表
								it = treating_list.erase(it);
								mtx.unlock();

								mtx.lock();
								/* 输出相关处理完成信息 */
								cout << "number " << temp.GetID() << " rubbish has been treated.\nfinish time "
									<< temp.GetFinishTime() / (double)CLOCKS_PER_SEC << endl << endl;

								mtx.unlock();
								worker_it = worker_array.begin();
							}
						}
						else it++;

					}
				}
			}
		}
	}
	mtx.lock();
	cout << "处理进程结束" << endl << endl;
	mtx.unlock();
}


void work_moving()
{
	std::list<Worker>::iterator it;
	while (1)
	{
		if (treated_list.size() == TOTAL_RUBBISH) break;
		if (worker_array_moving.size() > 0)
		{
			for (it = worker_array_moving.begin(); it != worker_array_moving.end();)
			{
				if (clock() - (*it).GetStartMovingTime() >= (100 * abs_distance((*it).GetPosition(), (*it).GetRubbishPosition())))
				{
					mtx.lock();
					(*it).SetPosition((*it).GetRubbishPosition());

					(*it).FlushRubbish().SetProcessTime(clock());                  //记录开始处理时间
					(*it).SetEachWorkTime((*it).GetRubbish().GetProcessTime());
					treating_list.push_back((*it).GetRubbish());
					mtx.unlock();

					mtx.lock();
					/* 输出相关处理信息 */
					cout << "worker[" << (*it).GetWorkerID() << "] treating number " << (*it).GetRubbishID() << " rubbish.\n";
					cout << "worker Position:";
					(*it).ShowPosition();
					cout << endl << endl;
					mtx.unlock();

					worker_array.push_back(*it);
					it = worker_array_moving.erase(it);

				}
				else it++;
			}
		}
	}
}


void data_analyze()
{
	//垃圾信息统计
	std::list<Rubbish>::iterator it;
	treated_list.sort(rubbish_cmp);
	double total_responsive_time = 0;       //总响应时间(开始处理时刻-发现时刻)
	double total_live_time = 0;             //总存活时间(完成时刻-发现时刻)
	double total_process_time = 0;             //总处理时间(完成时刻-开始处理时刻)
	for (it = treated_list.begin(); it != treated_list.end(); it++)
	{
		double find_time = 0;       //发现时刻
		double start_process_time = 0;  //开始处理时刻
		double finish_time = 0;     //完成时刻
		double responsive_time = 0; //响应时间
		double process_time = 0;    //处理时间
		double live_time = 0;       //存活时间

		find_time = (*it).GetCreateTime() / (double)CLOCKS_PER_SEC;
		start_process_time = (*it).GetProcessTime() / (double)CLOCKS_PER_SEC;
		finish_time = (*it).GetFinishTime() / (double)CLOCKS_PER_SEC;
		responsive_time = start_process_time - find_time;
		process_time = finish_time - start_process_time;
		live_time = finish_time - find_time;

		total_responsive_time += responsive_time;
		total_process_time += process_time;
		total_live_time += live_time;

		cout << (*it).GetID() << " rubbish:\n"
			<< "find time: " << find_time << " seconds\n"
			<< "start process time: " << start_process_time << " seconds\n"
			<< "process time: " << process_time << " seconds\n"
			<< "responsive time: " << responsive_time << " seconds\n"
			<< "live time: " << live_time << " seconds\n\n\n";
	}

	double average_responsive_time = total_responsive_time / treated_list.size();
	double average_live_time = total_live_time / treated_list.size();
	double average_process_time = total_process_time / treated_list.size();

	cout << "average responsive time: " << average_responsive_time << endl
		<< "average process time: " << average_process_time << endl
		<< "average live time: " << average_live_time << endl<<endl<<endl;



	extern double system_run_time;
	std::list<Worker>::iterator worker_it;
	worker_array.sort(worker_cmp);
	for (worker_it = worker_array.begin(); worker_it != worker_array.end(); worker_it++)        //统计每个工人的数据
	{
		cout << "worker " << (*worker_it).GetWorkerID() << " has treated " << (*worker_it).GetTreatedNum()
			 << " rubbish" << endl;       //输出每个工人处理垃圾个数

		cout << "treated rubbish id: ";       //输出每个工人处理的垃圾id

		(*worker_it).ShowTreatedID();
		double work_total_time = (*worker_it).GetTotalWorkTime() / (double)CLOCKS_PER_SEC;    //输出每个工人的工作总时间
		cout << "work total time: " << work_total_time << " seconds";

		cout << endl;
		double work_strength = (work_total_time / system_run_time) * 100;       //输出每个工人的工作强度
		cout << "work strength: " << work_strength << " %";


		cout << endl << endl << endl;
	}
}

