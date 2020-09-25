#include <iostream>
#include "countThread.h"
using namespace std;

void CountThread::run(){

    int second = 0;
    while(mIsStopped == false){
        emit showCount(second++);
        usleep(1*1000*1000);
    }
}
