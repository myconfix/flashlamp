#include "flashlamp.h"
#include "ui_flashlamp.h"
#include "softPwm.h"
#include "QDebug"

flashlamp::flashlamp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flashlamp)
{

    if(wiringPiSetup() < 0){
      qDebug() << "Can not setup GPIO!";
    }

    pinMode(EncoderPIN,INPUT);
    pinMode(ArmServoPIN,OUTPUT);

    softPwmCreate(ArmServoPIN,0,200);

    client = new QMQTT::Client();

    ui->setupUi(this);

    encoderthread = new encoderThread(this);
    microservothread = new microservoThread(this);

    connect(encoderthread,SIGNAL(encoderChanged()),this,SLOT(encoder_count()));

    encoderthread->start();

    connect(client, SIGNAL(connected()), this, SLOT(onMQTT_Connected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onMQTT_disconnected()));
    connect(client,SIGNAL(received(QMQTT::Message)),this,SLOT(onMQTT_Received(QMQTT::Message)));

       mqtt_setup();

       if(!client->isConnected())
       {
           qDebug() << "Can not Connect MQTT Server";
       }
}

flashlamp::~flashlamp()
{
    delete ui;
}

void flashlamp::encoder_count()
{
    ui->label->setText(QString::number(i++));
}

void flashlamp::on_Base_Dial_sliderMoved(int position)
{
    ui->Base_Dial_LCDnumber->display(position);

}

void flashlamp::on_Arm_Dial_sliderMoved(int position)
{
    ui->Arm_Dial_LCDnumber->display(position);
    microservothread->degree = position;
    if(position != Arm_Dial_Last_Position)
    {
    microservothread->start();
    Arm_Dial_Last_Position = position;
    }
}

//MQTT
void flashlamp::onMQTT_Connected()
{
    qDebug() << "Connected";
    QMQTT::Message msg(0,"/openqt","Hello Flash Lamp From RPI");
    client->publish(msg);
    mqtt_sub(mqtt_topic_sub);
}
void flashlamp::onMQTT_disconnected()
{
    qDebug() << "Disconnected";
}
void flashlamp::onMQTT_Received(const QMQTT::Message &message)
{
    qDebug() << "Topic : " << message.topic();
    qDebug() << "Value : " << message.payload().toInt();

    if(message.topic() == mqtt_topic_base) {
        ui->Base_Dial->setValue(message.payload().toInt());
    }
    if(message.topic() == mqtt_topic_arm) {
        ui->Arm_Dial->setValue(message.payload().toInt());
    }
}
void flashlamp::mqtt_pub(QString topic, QString value)
{
        QMQTT::Message msg(0,topic,value.toUtf8());
        client->publish(msg);
}
void flashlamp::mqtt_sub(QString topic)
{
    client->subscribe(topic,0);
}
void flashlamp::mqtt_setup()
{
    try{
        client->setHost("192.168.1.45");
        client->setPort(1884);
        //client->setUsername
        client->connect();


    }catch (...){

    }
}

void flashlamp::on_Arm_Dial_valueChanged(int value)
{
    microservothread->degree = value;
    if(value != Arm_Dial_Last_Position)
    {
    microservothread->start();
    Arm_Dial_Last_Position = value;
    }else {
        softPwmWrite(ArmServoPIN,0);
    }
}
