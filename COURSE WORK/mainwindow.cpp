#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1050, 645);
    this->setMinimumSize(1050, 645);
    ui->textBrowser->append("Client: Клиент не подключен к серверу.");
    formForMessage = new Form();
    ftpClient.setForm(formForMessage);
    connect(formForMessage, SIGNAL(sendMessageSignal(QString)), this, SLOT(showMessage(QString)));
    connect(formForMessage, SIGNAL(sendListOfFile(QString)), this, SLOT(showListOfFile(QString)));
    connect(&dialogDeleteFile, SIGNAL(deleteFileSignal(QString)), this, SLOT(deleteFile(QString)));
    connect(&dialogGoToDir, SIGNAL(goToDirSignal(QString)), this, SLOT(goToDir(QString)));
    connect(&dialogDonwloadFile, SIGNAL(downloadFile(QString,QString)), this, SLOT(downloadFile(QString,QString)));
    connect(&dialogUploadFile, SIGNAL(uploadFile(QString)), this, SLOT(uploadFile(QString)));
}

MainWindow::~MainWindow()
{
    delete formForMessage;
    delete ui;
}

int MainWindow::showMessage(QString message)
{
    ui->textBrowser->append(message);
    return 0;
}

int MainWindow::showListOfFile(QString message)
{
    ui->textBrowser_2->clear();
    ui->textBrowser_2->setText(message);
    return 0;
}

int MainWindow::deleteFile(QString fileName)
{
    char temp[256];
    strcpy(temp, fileName.toStdString().c_str());
    ftpClient.deleteFile(temp);
    return 0;
}

int MainWindow::goToDir(QString dirName)
{
    char temp[256];
    strcpy(temp, dirName.toStdString().c_str());
    ftpClient.goToTheDirectory(temp);
    cout<<"Succses"<<endl;
    return 0;
}

int MainWindow::downloadFile(QString fileName, QString dirForSaving)
{
    char temp[256], temp1[256];
    strcpy(temp, fileName.toStdString().c_str());
     strcpy(temp1, dirForSaving.toStdString().c_str());
    ftpClient.downloadFileFromServer(temp, temp1);
    return 0;
}

int MainWindow::uploadFile(QString fileName)
{
    char temp[256], temp1[256];
    strcpy(temp, fileName.toStdString().c_str());
    QFileInfo fi(fileName);
    QString name = fi.fileName();
    strcpy(temp1, name.toStdString().c_str());
    ftpClient.uploadFileToServer(temp1, temp);

    return 0;
}

void MainWindow::on_mQuickConnection_clicked()
{
    if(ui->lineEditHost->text().size() == 0 || ui->lineEditPort->text().size() == 0)
    {
         QMessageBox::warning(this, "Внимание!", "Для соединения с сервером необходимо указать его ip адрес и порт");
    }
    else if(ui->lineEditUsername->text().size() == 0 && ui->lineEditPassw->text().size()== 0)
    {
         QMessageBox::warning(this, "Внимание!", "Для соединения с сервером необходимо указать как минимум пароль");
    }
    else
    {
        if(ui->lineEditUsername->text().size() > 0)
        {
            ftpClient.connectToServer(ui->lineEditHost->text().toStdString(), ui->lineEditPort->text().toInt(),
                                      ui->lineEditUsername->text().toStdString(), ui->lineEditPassw->text().toStdString());
        }
        else
        {
            ftpClient.connectToServer(ui->lineEditHost->text().toStdString(), ui->lineEditPort->text().toInt(),
                                      "anonymous", ui->lineEditPassw->text().toStdString());
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    this->dialogDeleteFile.show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->dialogGoToDir.show();
}

void MainWindow::on_pushButton_3_clicked()
{
    ftpClient.goToTheRootDirectory();
}

void MainWindow::on_pushButton_7_clicked()
{
    ftpClient.transmissionInterrupt();
}

void MainWindow::on_pushButton_8_clicked()
{
    ftpClient.disconnect();
}

void MainWindow::on_pushButton_5_clicked()
{
    dialogDonwloadFile.show();
}

void MainWindow::on_pushButton_6_clicked()
{
    this->dialogUploadFile.show();
}
