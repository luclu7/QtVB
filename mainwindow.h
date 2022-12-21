#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QClipboard>
#include <QSerialPort>


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

    void on_VALbtn_toggled(bool checked);

    void on_MVbtn_toggled(bool checked);

    void on_FCbtn_toggled(bool checked);

    void on_LSSFbtn_toggled(bool checked);

    void on_genNGBtn_clicked();

    void on_GenCopyNGBtn_clicked();

    void on_GenSerialNGBtn_clicked();

    void on_LSVBtn_toggled(bool checked);

    void on_LSFUBtn_toggled(bool checked);

    void on_PanneSolBtn_toggled(bool checked);

    void on_PanneEnginBtn_toggled(bool checked);

private:
    QSerialPort *port;
    Ui::MainWindow *ui;
    void loadTextFile(void);
    QString generateJSON(void);
    QString generateJSONNG(void);
    QString generateAndSet(void);
    QString generateAndSetNG(void);
};
#endif // MAINWINDOW_H
