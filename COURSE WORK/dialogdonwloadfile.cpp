#include "dialogdonwloadfile.h"
#include "ui_dialogdonwloadfile.h"

DialogDonwloadFile::DialogDonwloadFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDonwloadFile)
{
    ui->setupUi(this);
}

DialogDonwloadFile::~DialogDonwloadFile()
{
    delete ui;
}

void DialogDonwloadFile::on_pushButton_clicked()
{
   this->ui->lineEdit_2->setText(QFileDialog::getExistingDirectory(0, "Выберите путь для сохранения пакета", "/", QFileDialog::ShowDirsOnly));
}

void DialogDonwloadFile::on_buttonBox_accepted()
{
    if(ui->lineEdit_2->text().size() > 0)
    {
        emit downloadFile(ui->lineEdit->text(), ui->lineEdit_2->text());
    }
}

void DialogDonwloadFile::on_buttonBox_rejected()
{
    this->close();
}
