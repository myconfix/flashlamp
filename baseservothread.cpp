#include "baseservothread.h"
#include "wiringPi.h"
#include "softPwm.h"

baseservoThread::baseservoThread(QObject *parent) :
    QThread(parent)
{
}

void baseservoThread::run()
{
   // for(int i=0; i<10;i++)
    //{
       softPwmWrite(1,pwm);
       this->msleep(20);
    //}
    softPwmWrite(1,0);
}
