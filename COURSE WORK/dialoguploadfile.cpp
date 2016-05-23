#include "dialoguploadfile.h"
#include "ui_dialoguploadfile.h"

DialogUploadFile::DialogUploadFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUploadFile)
{
    ui->setupUi(this);
}

DialogUploadFile::~DialogUploadFile()
{
    delete ui;
}

void DialogUploadFile::on_pushButton_clicked()
{
    ui->lineEdit_2->setText(QFileDialog::getOpenFileName(0,
                                                     QObject::tr("Укажите файл"),
                                                     QCoreApplication::applicationDirPath (),
                                                   NULL ));

}

void DialogUploadFile::on_buttonBox_rejected()
{
    this->close();
}

void DialogUploadFile::on_buttonBox_accepted()
{
    if(ui->lineEdit_2->text().size() > 0)
    {
        emit uploadFile(ui->lineEdit_2->text());
    }
}
