#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QtCore>
#include <QFileDialog>
#include <QIODevice>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_portNameList = getPortNameList();
    ui->comboBox_portNameA->addItems(m_portNameList);


    //设置默认波特率
    ui->comboBox_baudRateA->setCurrentIndex(6);//115200    

    if(ui->comboBox_portNameA->count()>2)
    {
        ui->comboBox_portNameA->setCurrentIndex(0);

    }    


   m_fileA = nullptr;

   m_DatastreamA = nullptr;


    m_showTimer = new QTimer(this);

    m_showTimer->start(10); /*开启定时器，并且每10ms后询问一次。定时的时间一到，马上产生timeout（）信号，继续执行自定义槽函数*/

}

Widget::~Widget()
{
    delete ui;
}
QStringList Widget::getPortNameList()
{
    QStringList serialPortName;
    //查找可用的串口
    foreach(const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
    {
        serialPortName << Info.portName();
        qDebug()<<"portname: "<<Info.portName();//调试时可以看的串口信息
    }
    return serialPortName;
}

void Widget::on_pushButton_comopenA_clicked()
{
    //对串口进行一些初始化
    if(ui->pushButton_comopenA->text()=="打开")
    {        
         PortA = new SerialPort(COM0,ui->comboBox_portNameA->currentText(), ui->comboBox_baudRateA->currentText().toInt());

        //接收从子线程传输数据的信号
        connect(PortA,SIGNAL(receive_data(int,char,int)),this,SLOT(on_showData(int,char,int)));//,Qt::QueuedConnection
        connect(this,SIGNAL(writePort_sig(int,QByteArray)),PortA,SLOT(write_data(int,QByteArray)));
        connect(this,SIGNAL(closePort_sig(int)),PortA,SLOT(closePort(int)));
        //关闭设置菜单使能
        ui->comboBox_portNameA->setEnabled(false);
        ui->comboBox_baudRateA->setEnabled(false);
        ui->pushButton_comopenA->setText("关闭");
    }
    else
    {
        //关闭线程
        if(PortA!=nullptr)
        {
            emit closePort_sig(COM0);
        }
        //恢复设置菜单使能
        ui->comboBox_portNameA->setEnabled(true);
        ui->comboBox_baudRateA->setEnabled(true);
        ui->pushButton_comopenA->setText("打开");
    }
}

void Widget::on_pushButton_sendA_clicked()
{
    emit writePort_sig(COM0,ui->lineEdit_writeA->text().toUtf8());
    if(ui->checkBoxA->isChecked())
    {
        emit writePort_sig(COM0,"\n");
    }
}

void Widget::on_pushButton_sendB_clicked()
{
//    emit writePort_sig(COM1,ui->lineEdit_writeB->text().toUtf8());
//    if(ui->checkBoxB->isChecked())
//    {
//        emit writePort_sig(COM1,"\n");
//    }
}

void Widget::on_pushData(int portId, int newobs)
{
    QTextEdit* pTextEd=nullptr;

    QString msg;

    switch(portId)
    {
    case COM0:
       // pTextEd = ui->textEdit_readC;
        break;
    case COM1:
//        pTextEd = ui->textEdit_readD;
        break;
    default:
        break;
    }

    msg.sprintf("%d\n", newobs);
    pTextEd->moveCursor(QTextCursor::End);
    pTextEd->insertPlainText(msg);
}

int flag = 0;
void Widget::on_showData(int portId, char buff,int flag)//
{
    QTextEdit* pTextEd=nullptr;
    QDateTime curDateTime=QDateTime::currentDateTime();
    QString data =curDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QString byte = QString(buff);

    portId = COM0;
    switch(portId)
    {
    case COM0:

        pTextEd = ui->textEdit_readA;
        if(m_DatastreamA != nullptr)
        {

            *m_DatastreamA << buff;       // 将数据写入到数据流中，注意是带*
        }
        break;
    case COM1:
//        pTextEd = ui->textEdit_readB;
        if(m_DatastreamB != nullptr)
        {
            *m_DatastreamB << buff;       // 将数据写入到数据流中，注意是带*
        }
        break;
    default:
        pTextEd = nullptr;
        break;
    }
    if(flag==0)  pTextEd->insertPlainText(data+" ");
    pTextEd->moveCursor(QTextCursor::End);
    pTextEd->insertPlainText(byte);

}

void Widget::on_pushButton_saveA_clicked()
{
    if(ui->pushButton_saveA->text()=="打开雷达")
    {
        flag = 1;
       ui->pushButton_saveA->setText("停止");
    }
    else
    {

         flag = 0;
        ui->pushButton_saveA->setText("打开雷达");
    }
}



void Widget::on_pushButton_proc_clicked()
{
    //connect(this,SIGNAL(RoverObsReady()),this,SLOT(procStart()));
    QTextEdit* pTextEd=nullptr;
    pTextEd = ui->textEdit_readA;
    pTextEd->clear();
}
void Widget::procStart()
{


}
void Widget::Rtk()
{

}
