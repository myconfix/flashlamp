#include "microservothread.h"
#include "wiringPi.h"
#include "softPwm.h"

microservoThread::microservoThread(QObject *parent) :
    QThread(parent)
{
}

void microservoThread::run()
{
    for(int i=0; i< 10; i++)
    {
    softPwmWrite(0,degree);
    this->msleep(20);
    }
   // softPwmWrite(0,0);
}
