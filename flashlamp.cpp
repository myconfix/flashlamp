#include "flashlamp.h"
#include "ui_flashlamp.h"


flashlamp::flashlamp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::flashlamp)
{

    wiringPiSetup();
    pinMode(29,INPUT);

    ui->setupUi(this);
    encoderthread = new encoderThread(this);

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
}
