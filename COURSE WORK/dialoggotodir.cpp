#include "dialoggotodir.h"
#include "ui_dialoggotodir.h"

DialogGoToDir::DialogGoToDir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGoToDir)
{
    ui->setupUi(this);
}

DialogGoToDir::~DialogGoToDir()
{
    delete ui;
}

void DialogGoToDir::on_buttonBox_accepted()
{
    if(ui->lineEdit->text().size() > 0)
    {
        emit goToDirSignal(ui->lineEdit->text());
    }
}

void DialogGoToDir::on_buttonBox_rejected()
{
    this->close();
}
