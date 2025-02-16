#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace Qt::StringLiterals;

int flag_habilitacion = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Establecer estilos de los botones
    ui->pushButton_LedRed->setStyleSheet("background-color: rgb(200, 150, 150);");
    ui->pushButton_LedGreen->setStyleSheet("background-color: rgb(150, 200, 150);");
    ui->pushButton_LedBlue->setStyleSheet("background-color: rgb(150, 150, 200);");

    // (Opcional) Si deseas configurar el estado de los LEDs, puedes descomentar estas líneas:
    // ui->label_LedRed->setColour(LED_RED_GRAYED);
    // ui->label_LedGreen->setColour(LED_GREEN_GRAYED);
    // ui->label_LedBlue->setColour(LED_BLUE_GRAYED);

    server.listen(QHostAddress::Any, PORTsERVER);
    // Conectar la señal de nueva conexión
    connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_LedGreen_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,N,";
        char chk = 0;
        // Calcular el checksum mediante XOR de cada carácter
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay al menos un socket conectado y en estado conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        ui->label->setText("Esperando confirmacion del Lavado Normal...");
        qDebug() << buffer;
    }
}

void MainWindow::on_pushButton_LedRed_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,R,";
        char chk = 0;
        // Calcular el checksum mediante XOR
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay un socket conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }
        ui->label->setText("Esperando confirmacion del Rapido...");
        qDebug() << buffer;
    }
}

void MainWindow::on_pushButton_LedBlue_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,C,";
        char chk = 0;
        // Calcular el checksum mediante XOR
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay un socket conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        ui->label->setText("Esperando confirmacion del Centrifugado...");
        qDebug() << buffer;
    }
}

QString Matias = "03 A5 C6 12 72";

QString MainWindow::generateChecksum(const QString &data) {
    char chk = 0;
    for (int i = 1; i < data.size() - 2; i++) { // Excluir el '>' inicial y el '<' final
        chk ^= data[i].toLatin1();
    }
    return QString::number(static_cast<unsigned char>(chk), 16).toUpper();
}

void MainWindow::MessageReceived(const QString &msg)
{
    QString tramaRecibida = msg;
    qDebug() << "Mensaje recibido en el tema:" << msg;
    qDebug() << "Contenido del mensaje:" << tramaRecibida;

    // Comparar el mensaje recibido con el string preestablecido
    if (msg == Matias) {
        ui->label->setText("Hola Matias!");
        qDebug() << "Hola Matias!";
        flag_habilitacion = 1;
    }
    else if(msg == ">L,A,1<")
    {
        ui->label->setText("Lavado Aceptado!");
    }
    else if(msg == ">L,P,1<"){
        ui->label->setText("Equipo detenido!");
    }
    else {
        ui->label->setText("Persona no reocnocida.");
        qDebug() << "Persona no reocnocida.";
    }
    // Aquí se podrían agregar comprobaciones de ACKs de LEDs u otros mensajes
}

void MainWindow::onNewConnection()
{
    qDebug() << "onNewConnection\r\n";
    QTcpSocket *clientSocket = server.nextPendingConnection();

    // Conectar las señales readyRead y stateChanged
    connect(clientSocket, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onStateChanged(QAbstractSocket::SocketState)));

    sockets.push_back(clientSocket);
}

void MainWindow::onStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "onStateChanged\r\n";
    if (state == QAbstractSocket::UnconnectedState) {
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

    // Enviar el mensaje recibido a todos los sockets conectados
    for (QTcpSocket *socket : sockets) {
        qDebug() << data;
        MessageReceived(message);
        if (socket != s) {
            socket->write(data);
        }
    }
}

void MainWindow::on_paradaButton_clicked()
{
    if (flag_habilitacion) {
        // Construir la trama a enviar
        QString buffer = ">L,P,";
        char chk = 0;
        // Calcular el checksum mediante XOR
        for (int i = 0; i < buffer.size(); i++) {
            chk ^= buffer.toStdString()[i];
        }
        // Agregar el checksum, delimitador de fin y salto de línea
        buffer.append(chk);
        buffer.append('<');
        buffer.append('\n');

        // Enviar la trama si hay un socket conectado
        if (!sockets.isEmpty() &&
            sockets.at(0)->state() == QAbstractSocket::ConnectedState) {
            sockets.at(0)->write(buffer.toUtf8());
            sockets.at(0)->flush();
        }

        ui->label->setText("Enviando parada de emergencia...");
        qDebug() << buffer;
    }
}

