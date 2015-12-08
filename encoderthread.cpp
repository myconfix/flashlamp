#include "encoderthread.h"
#include "wiringPi.h"

encoderThread::encoderThread(QObject *parent) :
    QThread(parent)
{
}

void encoderThread::run()
{

    while(1)
    {
        int pinRead = digitalRead(encoder_PIN);
        if(pinRead !=Last_Pin_Status){
            emit encoderChanged();
            Last_Pin_Status= pinRead;
        }
        this->msleep(200);
    }
}
