#include "mainwindow.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <QDebug>

#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QPdfWriter>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    dev_label(new ClickableLabel),
    serial_port_label(new QLabel),
    temperature_label(new QLabel),
    serial(new QSerialPort),
    ui(new Ui::MainWindow)
{
    unsigned char i;
    this->ui->setupUi(this);
    settings = new config;
 //   settings->show();
 //   settings->hide();
    timer = new QTimer();
    timer_update_value=new QTimer();
    nbr=0;
    config_sent=false;
    marche_bool=0;
    connect(timer_update_value, SIGNAL(timeout()), this,SLOT(read_value()));
    connect(timer, SIGNAL(timeout()), this,SLOT(serial_timeout()));
    timer->stop();
    timer_update_value->stop();
    unsigned char j;
    for(j=0;j<4;j++)
    {
        tare[j]=0;
    }

    rcv_serie.nb_char=0;
    rcv_serie.rcv_ok=false;
    dev_label->setAlignment(Qt::AlignLeft);
    dev_label->setFixedWidth(180);
    connect(dev_label,SIGNAL(clicked()),this,SLOT(on_dev_label_clicked()));
    connect(settings,SIGNAL(modify_d()),this,SLOT(config_modify_d()));
    connect(settings,SIGNAL(modify_f()),this,SLOT(config_modify_f()));
    connect(settings,SIGNAL(config_ok()),this,SLOT(config_ok()));

    serial_port_label->setAlignment(Qt::AlignCenter);
    serial_port_label->setIndent(25);
    connect(ui->label,SIGNAL(clicked()),this,SLOT(open_config()));

    temperature_label->setAlignment(Qt::AlignCenter);
    temperature_label->setIndent(25);
    temperature_label->setFixedWidth(150);

    ui->statusBar->addWidget(dev_label);
    ui->statusBar->addWidget(temperature_label,1);
    ui->statusBar->addWidget(serial_port_label, 2);

    dev_label->setText("Réalisé par Enise - XG & LG");
    serial_port_label->setText("Port série: "+serial->portName());
    temperature_label->setText("Temperature ADC:");
    image.load(":/dessin.png");
    image.scaled(QSize(320,240));
    ui->view->setAlignment( Qt::AlignTop | Qt::AlignLeft );
    ui->view->setScene(&scene);
    scene.addPixmap(image);
    for(i=0;i<10;i++)
    {
        valeur[i] = scene.addText("200N", QFont("Libération Sérif", 10) );
        valeur[i]->setPos(45+35*i,3);
    }
    for(i=10;i<15;i++)
    {
        valeur[i] = scene.addText("200N", QFont("Libération Sérif", 10) );
        valeur[i]->setPos(360,50+60*(i-10));
    }
    for(i=15;i<20;i++)
    {
        valeur[i] = scene.addText("200N", QFont("Libération Sérif", 10) );
        valeur[i]->setPos(360-60*(i-15),350);
    }
    ui->view->setSceneRect(0,0,0,0);
    connect_serial();

 }

MainWindow::~MainWindow()
{
       serial->close();
       delete settings;
       delete ui;
}

void MainWindow::serial_timeout()
{
    rcv_serie.rcv_ok=false;
    timer->stop();
}


void MainWindow::read_value()
{
    //on lance la lecture periodiquement
    temps+=0.1;
    //ui->valeur_V1->setText(QString().number(temps));
    send_read();
    timer->start(100);
}

void MainWindow::serialreceived()
{
    int i;
    static unsigned char tabd[4];
    static bool trame_commence;
    timer->start(2000);
    serbuf=serial->readAll();

    for(i=0;i<serbuf.size();i++)
    {
        if ((serbuf.at(i)=='?')&&(rcv_serie.nb_char==0)&& !trame_commence)
        {
            rcv_serie.nb_char=1;
            trame_commence=true;
        }
        else if((rcv_serie.nb_char<2)&&trame_commence)
        {
            rcv_serie.num_module=serbuf.at(i);
            rcv_serie.nb_char++;
        }
        else if((rcv_serie.nb_char<3)&&trame_commence)
        {
            rcv_serie.voie=serbuf.at(i);
            rcv_serie.nb_char++;
        }
        else if((rcv_serie.nb_char<7)&&trame_commence)
        {
            tabd[rcv_serie.nb_char-3]=serbuf.at(i);
            rcv_serie.nb_char++;
        }
        else if((serbuf.at(i)=='!')&&(rcv_serie.nb_char==7)&&trame_commence)
        {
            //rcv_serie.deplacement=(rcv_serie.temp_val)+(static_cast<unsigned char> (serbuf.at(i))<<8);
            //  qDebug() << rcv_serie.deplacement << "dep" << rcv_serie.force << "force \r\n";
            rcv_serie.valeur=(long)((tabd[0]<<24)+(tabd[1]<<16)+(tabd[2]<<8)+(tabd[3]));
            timer->stop();
            rcv_serie.nb_char=0;
            rcv_serie.rcv_ok=true;
             trame_commence=false;
        }
        else
        {
            trame_commence=false;
            rcv_serie.nb_char=0;
            rcv_serie.valeur=0;
            rcv_serie.rcv_ok=false;
        }
        if(rcv_serie.rcv_ok==true)
        {
            rcv_serie.rcv_ok=false;
            cmpt_nbvoie++;
            if (rcv_serie.voie==0x0c)
            {
             //   temperature_label->setText("Temp. ADC = "+QString().asprintf("%.2f",4.0096*1e-4*rcv_serie.valeur*2.048-271.4)+" °C");
                tab_last_val[nb_last_val][4].valeur= rcv_serie.valeur;
                tab_last_val[nb_last_val][4].voie= rcv_serie.voie;
            }
            else if(rcv_serie.voie==0x08)
            {
               // ui->valeur_V1->setText(QString().number(rcv_serie.valeur-tare[0]));
                tab_last_val[nb_last_val][0].valeur= rcv_serie.valeur;
                tab_last_val[nb_last_val][0].voie= rcv_serie.voie;
            }
            else if(rcv_serie.voie==0x09)
            {
               // ui->valeur_V2->setText(QString().number(rcv_serie.valeur-tare[1]));
                tab_last_val[nb_last_val][1].valeur= rcv_serie.valeur;
                tab_last_val[nb_last_val][1].voie= rcv_serie.voie;
            }
            else if(rcv_serie.voie==0x0A)
            {
             //   ui->valeur_V3->setText(QString().number(rcv_serie.valeur-tare[2]));
                tab_last_val[nb_last_val][2].valeur= rcv_serie.valeur;
                tab_last_val[nb_last_val][2].voie= rcv_serie.voie;
            }
            else if(rcv_serie.voie==0x0B)
            {
              //  ui->valeur_V4->setText(QString().number(rcv_serie.valeur-tare[3]));
                tab_last_val[nb_last_val][3].valeur= rcv_serie.valeur;
                tab_last_val[nb_last_val][3].voie= rcv_serie.voie;
            }
            if (cmpt_nbvoie>=5)
            {
                cmpt_nbvoie=0;
                nb_last_val++;
                if (nb_last_val>=10)
                {
                    nb_last_val=0;
                    emit zero();
                }
            }
            //rcv_serie.force=abs(rcv_serie.force);
            //rcv_serie.deplacement=abs(rcv_serie.deplacement);
        }
    }

}

void MainWindow::config_ok()
{
}

void MainWindow::config_modify_d()
{
}

void MainWindow::config_modify_f()
{
}

void MainWindow::open_config()
{
    settings->show();
}

void MainWindow::on_dev_label_clicked()
{
    QMessageBox::about(this,"Boite Force",
                       "<h4>TP de Résistance Des Matériaux</h4>\n\n"
                       "TP RDM <br>"
                       "Copyright 2024 Enise.<br>"
                       "Support: Galtier Granjon - Centrale Lyon - ENISE <br>"
                       "<a href=\"http://www.enise.fr\">http://www.enise.fr</a>" );

}
void MainWindow::on_pb_savecsv_clicked()
{
    QFile fichiercsv;
    QString filename;
    filename=QFileDialog::getSaveFileName();// fd->directory().path()+"/fichiercsv.csv");
    fichiercsv.setFileName(filename);

    if (fichiercsv.open(QFile::WriteOnly|QFile::Truncate))
    {
        QTextStream stream(&fichiercsv);
        QStringList strList;
        //Entete fichier
        strList << ui->TitreCourbe->toPlainText();
        stream << strList.join(";")+"\n";
        strList.clear();
        strList << "Valeur Voie 1 (N);Valeur Voie 2 (N);Valeur Voie 3 (N);Valeur Voie 4 (N)";
        stream << strList.join(";")+"\n";
        // for( int r = 0; r < ui->tableWidget_1->rowCount(); ++r )
        // {
        //     strList.clear();
        //     strList << (ui->tableWidget_1->item( r,0)->text()+";"+ui->tableWidget_2->item( r,0)->text()+";"+ui->tableWidget_3->item( r,0)->text()+";"+ui->tableWidget_4->item( r,0)->text());
        //     stream << strList.join(";")+"\n";
        // }
        fichiercsv.close();
    }

}

void MainWindow::connect_serial()
{
        QString port_name;
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
      /*        qDebug() << "Name : " << info.portName();
                qDebug() << "Description : " << info.description();
                qDebug() << "Manufacturer: " << info.manufacturer();
                qDebug() << "VID " << info.vendorIdentifier();
                qDebug() << "PID" << info.productIdentifier();

       */
            if((info.vendorIdentifier()==0x04d8)&&(info.productIdentifier()==0x0057)) //Dmanufacturer()=="Essais Flambage Enise"))
            {
                port_name=info.portName();
            }
        }
//        if(serial->isOpen())
//        {
//            serial->disconnect(serial,SIGNAL(readyRead()),this,SLOT(serialreceived()));
//            serial->flush();
//            serbuf.clear();
//            serial->close();
//        }
        rcv_serie.nb_char=0;
        rcv_serie.rcv_ok=false;
#ifdef OS_LINUX
       serial->setPortName("/dev/"+port_name);
        //serial->setPortName("/dev/pts/1"); // only for debug
#elif
        serial->setPortName(portname);
#endif
        if(port_name=="")
        {
         //   ui->pb_start->setChecked(false);
            serial_port_label->setText("Port Série: Clos");
            config_sent=false;
            serial->disconnect();
            serial->clearError();
            serial->close();

        }
        else
        {
            tab_data.clear();
            serbuf.clear();
            serial->setBaudRate(19200);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            serial->setReadBufferSize(0);
            serial->open(QIODevice::ReadWrite);
            serial->connect(serial,SIGNAL(readyRead()),this,SLOT(serialreceived()),Qt::UniqueConnection);
            serial_port_label->setText("Port série: "+serial->portName());
            if(config_sent==false)
            {
                config_sent=true;
                config_ok();
            }
            timer_update_value->start(100);
        }
}

void MainWindow::send_read()
{
        serial->write("R",1);
        //qDebug() << data_to_send;
}

void MainWindow::send_commande(char type,long value)
{
    QByteArray data_to_send(8,'0');
     serial->write(data_to_send,8);
     qDebug() << data_to_send;
}

void MainWindow::on_pb_tare_clicked()
{
     unsigned char i,j;
     QEventLoop loop;
     connect(this, SIGNAL(zero()), &loop, SLOT(quit()));
     loop.exec();
     connect(this, SIGNAL(zero()), &loop, SLOT(quit()));
     loop.exec();
     for(j=0;j<4;j++)
     {
        tare[j]=0;
     }
     for(j=0;j<4;j++)
    {
    for(i=0;i<10;i++)
    {
            tare[j]=tare[j]+tab_last_val[i][j].valeur;
    }
     tare[j]=tare[j]/10;
     }
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
     QSize old_size,new_size,test;
     qreal sx,sy;
     QMainWindow::resizeEvent(event);
     old_size=event->oldSize();
     new_size=event->size();
     test=ui->view->size();
     sx=(qreal)new_size.width()/(qreal)old_size.width();
     sy=(qreal)new_size.height()/(qreal)old_size.height();
     if((sx > 0)&&(sy>0))
     {
     ui->view->scale(sx,sy);
     }
     // Your code here.
}

void MainWindow::on_pushButton_clicked()
{
     cmpt_nbvoie++;
     valeur[0]->setPlainText(QString().number(cmpt_nbvoie));


}

