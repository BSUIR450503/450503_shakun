#ifndef DIALOGGOTODIR_H
#define DIALOGGOTODIR_H

#include <QDialog>

namespace Ui {
class DialogGoToDir;
}

class DialogGoToDir : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGoToDir(QWidget *parent = 0);
    ~DialogGoToDir();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogGoToDir *ui;
signals:
    int goToDirSignal(QString dirName);
};

#endif // DIALOGGOTODIR_H
