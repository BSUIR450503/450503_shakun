#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <string>

using namespace std;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void showServerMessage(char* message);
    void showClientMessage(const char *message);
    void showListOfFile(const char *listOfFile);

private:
    Ui::Form *ui;

signals:
    int sendMessageSignal(QString message);
    int sendListOfFile(QString listOfFile);
};

#endif // FORM_H
