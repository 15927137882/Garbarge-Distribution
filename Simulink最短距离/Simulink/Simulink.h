#pragma once
#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include "Rubbish.h"
#include "Worker.h"
#include "Thread_func.h"

const unsigned int TOTAL_RUBBISH = 20;        //��Ҫ�����������������λ��
const int WORKER_NUMNER = 9;
const int TIME_NEED_TO_FINISH = 5;   //ÿ��������Ҫ�Ĵ���ʱ�䣬��λ��