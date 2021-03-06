#ifndef FLASHLAMP_H
#define FLASHLAMP_H

#include <QWidget>
#include "encoderthread.h"
#include "microservothread.h"
#include "baseservothread.h"

#include "wiringPi.h"
#include "qmqttlibRPI/qmqtt.h"

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
    microservoThread *microservothread;
    baseservoThread *baseservothread;


public slots:

    void encoder_count();

    void onMQTT_Connected();
    void onMQTT_disconnected();
    void onMQTT_Received(const QMQTT::Message &message);
    void mqtt_setup();
    void mqtt_pub(QString topic,QString value);
    void mqtt_sub(QString topic);



private slots:
    void on_Base_Dial_sliderMoved(int position);

    void on_Arm_Dial_sliderMoved(int position);


    void on_Arm_Dial_valueChanged(int value);

    void on_Base_Dial_valueChanged(int value);
    \

signals:

    void base_signal(int degree);
    void arm_signal(int degree);

private:
    Ui::flashlamp *ui;
    int i=0;
    int Base_Dial_Last_Degree=0;
    int Base_Current_Degree=0;
    int Arm_Dial_Last_Position=0;

    void moveArmServo(int value);
    void moveBaseServo(int degree);



    long map(long x, long in_min, long in_max,long out_min,long out_max);

    //PIN
    int EncoderPIN = 29;
    int BaseServoPIN=1;
    int ArmServoPIN =0;

    //MQTT
        QMQTT::Client *client;
        QString mqtt_topic_base="/openqt/base";
        QString mqtt_topic_arm="/openqt/arm";
        QString mqtt_topic_sub="/openqt/#";


};

#endif // FLASHLAMP_H
