#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include "serialport.h"
#include "obsbuffer.h"
#include <QFile>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QStringList getPortNameList();
    void initQFile();
 signals:
    void closePort_sig(int portId);

    void writePort_sig(int portId,QByteArray buff);
    void RoverObsReady();
public slots:
    void on_pushButton_comopenA_clicked();
    void on_pushButton_comopenB_clicked();
    void on_pushButton_sendA_clicked();
    void on_pushButton_sendB_clicked();
    void on_showData(int portId,char buff,int flag);//用于显示数据
    void on_pushData(int portId, int newobs);//用于解析数据

    void on_pushEph();
private slots:
    void on_pushButton_saveA_clicked();

    void on_pushButton_saveB_clicked();

    void on_pushButton_proc_clicked();

    void procStart();

    void Rtk();

private:
    Ui::Widget *ui;
    SerialPort *PortA;
    SerialPort *PortB;
    QStringList m_portNameList;
    CObsBuffer ObsBufferA;
    CObsBuffer ObsBufferB;

    QFile       *m_fileA;
    QFile       *m_fileB;
    QDataStream *m_DatastreamA;
    QDataStream *m_DatastreamB;
     QTimer *m_showTimer;

};

#endif // WIDGET_H
