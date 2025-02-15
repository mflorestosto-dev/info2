#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>


#define LED_GREEN_ON        0
#define LED_RED_ON          1
#define LED_GRAY            2
#define LED_BLUE_ON         3
#define LED_RED_GRAYED      4
#define LED_GREEN_GRAYED    5
#define LED_BLUE_GRAYED     6

using namespace Qt::StringLiterals;

int flag_habilitacion = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->pushButton_LedRed->setStyleSheet("background-color: rgb(200, 150, 150);");
    ui->pushButton_LedGreen->setStyleSheet("background-color: rgb(150, 200, 150);");
    ui->pushButton_LedBlue->setStyleSheet("background-color: rgb(150, 150, 200);");
    // ui->label_LedRed->setColour(LED_RED_GRAYED);
    // ui->label_LedGreen->setColour(LED_GREEN_GRAYED);
    // ui->label_LedBlue->setColour(LED_BLUE_GRAYED);
    server.listen(QHostAddress::Any, PORTsERVER);
    //Connect to slot
    connect( &server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_LedGreen_clicked()
{
    if(flag_habilitacion){
        // Construir la trama a enviar
        QString buffer = ">L,N,1,";
        char chk = 0;

        // Calcular el checksum mediante XOR de los caracteres
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, el delimitador de fin y el salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si existe al menos un socket conectado
        if (!sockets.isEmpty() && sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        qDebug() << buffer;
    }
}


void MainWindow::on_pushButton_LedRed_clicked()
{
    // Establecer el texto del botón (opcional)
    if(flag_habilitacion){
       // Construir la  trama a enviar
        QString buffer = ">L,Q,1,";
        char chk = 0;

        // Calcular el checksum mediante XOR de los caracteres
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, el delimitador de fin y el salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si existe al menos un socket conectado
        if (!sockets.isEmpty() && sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        qDebug() << buffer;
    }
}


void MainWindow::on_pushButton_LedBlue_clicked()
{
    // Establecer el texto del botón (opcional

    if(flag_habilitacion){
        // Construir la trama a enviar
        QString buffer = ">L,C,1,";
        char chk = 0;

        // Calcular el checksum mediante XOR de los caracteres
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, el delimitador de fin y el salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si existe al menos un socket conectado
        if (!sockets.isEmpty() && sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        qDebug() << buffer;
    }

}

QString Matias = "03 A5 C6 12 72";


void MainWindow::MessageReceived(const QString &msg){
    //>D,2345,z<
    //>D,0986,}<
    //>R,OK<
    //>G,OK<
    //>B,OK<
    QString tramaRecibida = msg;
    qDebug() << "Mensaje recibido en el tema:" << msg;
    qDebug() << "Contenido del mensaje:" << tramaRecibida;

    if(msg == Matias){
        ui->label->setText("Hola Matias!");
        qDebug() << "Hola Matias!" ;
        flag_habilitacion = 1;
    }
    else{
        ui->label->setText("Persona no reocnocida.");
        qDebug() << "Persona no reocnocida.";
    }

    //Chequeo de ACKS de Leds
}



void MainWindow::onNewConnection()
{
    qDebug() << "onNewConnection\r\n";
    QTcpSocket *clientSocket = server.nextPendingConnection();
    // Ready read
    connect( clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()) );

    connect( clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT( onStateChanged(QAbstractSocket::SocketState)) );
    sockets.push_back(clientSocket);

}

void MainWindow::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "onStateChanged\r\n";
    if( state == QAbstractSocket::UnconnectedState)
    {
        QTcpSocket *s = static_cast<QTcpSocket*>(QObject::sender());
        sockets.removeOne(s);
    }

}

void MainWindow::onReadyRead()
{
    qDebug() << "onReadyRead\r\n";
    QTcpSocket *s = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = s->readAll();
    QString message = data;

    for(QTcpSocket* socket : sockets )
    {
        qDebug() << data;
        MessageReceived(message);

        if( socket != s )
        {

            socket->write(data);
        }
    }
}
