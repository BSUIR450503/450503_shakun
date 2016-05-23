#ifndef DIALOGDELETEFILE_H
#define DIALOGDELETEFILE_H

#include <QDialog>

namespace Ui {
class DialogDeleteFile;
}

class DialogDeleteFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDeleteFile(QWidget *parent = 0);
    ~DialogDeleteFile();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogDeleteFile *ui;

signals:
    int deleteFileSignal(QString fileName);
};

#endif // DIALOGDELETEFILE_H
