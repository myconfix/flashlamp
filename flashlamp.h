#ifndef FLASHLAMP_H
#define FLASHLAMP_H

#include <QWidget>
#include "encoderthread.h"
#include "wiringPi.h"

namespace Ui {
class flashlamp;
}

class flashlamp : public QWidget
{
    Q_OBJECT

public:
    explicit flashlamp(QWidget *parent = 0);
    ~flashlamp();
    encoderThread *encoderthread;

public slots:

    void encoder_count();

private slots:
    void on_Base_Dial_sliderMoved(int position);

    void on_Arm_Dial_sliderMoved(int position);

private:
    Ui::flashlamp *ui;
    int i=0;
    int Base_Dial_Last_Position;

};

#endif // FLASHLAMP_H
