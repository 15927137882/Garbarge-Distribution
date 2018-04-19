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
			if (id>TOTAL_RUBBISH) break;     //���������������Ƿ�ﵽԤ��ֵ
			if (id <= TOTAL_RUBBISH)
			{
				Rubbish rubbish;		//����һ������

				/* ���������������Ϣ */
				// mtx.lock();
				cout << "A rubbish has been created: ID "
					<< rubbish.GetID() << " create_time:" << rubbish.GetCreateTime() / (double)CLOCKS_PER_SEC
					<< " seconds.\nPosition(" << rubbish.GetPosition().GetX()
					<< ',' << rubbish.GetPosition().GetY() << ')' << endl << endl;
				 //mtx.unlock();

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));     //ƽ����1�룬�����ͻᱻ���֣�������
				
				/* ������������ֵ���Ϣ */
				//mtx.lock();
				cout << "This rubbish(ID:" << rubbish.GetID() << ") has been found and reported\n\n";
				//mtx.unlock();
				
				untreated_list.push_back(rubbish);    //��������������δ������������

				id++;       //������������1
				std::this_thread::sleep_for(std::chrono::milliseconds(1500));     //��������1.5�룬������һ������
			}
		}
	}
	//mtx.lock();
	cout << "�������̽���" << endl << endl;
	//mtx.unlock();
}

void allocate_rubbish()
{	
	std::list<Worker>::iterator it;
	while (1)
	{
		{
			if (treated_list.size() == TOTAL_RUBBISH) break;     //��鴦��������������Ƿ�ﵽԤ��ֵ
			if (untreated_list.size() > 0)
			{
				//��������㷨
				for (it = worker_array.begin(); it!=worker_array.end();it++)            //�����������飬Ϊ�ù���ƥ������
				{
					srand((unsigned)time(NULL));
					if ((*it).GetState() == false && rand()%2 == 0)      //�ҵ���һ������״̬�Ĺ���
					{
						mtx.lock();
						
						Rubbish temp = untreated_list.front();      //ȡ��һ��δ��������
						untreated_list.pop_front();                 //����������δ��������������ɾ��

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

						break;  //����ɹ���ֱ���˳�
					}
				}
			}
		}
	}
	//mtx.lock();
	cout << "������̽���" << endl << endl;
	//mtx.unlock();
}


void treate_rubbish()
{
	std::list<Worker>::iterator worker_it;
	std::list<Rubbish>::iterator it;
	while (1)
	{
		if (treated_list.size() == TOTAL_RUBBISH) break;     //��鴦��������������Ƿ�ﵽԤ��ֵ
		if (treating_list.size() > 0)
		{
			for (worker_it = worker_array.begin(); worker_it != worker_array.end(); worker_it++)                    //������������
			{
				if ((*worker_it).GetState() == true)    //�ҵ����ڹ���״̬�Ĺ���
				{

					for (it = treating_list.begin(); it != treating_list.end();)      //�ҵ��ù������ڴ��������
					{
						if ((*it).GetID() == (*worker_it).GetRubbishID())
						{
							if (clock() - (*it).GetProcessTime() >= TIME_NEED_TO_FINISH * CLOCKS_PER_SEC)       //�жϴ���ʱ���Ƿ�ﵽԤ��ֵ
							{
								mtx.lock();
								(*worker_it).SetState(false);           //����״̬��Ϊ��
								(*worker_it).FlushTreatedNum();          //�ù��˴������������1
								(*worker_it).FlushTreatedID((*it).GetID()); //�����������id����ù����Ѵ�������id����
								(*it).SetFinishTime(clock());            //��¼���ʱ��
								(*worker_it).SetEachWorkTime((*it).GetFinishTime() - (*worker_it).GetEachWorkTime());    //��¼���˴���ÿ��������ʱ��
								(*worker_it).FlushTotalWorkTime();          //ͳ��ÿ�����˹�����ʱ��
								Rubbish temp = (*it);                   //�������������ڴ���������ȡ��
								treated_list.push_back(temp);           //�����Ѵ����������
								it = treating_list.erase(it);
								mtx.unlock();

								mtx.lock();
								/* �����ش��������Ϣ */
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
	cout << "������̽���" << endl << endl;
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

					(*it).FlushRubbish().SetProcessTime(clock());                  //��¼��ʼ����ʱ��
					(*it).SetEachWorkTime((*it).GetRubbish().GetProcessTime());
					treating_list.push_back((*it).GetRubbish());
					mtx.unlock();

					mtx.lock();
					/* �����ش�����Ϣ */
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
	//������Ϣͳ��
	std::list<Rubbish>::iterator it;
	treated_list.sort(rubbish_cmp);
	double total_responsive_time = 0;       //����Ӧʱ��(��ʼ����ʱ��-����ʱ��)
	double total_live_time = 0;             //�ܴ��ʱ��(���ʱ��-����ʱ��)
	double total_process_time = 0;             //�ܴ���ʱ��(���ʱ��-��ʼ����ʱ��)
	for (it = treated_list.begin(); it != treated_list.end(); it++)
	{
		double find_time = 0;       //����ʱ��
		double start_process_time = 0;  //��ʼ����ʱ��
		double finish_time = 0;     //���ʱ��
		double responsive_time = 0; //��Ӧʱ��
		double process_time = 0;    //����ʱ��
		double live_time = 0;       //���ʱ��

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
	for (worker_it = worker_array.begin(); worker_it != worker_array.end(); worker_it++)        //ͳ��ÿ�����˵�����
	{
		cout << "worker " << (*worker_it).GetWorkerID() << " has treated " << (*worker_it).GetTreatedNum()
			 << " rubbish" << endl;       //���ÿ�����˴�����������

		cout << "treated rubbish id: ";       //���ÿ�����˴��������id

		(*worker_it).ShowTreatedID();
		double work_total_time = (*worker_it).GetTotalWorkTime() / (double)CLOCKS_PER_SEC;    //���ÿ�����˵Ĺ�����ʱ��
		cout << "work total time: " << work_total_time << " seconds";

		cout << endl;
		double work_strength = (work_total_time / system_run_time) * 100;       //���ÿ�����˵Ĺ���ǿ��
		cout << "work strength: " << work_strength << " %";


		cout << endl << endl << endl;
	}
}

