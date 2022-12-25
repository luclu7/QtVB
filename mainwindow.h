#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QClipboard>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextCursor>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectBtn_clicked();

    void on_generateCopyBtn_clicked();

    void on_connectSerialBtn_clicked();

    void on_genSendSerialBtn_clicked();

    void on_disconnectSerialBtn_clicked();

    void on_GenNGBtn_clicked();

    void on_GenCopyNGBtn_clicked();

    void on_GenSerialNGBtn_clicked();

    void on_scanSerialPortBtn_clicked();

    void on_cleanBtn_clicked();

    void on_parseJsonBtn_clicked();

    void on_LSSFbtn_toggled(bool checked);

    void on_clearAll_clicked();

    void on_lightAll_clicked();

    void on_invertLights_clicked();

private:
    QSerialPort *port;
    QString inData;
    Ui::MainWindow *ui;
    void loadTextFile(void);
    QString generateJSON(void);
    QString generateJSONNG(void);
    QString generateAndSet(void);
    QString generateAndSetNG(void);
    void getSerialPorts(void);
    void readData(void);
    void setTextToSerialPortError();
    QVariantMap parseJson(QByteArray);
    void setAllButtons(bool);

};
#endif // MAINWINDOW_H
