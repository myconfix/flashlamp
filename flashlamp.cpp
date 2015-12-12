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
    pinMode(BaseServoPIN,OUTPUT);

    softPwmCreate(ArmServoPIN,0,200);
    softPwmCreate(BaseServoPIN,0,200);

    client = new QMQTT::Client();

    ui->setupUi(this);

    encoderthread = new encoderThread(this);
    microservothread = new microservoThread(this);
    baseservothread = new baseservoThread(this);

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
    //Encode 24 => 360 degree
    ui->label->setText(QString::number(i++));
    if(baseservothread->pwm > 15)
    {
      Base_degree=Base_degree+15;
    }else {
        Base_degree=Base_degree-15;
    }
    //Limit
    if(Base_degree > 360)
    {
        Base_degree =360;
    }else if (Base_degree <0) {
        Base_degree=0;
    }
    qDebug() << "Degree : " <<  Base_degree;
}

void flashlamp::on_Base_Dial_sliderMoved(int position)
{
    ui->Base_Dial_LCDnumber->display(position);

}

void flashlamp::on_Arm_Dial_sliderMoved(int position)
{
    ui->Arm_Dial_LCDnumber->display(position);
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
    }else   if(message.topic() == mqtt_topic_arm) {
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
        client->setHost("192.168.43.137");
        client->setPort(1884);
        //client->setUsername
        client->connect();


    }catch (...){

    }
}

void flashlamp::on_Arm_Dial_valueChanged(int value)
{
    moveArmServo(value);
}
void flashlamp::on_Base_Dial_valueChanged(int value)
{
    moveBaseServo(value);
}
void flashlamp::moveArmServo(int value){

    microservothread->degree = static_cast<int>(map(value,0,180,8,25));
    qDebug() << "Degree : " << microservothread->degree;
    if(value != Arm_Dial_Last_Position)
    {
    microservothread->start();
    Arm_Dial_Last_Position =value;
    }else {
        softPwmWrite(ArmServoPIN,0);
    }
}
void flashlamp::moveBaseServo(int value){
//for Test
    /*
    baseservothread->pwm = static_cast<int>(map(value,0,360,8,25));
    qDebug() << "PWM : " << baseservothread->pwm;
    i   f(value != Base_Dial_Last_Position)
    {
        baseservothread->start();
         Base_Dial_Last_Position = value;
    }else{
        softPwmWrite(BaseServoPIN,0);
    }
    */
// Get Degree Value -> check current degree value from thread-->move to new degree value
 /*   if (value-Base_degree > 0)
    {
       baseservothread->pwm = 16;
        baseservothread->start();
      while(Base_degree < value) {};
        baseservothread->pwm =15;//stop
    }else if (value-Base_degree <0)
    {
       baseservothread->pwm = 14;
       baseservothread->start();
       while(Base_degree > value) {};
       baseservothread->pwm =15;//stop
    }
*/
}
long flashlamp::map(long x, long in_min, long in_max,long out_min,long out_max){
    return (x-in_min)*(out_max-out_min) / (in_max - in_min) + out_min;
}


