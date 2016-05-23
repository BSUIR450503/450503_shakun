#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "form.h"
#include "client.h"
#include "dialogdeletefile.h"
#include "dialoggotodir.h"
#include "dialogdonwloadfile.h"
#include "dialoguploadfile.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Form *formForMessage;
    Client ftpClient;
    DialogDeleteFile dialogDeleteFile;
    DialogGoToDir  dialogGoToDir;
    DialogDonwloadFile dialogDonwloadFile;
    DialogUploadFile dialogUploadFile;

public slots:
    int showMessage(QString message);
    int showListOfFile(QString message);
    int deleteFile(QString fileName);
    int goToDir(QString dirName);
    int downloadFile(QString fileName, QString dirForSaving);
    int uploadFile(QString fileName);
private slots:
    void on_mQuickConnection_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
};

#endif // MAINWINDOW_H
