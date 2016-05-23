#ifndef DIALOGDONWLOADFILE_H
#define DIALOGDONWLOADFILE_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class DialogDonwloadFile;
}

class DialogDonwloadFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDonwloadFile(QWidget *parent = 0);
    ~DialogDonwloadFile();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogDonwloadFile *ui;
signals:
    int downloadFile(QString fileName, QString dirForSaving);
};

#endif // DIALOGDONWLOADFILE_H
