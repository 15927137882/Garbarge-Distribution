#include "Simulink.h"

Worker::Worker(Rubbish r):now_rubbish(r)
{
	worker_id = 0;
	start_moving_time = 0;
	busy = false;
	position.x = 0;
	position.y = 0;
	treated_num = 0;
	each_work_time = 0;
	total_work_time = 0;
	min_x = 0;
	max_x = 0;
	min_y = 0;
	max_y = 0;
}

void Worker::ShowTreatedID()
{
	if (treated_rubbish_id.size() == 0)
		std::cout << "worker " << worker_id << " has not treated any rubbish";
	else
	{
		std::list<int>::iterator it;
		for (it = treated_rubbish_id.begin(); it != treated_rubbish_id.end(); it++)
			std::cout << "->" << (*it);
	}
	std::cout << std::endl;
}

void Worker::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

void Worker::SetPosition(const Point p)
{
	position.x = p.GetX();
	position.y = p.GetY();
}

void Worker::ShowPosition() const
{
	std::cout << "(" << position.GetX() << "," << position.GetY() << ")";
}

void Worker::FlushTreatedID(int x)
{
	treated_rubbish_id.push_back(x);
}

void Worker::SetRange(int x1, int x2, int y1, int y2)
{
	min_x = x1;
	min_y = y1;
	max_x = x2;
	max_y = y2;
}

void InitWorker()
{
	extern std::list<Worker> worker_array;
	Rubbish r(0);
	Worker worker1(r), worker2(r), worker3(r);
	Worker worker4(r), worker5(r), worker6(r);
	Worker worker7(r), worker8(r), worker9(r);
	
	worker1.SetWorkerID(1);
	worker1.SetPosition(15, 15);
	worker1.SetRange(0, 30, 0, 30);
	worker_array.push_back(worker1);
	
	worker2.SetWorkerID(2);
	worker2.SetPosition(45, 15);
	worker2.SetRange(30,60,0,30);
	worker_array.push_back(worker2);

	worker3.SetWorkerID(3);
	worker3.SetPosition(75, 15);
	worker3.SetRange(60,90,0,30);
	worker_array.push_back(worker3);

	worker4.SetWorkerID(4);
	worker4.SetPosition(15, 45);
	worker4.SetRange(0,30,30,60);
	worker_array.push_back(worker4);

	worker5.SetWorkerID(5);
	worker5.SetPosition(45, 45);
	worker5.SetRange(30,60,30,60);
	worker_array.push_back(worker5);

	worker6.SetWorkerID(6);
	worker6.SetPosition(75, 45);
	worker6.SetRange(60,90,30,60);
	worker_array.push_back(worker6);

	worker7.SetWorkerID(7);
	worker7.SetPosition(15, 75);
	worker7.SetRange(0,30,60,90);
	worker_array.push_back(worker7);

	worker8.SetWorkerID(8);
	worker8.SetPosition(45, 75);
	worker8.SetRange(30,60,60,90);
	worker_array.push_back(worker8);

	worker9.SetWorkerID(9);
	worker9.SetPosition(75, 75);
	worker9.SetRange(60,90,60,90);
	worker_array.push_back(worker9);

}

int abs_distance(const Point a,const Point b)
{
	return abs(a.GetX() - b.GetX()) + abs(a.GetY() - b.GetY());
}

bool worker_cmp(const Worker& a, const Worker& b)
{
	return a.worker_id < b.worker_id;
}