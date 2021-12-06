#ifndef DIALOGLOADINGTAP_H
#define DIALOGLOADINGTAP_H

#include <QDialog>

namespace Ui {
class DialogLoadingTAP;
}

class DialogLoadingTAP : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLoadingTAP(QWidget *parent = nullptr, int maxbytes = 0, const QString fileName = "none");
    ~DialogLoadingTAP();
    void setValue(int value);
    void setBlockLength(int length);
    void setBlockValue(int value);
    bool isStopped = false;



private slots:
    void on_buttonBox_rejected();

private:
    Ui::DialogLoadingTAP *ui;
};

#endif // DIALOGLOADINGTAP_H
