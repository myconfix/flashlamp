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
    pinMode(MicroServoPIN,OUTPUT);

    softPwmCreate(MicroServoPIN,0,200);

    ui->setupUi(this);
    encoderthread = new encoderThread(this);
    microservothread = new microservoThread(this);

    connect(encoderthread,SIGNAL(encoderChanged()),this,SLOT(encoder_count()));

    encoderthread->start();


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
    microservothread->start();
}

void flashlamp::on_Arm_Dial_sliderReleased()
{
    softPwmWrite(MicroServoPIN,0);
}
