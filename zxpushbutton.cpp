#include "zxpushbutton.h"

ZXPushButton::ZXPushButton(int row, int col, QWidget * parent)
    : QPushButton(parent), _key_row(row), _key_col(col)
{
    connect(this, SIGNAL(pressed()),
            this, SLOT(on_pressed()));
    connect(this, SIGNAL(released()),
            this, SLOT(on_released()));
    connect(this, SIGNAL(toggled(bool)),
            this, SLOT(on_toggled(bool)));
}

void ZXPushButton::on_pressed()
{
    if (isCheckable()) return;
    emit pressed(_key_row, _key_col);
}

void ZXPushButton::on_released()
{
    if (isCheckable()) return;
    emit released(_key_row, _key_col);
}

void ZXPushButton::on_toggled(bool v)
{
    if (v) emit pressed(_key_row, _key_col);
    else emit released(_key_row, _key_col);
}
