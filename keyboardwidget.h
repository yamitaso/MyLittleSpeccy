#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include "zxpushbutton.h"

class KeyboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardWidget(QWidget *parent = nullptr);
    virtual ~KeyboardWidget();

public Q_SLOTS:
    void on_key_pressed(int row, int col);
    void on_key_released(int row, int col);
Q_SIGNALS:
    void key_pressed(int, int);
    void key_released(int, int);
protected:
    QVector<ZXPushButton*> buttons;
};

#endif // KEYBOARDWIDGET_H
