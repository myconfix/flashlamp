#ifndef MICROSERVOTHREAD_H
#define MICROSERVOTHREAD_H

#include <QThread>

class microservoThread : public QThread
{
    Q_OBJECT
public:
    explicit microservoThread(QObject *parent = 0);

    void run();

    int degree=0;

signals:


public slots:

private :
    //int degree2pulsewidth();

};

#endif // MICROSERVOTHREAD_H
