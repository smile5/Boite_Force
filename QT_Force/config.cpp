#include "config.h"
#include "ui_config.h"

config::config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);
    //this->ui->cb_Deplacement->hide();
    this->ui->sp_force->hide();
    this->ui->sp_Gv->hide();
    this->ui->sp_Pv->hide();
    this->ui->sp_Step->hide();
}

char config::value_deplacement() const
{
   // return(this->ui->cb_Deplacement->currentIndex());
}

long config::value_force() const
{
    return(this->ui->sp_force->value());
}

unsigned short config::value_PWM_Pv() const
{
    return(this->ui->sp_Pv->value());
}

unsigned short config::value_PWM_Gv() const
{
    return(this->ui->sp_Gv->value());
}

unsigned long config::value_step() const
{
    return(this->ui->sp_Step->value());
}

config::~config()
{
    delete ui;
}

void config::on_cb_Deplacement_currentIndexChanged(int index)
{
    emit modify_d();
}


void config::on_cb_force_currentIndexChanged(int index)
{
    emit modify_f();
}


void config::on_buttonBox_accepted()
{
    //this->ui->cb_Deplacement->hide();
    this->ui->sp_force->hide();
    this->ui->sp_Gv->hide();
    this->ui->sp_Pv->hide();
    this->ui->sp_Step->hide();
    emit config_ok();
}

void config::on_passwd_button_clicked()
{
    if(this->ui->le_passwd->text()=="Mogwai")
    {
      //  this->ui->cb_Deplacement->show();
        this->ui->sp_force->show();
        this->ui->sp_Gv->show();
        this->ui->sp_Pv->show();
        this->ui->sp_Step->show();
    }
    else
    {
        //this->ui->cb_Deplacement->hide();
        this->ui->sp_force->hide();
        this->ui->sp_Gv->hide();
        this->ui->sp_Pv->hide();
        this->ui->sp_Step->hide();
    }
    this->ui->le_passwd->setText("");
}

