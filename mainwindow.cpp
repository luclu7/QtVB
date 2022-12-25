#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTextFile();
    getSerialPorts();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::on_GenNGBtn_clicked()
{
    generateAndSetNG();
}

void MainWindow::loadTextFile()
{
    ui->textEdit->setPlainText("...");
    ui->disconnectSerialBtn->setVisible(false);
    ui->serialPortsBox->setCurrentText("ttyACM0");
}

void MainWindow::setTextToSerialPortError(void) {
    ui->serialStateLineEdit->clear();
    ui->serialStateLineEdit->setText(port->errorString());
    ui->serialStateLineEdit->setStyleSheet("background-color: rgba(255, 204, 203, 0.5);");
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
        this->port = new QSerialPort(ui->serialPortsBox->currentText());
    //    this->port->open(QIODevice::ReadWrite);

    if(this->port->open(QIODevice::ReadWrite)){
        if(!this->port->setBaudRate(ui->baudRatesBox->currentText().toInt())) {
        }


        if(!this->port->setDataBits(QSerialPort::Data8)) {
            setTextToSerialPortError();
        }

        if(!this->port->setParity(QSerialPort::NoParity)) {
            setTextToSerialPortError();
        }

        if(!this->port->setStopBits(QSerialPort::OneStop)) {
            setTextToSerialPortError();
        }

        if(!this->port->setFlowControl(QSerialPort::NoFlowControl)) {
            setTextToSerialPortError();
        }

        ui->genSendSerialBtn->setEnabled(true);
        ui->GenSerialNGBtn->setEnabled(true);
        ui->connectSerialBtn->setVisible(false);
        ui->disconnectSerialBtn->setVisible(true);
        ui->disconnectSerialBtn->setEnabled(true);
        ui->serialStateLineEdit->setText("Connecté");
        ui->serialStateLineEdit->setStyleSheet("");

    } else {
        setTextToSerialPortError();
    }

    qDebug("port ouvert: %d", this->port->isOpen());


    // Connect the readyRead() signal of the serial port to a slot that reads data
    // from the serial port and appends it to the QTextEdit widget
    connect(port, &QSerialPort::readyRead, this, &MainWindow::readData);

    if(this->port->isOpen()) {
        //ui->connectSerialBtn->setText("Disonnect serial");
    }
}

void MainWindow::readData()
{
    QByteArray data = this->port->readAll();
    QString dataStr = QString::fromUtf8(data);

    this->inData += dataStr;

//    ui->SerialTextEdit->append(QString::fromUtf8(data));
    ui->SerialTextEdit->moveCursor (QTextCursor::End);
    ui->SerialTextEdit->insertPlainText(dataStr);

    qDebug() << this->inData[inData.length()-1];

    if(dataStr[dataStr.length()-1] == '\n') {
        QVariantMap a = parseJson(inData.toLocal8Bit());
        ui->JSONTextEditNG_2->setText(inData);

        this->inData.clear();

        ui->DecVALBtn->setChecked(a["VAL"].toBool());
        ui->DecMVBtn->setChecked(a["MV"].toBool());
        ui->DecFCBtn->setChecked(a["FC"].toBool());
        ui->DecTESTBtn->setChecked(a["TEST"].toBool());
        ui->DecBPSFBtn->setChecked(a["SF"].toBool());
    }
}

void MainWindow::getSerialPorts(void) {
    ui->serialPortsBox->clear();
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->serialPortsBox->addItem(port.portName());
    }

    foreach(const int baudRate, QSerialPortInfo::standardBaudRates()) {
        ui->baudRatesBox->addItem(QString::number(baudRate));
    }

    ui->baudRatesBox->setCurrentText("115200");
}

void MainWindow::on_genSendSerialBtn_clicked()
{
    QString toSend = generateAndSet();
    this->port->write(toSend.toStdString().c_str(), toSend.size());

}


void MainWindow::on_disconnectSerialBtn_clicked()
{
    this->port->close();
    ui->serialStateLineEdit->setText("Déconnecté");
    ui->genSendSerialBtn->setEnabled(false);
    ui->GenSerialNGBtn->setEnabled(false);
    ui->connectSerialBtn->setVisible(true);
    ui->disconnectSerialBtn->setVisible(false);
    ui->disconnectSerialBtn->setEnabled(false);
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

void MainWindow::on_scanSerialPortBtn_clicked()
{
    getSerialPorts();
}


void MainWindow::on_cleanBtn_clicked()
{
    ui->SerialTextEdit->clear();
}


void MainWindow::on_parseJsonBtn_clicked()
{

    QString data = ui->JSONTextEditNG_2->toPlainText();

    QVariantMap parsedJson = parseJson(data.toLocal8Bit());

    ui->DecVALBtn->setChecked(parsedJson["VAL"].toBool());
    ui->DecMVBtn->setChecked(parsedJson["MV"].toBool());
    ui->DecFCBtn->setChecked(parsedJson["FC"].toBool());
    ui->DecTESTBtn->setChecked(parsedJson["TEST"].toBool());
    ui->DecBPSFBtn->setChecked(parsedJson["SF"].toBool());

}

QVariantMap MainWindow::parseJson(QByteArray json_bytes) {
    //QByteArray json_bytes = jsonText.toLocal8Bit();

    // step 3
    auto json_doc = QJsonDocument::fromJson(json_bytes);

    if (json_doc.isNull()) {
        ui->serialStateLineEdit->setText("Failed to create JSON doc.");
    }

    if (!json_doc.isObject()) {
        ui->serialStateLineEdit->setText("JSON is not an object.");
    }

    QJsonObject json_obj = json_doc.object();

    if (json_obj.isEmpty()) {
        ui->serialStateLineEdit->setText("JSON object is empty.");
    }

    // step 4
    QVariantMap a = json_obj.toVariantMap();

    return a;
}


void MainWindow::on_LSSFbtn_toggled(bool checked)
{
    if(checked) {
        ui->LSSFbtn->setIcon(QIcon(":/img/LSSF_ON.png"));
    } else {
        ui->LSSFbtn->setIcon(QIcon(":/img/LSSF_OFF.png"));
    }
}

void MainWindow::setAllButtons(bool value) {
    ui->VALbtn->setChecked(value);
    ui->MVbtn->setChecked(value);
    ui->FCbtn->setChecked(value);
    ui->LSSFbtn->setChecked(value);

    ui->LSVBtn->setChecked(value);
    ui->LSFUBtn->setChecked(value);
    ui->PanneSolBtn->setChecked(value);
    ui->PanneEnginBtn->setChecked(value);

}

void MainWindow::on_clearAll_clicked()
{
    setAllButtons(false);
}

void MainWindow::on_lightAll_clicked()
{
    setAllButtons(true);
}


void MainWindow::on_invertLights_clicked()
{
    ui->VALbtn->toggle();
    ui->MVbtn->toggle();
    ui->FCbtn->toggle();
    ui->LSSFbtn->toggle();

    ui->LSVBtn->toggle();
    ui->LSFUBtn->toggle();
    ui->PanneSolBtn->toggle();
    ui->PanneEnginBtn->toggle();

}

