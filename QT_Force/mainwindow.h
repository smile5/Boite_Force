#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define OS_LINUX

#include "config.h"
#include "clickablelabel.h"
#include <QMainWindow>
#include <QLabel>
#include <QSerialPortInfo>

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
public:
    explicit MainWindow(QWidget *parent = 0);
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
    void on_pushButton_clicked();
    void on_add_tableau_clicked();
    void on_reset_data_clicked();
    void on_pb_tare_clicked();
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
};

#endif // MAINWINDOW_H
