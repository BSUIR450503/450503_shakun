#include "dialogdeletefile.h"
#include "ui_dialogdeletefile.h"

DialogDeleteFile::DialogDeleteFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDeleteFile)
{
    ui->setupUi(this);
}

DialogDeleteFile::~DialogDeleteFile()
{
    delete ui;
}

void DialogDeleteFile::on_buttonBox_accepted()
{
    if(ui->lineEdit->text().size() > 0)
    {
        emit deleteFileSignal(ui->lineEdit->text());
    }
}

void DialogDeleteFile::on_buttonBox_rejected()
{
    this->close();
}
