#include "widget.h"
#include "ui_widget.h"

bool isLinkLocalAddress(QHostAddress addr);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);

    //断开连接和发送按键灰化
    ui->pushButtonDiscon->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);

    //功能控制按钮灰化
    ui->pushButtonLightOn->setEnabled(false);
    ui->pushButtonLightOff->setEnabled(false);
    ui->pushButtonFanOn->setEnabled(false);
    ui->pushButtonFanOff->setEnabled(false);
    ui->pushButtonLeaveHome->setEnabled(false);
    ui->pushButtonReturnHome->setEnabled(false);

    //获取系统上ip地址，例如ipconfig
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for(QHostAddress add : addresses)
    {
        //如果是ipv4：
        if(add.protocol() == 0 && isLinkLocalAddress(add))
            ui->comboBoxIPAddr->addItem(add.toString());
    }

    //对于接收数据，连接信号与槽
    //readyRead信号：void QIODevice::​readyRead()
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()) );
}

//用于筛选范围内的IP地址
bool isLinkLocalAddress(QHostAddress addr){
    quint32 nIPv4 = addr.toIPv4Address();
    quint32 nMinRange = QHostAddress("192.168.0.0").toIPv4Address();
    quint32 nMaxRange = QHostAddress("192.168.255.244").toIPv4Address();
    if ((nIPv4 >= nMinRange) && (nIPv4 <= nMaxRange)) {
        return true;
    } else {
        return false;
    }
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::bind(QString ip, ushort port)
{
    return udpSocket->bind(QHostAddress(ip), port);
}

void Widget::setTargetInfo(QString ip, quint16 port)
{
    // 存储传入的IP和端口号
    m_hostAddr = QHostAddress(ip);
    m_port = port;
}

void Widget::sendData(QString data)
{
    udpSocket->writeDatagram(data.toUtf8(), m_hostAddr, m_port);
}

void Widget::on_pushButtonConnect_clicked()
{
    //获取用户端口号
    int port_num = ui->lineEditPort->text().toInt();
    QString ip_addr = ui->comboBoxIPAddr->currentText();
    qDebug()<<ip_addr;

    //绑定自己的端口和ip
    int localport_num = ui->lineEditPortLocal->text().toInt();
    if(bind(ip_addr, localport_num))
    {
        qDebug()<<"connect success!";
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("The port has been used.");
        msgBox.exec();
        return;
    }

    //设置对方的端口和ip
    setTargetInfo(ip_addr, port_num);

    //按键灰化
    ui->pushButtonConnect->setEnabled(false);

    //断开连接和发送按键使能
    ui->pushButtonDiscon->setEnabled(true);
    ui->pushButtonSend->setEnabled(true);

    //功能控制按钮使能
    ui->pushButtonLightOn->setEnabled(true);
    ui->pushButtonLightOff->setEnabled(true);
    ui->pushButtonFanOn->setEnabled(true);
    ui->pushButtonFanOff->setEnabled(true);
    ui->pushButtonLeaveHome->setEnabled(true);
    ui->pushButtonReturnHome->setEnabled(true);
}

void Widget::on_pushButtonSend_clicked()
{
    //qDebug()<<"send";
    QString send_msg = ui->textEditSend->toPlainText();
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: "+send_msg+"\r\n");
}


//实现接收数据的槽函数
void Widget::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

        //processTheDatagram(datagram);
        ui->textEditRecv->insertPlainText("Recv: "+QString(datagram)+"\r\n");
    }
}

void Widget::on_pushButtonDiscon_clicked()
{
    //断开现有连接
    udpSocket->close();

    //按键状态恢复
    //连接按键重新使能
    ui->pushButtonConnect->setEnabled(true);

    //断开连接和发送按键灰化
    ui->pushButtonDiscon->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);

    //功能控制按钮灰化
    ui->pushButtonLightOn->setEnabled(false);
    ui->pushButtonLightOff->setEnabled(false);
    ui->pushButtonFanOn->setEnabled(false);
    ui->pushButtonFanOff->setEnabled(false);
    ui->pushButtonLeaveHome->setEnabled(false);
    ui->pushButtonReturnHome->setEnabled(false);
}

//开灯
void Widget::on_pushButtonLightOn_clicked()
{
    QString send_msg = "{\"dev\":\"lamp1\",\"status\":\"1\"}";
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: light1 on\r\n");
}

//关灯
void Widget::on_pushButtonLightOff_clicked()
{
    QString send_msg = "{\"dev\":\"lamp1\",\"status\":\"0\"}";
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: light1 off\r\n");
}

//开风扇
void Widget::on_pushButtonFanOn_clicked()
{
    QString send_msg = "{\"dev\":\"fan\",\"status\":\"1\"}";
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: fan on\r\n");
}

//关风扇
void Widget::on_pushButtonFanOff_clicked()
{
    QString send_msg = "{\"dev\":\"fan\",\"status\":\"0\"}";
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: fan off\r\n");
}

//离家模式
void Widget::on_pushButtonLeaveHome_clicked()
{
    QString send_msg = "{\"dev\":\"home\",\"status\":\"1\"}";
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: leave home\r\n");
}

//回家模式
void Widget::on_pushButtonReturnHome_clicked()
{
    QString send_msg = "{\"dev\":\"home\",\"status\":\"0\"}";
    sendData(send_msg);
    ui->textEditRecv->insertPlainText("Send: return home\r\n");
}
