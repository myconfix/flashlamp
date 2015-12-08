#ifndef ENCODERTHREAD_H
#define ENCODERTHREAD_H

#include <QThread>

class encoderThread : public QThread
{
    Q_OBJECT
public:
    explicit encoderThread(QObject *parent = 0);
    void run();

signals:
    void encoderChanged();


public slots:

private :
    int Last_Pin_Status=0;
    int encoder_PIN=29;         //Change RPI pin hear
};

#endif // ENCODERTHREAD_H
