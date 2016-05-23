#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::showClientMessage(const char *message)
{
    string s(message);
    emit sendMessageSignal("Client: " + QString::fromStdString(s).trimmed());
}

void Form::showListOfFile(const char *listOfFile)
{
    string s(listOfFile);
    emit sendListOfFile(QString::fromStdString(s));
}

void Form::showServerMessage(char *message)
{
    string s(message);
    emit sendMessageSignal("Server: " + QString::fromStdString(s).trimmed());
}
