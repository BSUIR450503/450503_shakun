#ifndef DIALOGUPLOADFILE_H
#define DIALOGUPLOADFILE_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class DialogUploadFile;
}

class DialogUploadFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUploadFile(QWidget *parent = 0);
    ~DialogUploadFile();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::DialogUploadFile *ui;
signals:
    int uploadFile(QString fileName);
};

#endif // DIALOGUPLOADFILE_H
