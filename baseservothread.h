#ifndef BASESERVOTHREAD_H
#define BASESERVOTHREAD_H

#include <QThread>

class baseservoThread : public QThread
{
    Q_OBJECT
public:
    explicit baseservoThread(QObject *parent = 0);
    void run();

    int pwm=15;  //15=stop,pwm>15 rotate CW,pwm<15 rotate CCW
    int degree=0; //Range 0-360

signals:

public slots:

};

#endif // BASESERVOTHREAD_H
