#include "dialogloadingtap.h"
#include "ui_dialogloadingtap.h"

DialogLoadingTAP::DialogLoadingTAP(QWidget *parent, int maxbytes, const QString fileName) :
    QDialog(parent),
    ui(new Ui::DialogLoadingTAP)
{
    ui->setupUi(this);
    ui->fileName->setText(fileName);
    ui->progressBar->setMaximum(maxbytes);
}

DialogLoadingTAP::~DialogLoadingTAP()
{
    delete ui;
}

void DialogLoadingTAP::setValue(int value)
{
    ui->progressBar->setValue(value);
}

void DialogLoadingTAP::setBlockLength(int length)
{
    ui->progressBar_block->setMaximum(length);
}

void DialogLoadingTAP::setBlockValue(int value)
{
    ui->progressBar_block->setValue(value);
}

void DialogLoadingTAP::on_buttonBox_rejected()
{
    isStopped = true;
}
