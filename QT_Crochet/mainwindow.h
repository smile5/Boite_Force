#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#define OS_LINUX

#include "config.h"
#include "clickablelabel.h"
#include <QMainWindow>
#include <QLabel>
#include <QSerialPortInfo>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QResizeEvent>

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class config;

class MainWindow : public QMainWindow
{
    Q_OBJECT

#define COLOR_force QColor(193,125,17)

    typedef struct{
        bool rcv_ok;
        unsigned char nb_char;
        unsigned char voie;
        unsigned char num_module;
        long valeur;
    } rcv_message;

    typedef struct{
        unsigned char voie;
        long valeur;
    } val_mcp356x;

    typedef struct{
        qreal x;
        qreal y;
    } position;
public:
    explicit MainWindow(QWidget *parent = 0);

void resizeEvent(QResizeEvent* event);
     QSerialPortInfo serialPortInfo;
     ClickableLabel *dev_label;
     QLabel *serial_port_label;
     QLabel *temperature_label;
     double compteur=0;
     ~MainWindow();
     rcv_message rcv_serie;
     QTimer *timer;

private slots:
    void serial_timeout();
    void serialreceived();
    void on_pb_savecsv_clicked();
    void on_dev_label_clicked();
    void open_config();
    void config_modify_d();
    void config_modify_f();
    void config_ok();
    void read_value();
    void on_pb_tare_clicked();
    void on_pushButton_clicked();

signals:
    void zero();
private:
    val_mcp356x tab_last_val[10][5];
    unsigned char nb_last_val=0;
    double temps=0;
    QSerialPort *serial;
    Ui::MainWindow *ui;
    void connect_serial();
    void send_commande(char type,long value=0);
    void send_read();
    QByteArray serbuf;
    QVector<rcv_message> tab_data;
    long nbr;
    bool marche_bool;
    bool config_sent;
    config *settings;
    QTimer *timer_update_value;
    unsigned char cmpt_nbvoie=0;
    long tare[5];

    QPixmap image;
    QGraphicsScene scene;
    QGraphicsTextItem *valeur[20];
    position text_pos[20]; //={{45,3},{90,3},{130,3},{170,3},{210,3},{250,3},{290,3},{330,3},{370,3},{410,3}, \
                            {120,120},{150,150},{200,200},{250,250},{300,300},{320,320},{350,350},{0,0},{0,0},{0,0}};
};

#endif // MAINWINDOW_H
