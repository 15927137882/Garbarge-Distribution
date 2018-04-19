#pragma once
#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include "Rubbish.h"
#include "Worker.h"
#include "Thread_func.h"

const unsigned int TOTAL_RUBBISH = 20;        //需要处理的垃圾总数，单位个
const int WORKER_NUMNER = 9;
const int TIME_NEED_TO_FINISH = 5;   //每个垃圾需要的处理时间，单位秒