#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTextFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(m_ui->actionConfigure, &QAction::triggered, m_settings, &SettingsDialog::show);
    connect(m_ui->actionClear, &QAction::triggered, m_console, &Console::clear);
    connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}*/

void MainWindow::on_connectBtn_clicked()
{
    //QString searchString = ui->lineEdit->text();
    //ui->textEdit->find(searchString, QTextDocument::FindWholeWords);

    generateAndSet();

}

void MainWindow::on_generateCopyBtn_clicked()
{
    QString toCopy = generateAndSet();

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(toCopy);

}


QString MainWindow::generateAndSet() {
    QString strJson = generateJSON()+"\n";

    ui->textEdit->clear();
    ui->textEdit->insertPlainText(strJson);

    return strJson;
}

QString MainWindow::generateAndSetNG() {
    QString strJson = generateJSONNG()+"\n";

    ui->JSONTextEditNG->clear();
    ui->JSONTextEditNG->insertPlainText(strJson);

    return strJson;
}

QString MainWindow::generateJSON() {
    QJsonObject recordObject;

    recordObject.insert("LSSF2", QJsonValue::fromVariant(ui->VALbtn->isChecked() ? 1 : 0));

    recordObject.insert("LSSF", QJsonValue::fromVariant(ui->LSSF->checkState() == 2 ? 1 : 0));
    recordObject.insert("VAL", QJsonValue::fromVariant(ui->VAL->checkState() == 2 ? 1 : 0));
    recordObject.insert("MV", QJsonValue::fromVariant(ui->MV->checkState() == 2 ? 1 : 0));
    recordObject.insert("FC", QJsonValue::fromVariant(ui->FC->checkState() == 2 ? 1 : 0));
    recordObject.insert("ENGIN", QJsonValue::fromVariant(ui->Panne_engin->checkState() == 2 ? 1 : 0));
    recordObject.insert("SOL", QJsonValue::fromVariant(ui->Panne_sol->checkState() == 2 ? 1 : 0));
    recordObject.insert("LSV", QJsonValue::fromVariant(ui->LSV->checkState() == 2 ? 1 : 0));
    recordObject.insert("LSFU", QJsonValue::fromVariant(ui->LSFU->checkState() == 2 ? 1 : 0));
    recordObject.insert("autotest", QJsonValue::fromVariant(ui->spinBox_autotest->value()));
    recordObject.insert("visu", QJsonValue::fromVariant(ui->spinBox_visu->value()));

    QJsonDocument doc(recordObject);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

QString MainWindow::generateJSONNG() {
    QJsonObject recordObject;

    recordObject.insert("LSSF", QJsonValue::fromVariant(ui->LSSFbtn->isChecked() ? 1 : 0));

    recordObject.insert("VAL", QJsonValue::fromVariant(ui->VALbtn->isChecked() ? 1 : 0));
    recordObject.insert("MV", QJsonValue::fromVariant(ui->MVbtn->isChecked() ? 1 : 0));
    recordObject.insert("FC", QJsonValue::fromVariant(ui->FCbtn->isChecked() ? 1 : 0));

    recordObject.insert("ENGIN", QJsonValue::fromVariant(ui->PanneEnginBtn->isChecked() ? 1 : 0));
    recordObject.insert("SOL", QJsonValue::fromVariant(ui->PanneSolBtn->isChecked() ? 1 : 0));
    recordObject.insert("LSV", QJsonValue::fromVariant(ui->LSVBtn->isChecked() ? 1 : 0));
    recordObject.insert("LSFU", QJsonValue::fromVariant(ui->LSFUBtn->isChecked() ? 1 : 0));

    recordObject.insert("autotest", QJsonValue::fromVariant(ui->spinBox_autotest->value()));
    recordObject.insert("visu", QJsonValue::fromVariant(ui->spinBox_visu->value()));

    QJsonDocument doc(recordObject);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

void MainWindow::loadTextFile()
{
    ui->textEdit->setPlainText("...");
    ui->disconnectSerialBtn->setVisible(false);
    ui->serialPortName->setText("/dev/ttyACM0");
}

void MainWindow::on_connectSerialBtn_clicked()
{

    /*if(this->port->isOpen()) {
        this->port->close();
        if(!this->port->isOpen()) {
            ui->connectSerialBtn->setText("Connect serial");
        }
        delete this->port;
    } else {*/
        this->port = new QSerialPort(ui->serialPortName->displayText());
    //    this->port->open(QIODevice::ReadWrite);

    if(this->port->open(QIODevice::ReadWrite)){
        if(!this->port->setBaudRate(QSerialPort::Baud115200)) {
            qDebug()<<this->port->errorString();
            ui->textEdit->clear();
            ui->textEdit->insertPlainText(port->errorString());
        }


        if(!this->port->setDataBits(QSerialPort::Data8)) {
            qDebug()<<this->port->errorString();
            ui->textEdit->clear();
            ui->textEdit->insertPlainText(port->errorString());
        }

        if(!this->port->setParity(QSerialPort::NoParity)) {
            qDebug()<<this->port->errorString();
            ui->textEdit->clear();
            ui->textEdit->insertPlainText(port->errorString());
        }

        if(!this->port->setStopBits(QSerialPort::OneStop)) {
            qDebug()<<this->port->errorString();
            ui->textEdit->clear();
            ui->textEdit->insertPlainText(port->errorString());
        }

        if(!this->port->setFlowControl(QSerialPort::NoFlowControl))
            qDebug()<<this->port->errorString(); {
            ui->textEdit->clear();
            ui->textEdit->insertPlainText(port->errorString());
        }

        ui->genSendSerialBtn->setEnabled(true);
        ui->GenSerialNGBtn->setEnabled(true);
        ui->connectSerialBtn->setVisible(false);
        ui->disconnectSerialBtn->setVisible(true);
        ui->disconnectSerialBtn->setEnabled(true);


    } else {
        ui->textEdit->clear();
        ui->textEdit->insertPlainText(port->errorString());
    }


        qDebug("port ouvert: %d", this->port->isOpen());

        if(this->port->isOpen()) {
            //ui->connectSerialBtn->setText("Disonnect serial");
        }

    //}
}


void MainWindow::on_genSendSerialBtn_clicked()
{
    QString toSend = generateAndSet();
    this->port->write(toSend.toStdString().c_str(), toSend.size());

}


void MainWindow::on_disconnectSerialBtn_clicked()
{

    this->port->close();
    ui->genSendSerialBtn->setEnabled(false);
    ui->GenSerialNGBtn->setEnabled(false);
    ui->connectSerialBtn->setVisible(true);
    ui->disconnectSerialBtn->setVisible(false);
    ui->disconnectSerialBtn->setEnabled(false);

}


void MainWindow::on_VALbtn_toggled(bool checked)
{
    if(checked) {
        ui->VALbtn->setIcon(QIcon(":/img/VAL_ON.png"));
    } else {
        ui->VALbtn->setIcon(QIcon(":/img/VAL_OFF.png"));
    }
}


void MainWindow::on_MVbtn_toggled(bool checked)
{
    if(checked) {
        ui->MVbtn->setIcon(QIcon(":/img/MV_ON.png"));
    } else {
        ui->MVbtn->setIcon(QIcon(":/img/MV_OFF.png"));
    }
}

void MainWindow::on_FCbtn_toggled(bool checked)
{
    if(checked) {
        ui->FCbtn->setIcon(QIcon(":/img/FC_ON.png"));
    } else {
        ui->FCbtn->setIcon(QIcon(":/img/FC_OFF.png"));
    }
}


void MainWindow::on_LSSFbtn_toggled(bool checked)
{
    if(checked) {
        ui->LSSFbtn->setIcon(QIcon(":/img/LSSF_ON.png"));
    } else {
        ui->LSSFbtn->setIcon(QIcon(":/img/LSSF_OFF.png"));
    }
}

void MainWindow::on_genNGBtn_clicked()
{
    generateAndSetNG();
}


void MainWindow::on_GenCopyNGBtn_clicked()
{
    QString toCopy = generateAndSetNG();

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(toCopy);
}


void MainWindow::on_GenSerialNGBtn_clicked()
{
    QString toSend = generateAndSetNG();
    this->port->write(toSend.toStdString().c_str(), toSend.size());
}


void MainWindow::on_LSVBtn_toggled(bool checked)
{
    if(checked) {
        ui->LSVBtn->setIcon(QIcon(":/img/Lampe_ON.png"));
    } else {
        ui->LSVBtn->setIcon(QIcon(":/img/Lampe_OFF.png"));
    }
}


void MainWindow::on_LSFUBtn_toggled(bool checked)
{
    if(checked) {
        ui->LSFUBtn->setIcon(QIcon(":/img/Lampe_ON.png"));
    } else {
        ui->LSFUBtn->setIcon(QIcon(":/img/Lampe_OFF.png"));
    }
}


void MainWindow::on_PanneSolBtn_toggled(bool checked)
{
    if(checked) {
        ui->PanneSolBtn->setIcon(QIcon(":/img/Lampe_ON.png"));
    } else {
        ui->PanneSolBtn->setIcon(QIcon(":/img/Lampe_OFF.png"));
    }
}


void MainWindow::on_PanneEnginBtn_toggled(bool checked)
{
    if(checked) {
        ui->PanneEnginBtn->setIcon(QIcon(":/img/Lampe_engin_ON.png"));
    } else {
        ui->PanneEnginBtn->setIcon(QIcon(":/img/Lampe_OFF.png"));
    }
}

