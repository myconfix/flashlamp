#include "baseservothread.h"
#include "wiringPi.h"
#include "softPwm.h"
#include <QDebug>

baseservoThread::baseservoThread(QObject *parent) :
    QThread(parent)
{
}

void baseservoThread::run()
{
    softPwmWrite(1,pwm);
    this->msleep(20);
    softPwmWrite(1,0);
    qDebug() << "Thred exit";

}

void baseservoThread::encoder_count()
{

}
void baseservoThread::set_pwm(int value)
{
    pwm=value;
}



