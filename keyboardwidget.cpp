#include "keyboardwidget.h"
#include <QGridLayout>

// row, col, colspan
enum {
    NONE = 0,
    CS = 1,
    SS = 2,
};
static const int s_button_pos[58][4] {
    { 0,  0, 2 },
    { 0,  3, 2 },
    { 0,  6, 2 },
    { 0,  9, 2 },
    { 0, 12, 2 },
    { 0, 15, 2 },
    { 0, 18, 2 },
    { 0, 21, 2 },
    { 0, 24, 2 },
    { 0, 27, 2 },
    { 0, 30, 2 },
    { 0, 33, 2 },
    { 0, 36, 3 },
    { 1,  0, 3 },
    { 1,  4, 2 },
    { 1,  7, 2 },
    { 1, 10, 2 },
    { 1, 13, 2 },
    { 1, 16, 2 },
    { 1, 19, 2 },
    { 1, 22, 2 },
    { 1, 25, 2 },
    { 1, 28, 2 },
    { 1, 31, 2 },
    { 1, 34, 2 },
    { 2,  0, 4 },
    { 2,  5, 2 },
    { 2,  8, 2 },
    { 2, 11, 2 },
    { 2, 14, 2 },
    { 2, 17, 2 },
    { 2, 20, 2 },
    { 2, 23, 2 },
    { 2, 26, 2 },
    { 2, 29, 2 },
    { 2, 32, 2 },
    { 2, 35, 4 },
    { 3,  0, 5 },
    { 3,  6, 2 },
    { 3,  9, 2 },
    { 3, 12, 2 },
    { 3, 15, 2 },
    { 3, 18, 2 },
    { 3, 21, 2 },
    { 3, 24, 2 },
    { 3, 27, 2 },
    { 3, 30, 2 },
    { 3, 33, 6 },
    { 4,  0, 2 },
    { 4,  3, 2 },
    { 4,  6, 2 },
    { 4,  9, 2 },
    { 4, 12, 2 },
    { 4, 15, 12},
    { 4, 28, 2 },
    { 4, 31, 2 },
    { 4, 34, 2 },
    { 4, 37, 2 },
};

static QString s_button_labels[58]{
     "TV",  "IV", "1",  "2", "3", "4",   "5", "6", "7", "8", "9", "0",  "BRK",
     "DEL", "GR", "Q",  "W", "E", "R",   "T", "Y", "U", "I", "O", "P",
     "EM",  "ED", "A",  "S", "D", "F",   "G", "H", "J", "K", "L", "ENT",
     "CS",  "CL", "Z",  "X", "C", "V",   "B", "N", "M", ".", "CS",
     "SS",  ";",  ",,", "˂", "˃", "SPC", "˄", "˅", ",", "SS",
};

static int s_button_bits[58][3]{
    {2, 11,  CS }, {3, 11,  CS }, {0, 11, NONE}, {1, 11, NONE}, {2, 11, NONE}, {3, 11, NONE}, {4, 11, NONE}, {4, 12, NONE}, {3, 12, NONE}, {2, 12, NONE}, {1, 12, NONE}, {0, 12, NONE}, {0, 15,  CS },
    {0, 12,  CS }, {1, 12,  CS }, {0, 10, NONE}, {1, 10, NONE}, {2, 10, NONE}, {3, 10, NONE}, {4, 10, NONE}, {4, 13, NONE}, {3, 13, NONE}, {2, 13, NONE}, {1, 13, NONE}, {0, 13, NONE},
    {1, 15,  CS }, {0, 11,  CS }, {0,  9, NONE}, {1,  9, NONE}, {2,  9, NONE}, {3,  9, NONE}, {4,  9, NONE}, {4, 14, NONE}, {3, 14, NONE}, {2, 14, NONE}, {1, 14, NONE}, {0, 14, NONE},
    {0,  8, NONE}, {1, 11,  CS }, {1,  8, NONE}, {2,  8, NONE}, {3,  8, NONE}, {4,  8, NONE}, {4, 15, NONE}, {3, 15, NONE}, {2, 15, NONE}, {2, 15,  SS }, {0,  8, NONE},
    {1, 15, NONE}, {1, 13,  SS }, {0, 13,  SS }, {4, 11,  CS }, {2, 12,  CS }, {0, 15, NONE}, {3, 12,  CS }, {4, 12,  CS }, {3, 15,  SS }, {1, 15, NONE},

};
static QString s_button_hint[58]{
     "True Video (F1)", "Inverse Video (F2)", "",  "", "", "",   "", "", "", "", "", "",  "Break (backspace)",
     "Delete (del)", "Graph (F3)", "",  "", "", "",   "", "", "", "", "", "",
     "Extend Mode (F4)",  "Edit (F5)", "",  "", "", "",   "", "", "", "", "", "Enter",
     "Caps Shift (Shift)",  "Caps Lock (Caps Lock)", "",  "", "", "",   "", "", "", "", "Caps Shift (Shift)",
     "Symbol Shift",  "",  "", "", "", "Space", "", "", "", "Symbol Shift",
};
KeyboardWidget::KeyboardWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout * layout = new QGridLayout(this);

    setLayout(layout);
    for (int i = 0; i< 58; ++i){
        ZXPushButton * button = new ZXPushButton(s_button_bits[i][0],s_button_bits[i][1],s_button_bits[i][2],this);
        if ((button->row() == 0 and button->col() == 8 and button->add()==NONE) or (button->row() == 1 and button->col() == 15 and button->add()==NONE)){
            button->setCheckable(true);
        }
        connect(button,
                SIGNAL(pressed(int,int,int)),
                this,
                SLOT(on_key_pressed(int,int,int)));
        connect(button,
                SIGNAL(released(int,int,int)),
                this,
                SLOT(on_key_released(int,int,int)));
        button->setText(s_button_labels[i]);
        button->setToolTip(s_button_hint[i]);
        layout->addWidget(button, s_button_pos[i][0], s_button_pos[i][1], 1, s_button_pos[i][2]);
        buttons.append(button);


    }
}

KeyboardWidget::~KeyboardWidget()
{
    qDeleteAll(buttons);
}

void KeyboardWidget::on_key_pressed(int row, int col, int add)
{
    emit key_pressed(row, col, add);
}

void KeyboardWidget::on_key_released(int row, int col, int add)
{
    emit key_released(row, col, add);
}
