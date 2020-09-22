#include <iostream>
#include "countThread.h"
using namespace std;

void CountThread::run(){

    int second = 0;
    while(mIsStopped == false){
        usleep(1*1000*1000);
        emit showCount(second++);
    }
}
