#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QKeyEvent>
#include <QMap>
#include <QFileDialog>
#include "businterface48.h"
#include "businterface128.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QtGamepad>
static constexpr int PAIR(int a, int b) { return a * 100 + b; }
static constexpr int FIRST(int v) { return v / 100; }
static constexpr int SECOND(int v) { return v % 100; }
QPalette pal;
QColor dColor;
static const QMap<int, int> s_key_mapping{
#if defined(WIN32)
    { 2,  PAIR(0, 11) },
    { 3,  PAIR(1, 11) },
    { 4,  PAIR(2, 11) },
    { 5,  PAIR(3, 11) },
    { 6,  PAIR(4, 11) },
    { 7,  PAIR(4, 12) },
    { 8,  PAIR(3, 12) },
    { 9,  PAIR(2, 12) },
    { 10, PAIR(1, 12) },
    { 11, PAIR(0, 12) },

    { 16, PAIR(0, 10) },
    { 17, PAIR(1, 10) },
    { 18, PAIR(2, 10) },
    { 19, PAIR(3, 10) },
    { 20, PAIR(4, 10) },
    { 21, PAIR(4, 13) },
    { 22, PAIR(3, 13) },
    { 23, PAIR(2, 13) },
    { 24, PAIR(1, 13) },
    { 25, PAIR(0, 13) },

    { 30, PAIR(0, 9) },
    { 31, PAIR(1, 9) },
    { 32, PAIR(2, 9) },
    { 33, PAIR(3, 9) },
    { 34, PAIR(4, 9) },
    { 35, PAIR(4, 14) },
    { 36, PAIR(3, 14) },
    { 37, PAIR(2, 14) },
    { 38, PAIR(1, 14) },
    { 28, PAIR(0, 14) },

    { 42, PAIR(0, 8) },
    { 44, PAIR(1, 8) },
    { 45, PAIR(2, 8) },
    { 46, PAIR(3, 8) },
    { 47, PAIR(4, 8) },
    { 48, PAIR(4, 15) },
    { 49, PAIR(3, 15) },
    { 50, PAIR(2, 15) },
    { 54, PAIR(1, 15) },
    { 57, PAIR(0, 15) },
#else
    { 10 , PAIR(0, 11) },
    { 11 , PAIR(1, 11) },
    { 12 , PAIR(2, 11) },
    { 13 , PAIR(3, 11) },
    { 14 , PAIR(4, 11) },
    { 15 , PAIR(4, 12) },
    { 16 , PAIR(3, 12) },
    { 17 , PAIR(2, 12) },
    { 18 , PAIR(1, 12) },
    { 19 , PAIR(0, 12) },

    { 24 , PAIR(0, 10) },
    { 25 , PAIR(1, 10) },
    { 26 , PAIR(2, 10) },
    { 27 , PAIR(3, 10) },
    { 28 , PAIR(4, 10) },
    { 29 , PAIR(4, 13) },
    { 30 , PAIR(3, 13) },
    { 31 , PAIR(2, 13) },
    { 32 , PAIR(1, 13) },
    { 33 , PAIR(0, 13) },

    { 38 , PAIR(0,  9) },
    { 39 , PAIR(1,  9) },
    { 40 , PAIR(2,  9) },
    { 41 , PAIR(3,  9) },
    { 42 , PAIR(4,  9) },
    { 43 , PAIR(4, 14) },
    { 44 , PAIR(3, 14) },
    { 45 , PAIR(2, 14) },
    { 46 , PAIR(1, 14) },
    { 36 , PAIR(0, 14) },

    { 50 , PAIR(0,  8) },
    { 52 , PAIR(1,  8) },
    { 53 , PAIR(2,  8) },
    { 54 , PAIR(3,  8) },
    { 55 , PAIR(4,  8) },
    { 56 , PAIR(4, 15) },
    { 57 , PAIR(3, 15) },
    { 58 , PAIR(2, 15) },
    { 62 , PAIR(1, 15) },
    { 65 , PAIR(0, 15) },

#endif
};
enum {
    CURSOR_IF = 0,
    KEMPSTON_IF = 1,
    SINCLAIR_IF2_P1 = 2,
    SINCLAIR_IF2_P2 = 3,
};
static constexpr int ESC_SCANCODE = 9;
static constexpr int F11_SCANCODE = 95;
static constexpr int F12_SCANCODE = 96;
static constexpr int UP_SCANCODE = 111;
static constexpr int DOWN_SCANCODE = 116;
static constexpr int LEFT_SCANCODE = 113;
static constexpr int RIGHT_SCANCODE = 114;
static constexpr int LCTRL_SCANCODE = 37;

static uint8_t s_mem_read(void *context, uint16_t addr){
    BusInterface * bi = reinterpret_cast<BusInterface*>(context);
    return bi->mem_read8(addr);
}
static void s_mem_write(void *context, uint16_t addr, uint8_t value){
    BusInterface * bi = reinterpret_cast<BusInterface*>(context);
    bi->mem_write8(addr,value);
}
static uint8_t s_port_read(void *context, uint16_t addr){
    BusInterface * bi = reinterpret_cast<BusInterface*>(context);
    return bi->io_read8(addr);
}
static void s_port_write(void *context, uint16_t addr, uint8_t value){
    BusInterface * bi = reinterpret_cast<BusInterface*>(context);
    bi->io_write8(addr,value);
}
static uint32_t s_int_data(void *context){
    Q_UNUSED(context);
    return 0xC3000000;
}
static void s_halt(void * context, uint8_t state){
    Q_UNUSED(context); Q_UNUSED(state);
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->showControlsCB->setChecked(false);
    ui->controlsTab->setVisible(false);

    bus = new BusInterface128();

    ui->screen->setBusInterface(bus);
    connect(ui->kbwidget,
            SIGNAL(key_pressed(int,int)),
            this,
            SLOT(on_key_pressed(int,int)));
    connect(ui->kbwidget,
            SIGNAL(key_released(int,int)),
            this,
            SLOT(on_key_released(int,int)));
    setFocusPolicy(Qt::StrongFocus);

    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(ui->upButton,    SIGNAL(pressed()), this, SLOT(upPressed()));
    connect(ui->downButton,  SIGNAL(pressed()), this, SLOT(downPressed()));
    connect(ui->leftButton,  SIGNAL(pressed()), this, SLOT(leftPressed()));
    connect(ui->rightButton, SIGNAL(pressed()), this, SLOT(rightPressed()));
    connect(ui->fireButton,  SIGNAL(pressed()), this, SLOT(firePressed()));

    connect(ui->upButton,    SIGNAL(released()), this, SLOT(upReleased()));
    connect(ui->downButton,  SIGNAL(released()), this, SLOT(downReleased()));
    connect(ui->leftButton,  SIGNAL(released()), this, SLOT(leftReleased()));
    connect(ui->rightButton, SIGNAL(released()), this, SLOT(rightReleased()));
    connect(ui->fireButton,  SIGNAL(released()), this, SLOT(fireReleased()));


    bus->io_write8(0xfe, 3);

    cpustate.context = bus;
    cpustate.read = s_mem_read;
    cpustate.write = s_mem_write;
    cpustate.in = s_port_read;
    cpustate.out = s_port_write;
    cpustate.int_data = s_int_data;
    cpustate.halt = s_halt;


    reset();
    pal = palette();
    dColor = pal.color(QPalette::Window);
    for (uint32_t addr = 16384; addr < 16384 + 32*192 + 32*24; ++addr){
        bus->mem_write8(addr,addr & 0xff);
    }

    frame_timer = new QTimer();

    connect(    frame_timer,
                SIGNAL(timeout()),
                this,
                SLOT(frameRefresh()));
    frame_timer->start(1000/50);
    flash_timer = new QTimer();
    connect(    flash_timer,
                SIGNAL(timeout()),
                ui->screen,
                SLOT(toggleFlash()));
    flash_timer->start(320);

    gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
            ui->actionGamepad->setText("Gamepad not found");
            ui->actionGamepad->setDisabled(true);
            return;
        }
    else{
        ui->actionGamepad->setText(QGamepadManager::instance()->gamepadName(*QGamepadManager::instance()->connectedGamepads().begin()));
        ui->actionGamepad->setDisabled(false);
    }




}

MainWindow::~MainWindow()
{
    delete frame_timer;
    delete flash_timer;
    delete ui;
}

void MainWindow::upPressed()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_press(3, 12); break;
    case KEMPSTON_IF:  bus->kj_button_press(Port1F::KJ_UP); break;
    case SINCLAIR_IF2_P1: bus->key_press(1, 12); break;
    case SINCLAIR_IF2_P2: bus->key_press(3, 11); break;
    }
}

void MainWindow::downPressed()
{  
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:   bus->key_press(4, 12); break;
    case KEMPSTON_IF:   bus->kj_button_press(Port1F::KJ_DOWN); break;
    case SINCLAIR_IF2_P1: bus->key_press(2, 12); break;
    case SINCLAIR_IF2_P2: bus->key_press(2, 11); break;
    }
}

void MainWindow::leftPressed()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_press(4, 11); break;
    case KEMPSTON_IF:  bus->kj_button_press(Port1F::KJ_LEFT); break;
    case SINCLAIR_IF2_P1: bus->key_press(4, 12); break;
    case SINCLAIR_IF2_P2: bus->key_press(0, 11); break;
    }
}

void MainWindow::rightPressed()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_press(2, 12); break;
    case KEMPSTON_IF:  bus->kj_button_press(Port1F::KJ_RIGHT); break;
    case SINCLAIR_IF2_P1: bus->key_press(3, 12); break;
    case SINCLAIR_IF2_P2: bus->key_press(1, 11); break;
    }
}

void MainWindow::firePressed()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_press(0, 12); break;
    case KEMPSTON_IF:  bus->kj_button_press(Port1F::KJ_FIRE); break;
    case SINCLAIR_IF2_P1: bus->key_press(0, 12); break;
    case SINCLAIR_IF2_P2: bus->key_press(4, 11); break;
    }
}

void MainWindow::upReleased()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_release(3, 12); break;
    case KEMPSTON_IF:  bus->kj_button_release(Port1F::KJ_UP); break;
    case SINCLAIR_IF2_P1: bus->key_release(1, 12); break;
    case SINCLAIR_IF2_P2: bus->key_release(3, 11); break;
    }
}

void MainWindow::keyPressed(int sc)
{
    auto elem = s_key_mapping.find(sc);
    if (elem != s_key_mapping.end()){
        int row = FIRST(elem.value());
        int col = SECOND(elem.value());
       bus->key_press(row,col);
    }
}

void MainWindow::downReleased()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_release(4, 12); break;
    case KEMPSTON_IF:  bus->kj_button_release(Port1F::KJ_DOWN); break;
    case SINCLAIR_IF2_P1: bus->key_release(2, 12); break;
    case SINCLAIR_IF2_P2: bus->key_release(2, 11); break;
    }
}

void MainWindow::leftReleased()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_release(4, 11); break;
    case KEMPSTON_IF:  bus->kj_button_release(Port1F::KJ_LEFT); break;
    case SINCLAIR_IF2_P1: bus->key_release(4, 12); break;
    case SINCLAIR_IF2_P2: bus->key_release(0, 11); break;
    }
}

void MainWindow::rightReleased()
{
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_release(2, 12); break;
    case KEMPSTON_IF:  bus->kj_button_release(Port1F::KJ_RIGHT); break;
    case SINCLAIR_IF2_P1: bus->key_release(3, 12); break;
    case SINCLAIR_IF2_P2: bus->key_release(1, 11); break;
    }
}

void MainWindow::fireReleased(){
    switch (ui->jsCombo->currentIndex()){
    case CURSOR_IF:    bus->key_release(0, 12); break;
    case KEMPSTON_IF:  bus->kj_button_release(Port1F::KJ_FIRE); break;
    case SINCLAIR_IF2_P1: bus->key_release(0, 12); break;
    case SINCLAIR_IF2_P2: bus->key_release(4, 11); break;
    }
}

void MainWindow::keyReleased(int sc)
{
    auto elem = s_key_mapping.find(sc);
    if (elem != s_key_mapping.end()){
        int row = FIRST(elem.value());
        int col = SECOND(elem.value());
       bus->key_release(row,col);
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
//        qDebug() << "Key pressed: " << ke->nativeScanCode();
        auto sc = ke->nativeScanCode();

        switch (sc){

        case ESC_SCANCODE: reset(); break;
        case F12_SCANCODE: z80_nmi(&cpustate); break;
        case F11_SCANCODE: fullScreenSwitch(); break;
        case UP_SCANCODE: upPressed(); break;
        case DOWN_SCANCODE: downPressed(); break;
        case LEFT_SCANCODE: leftPressed(); break;
        case RIGHT_SCANCODE: rightPressed(); break;
        case LCTRL_SCANCODE: firePressed(); break;
        default: keyPressed(sc);
        }




    }
    if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *ke = static_cast<QKeyEvent*>(event);
//            qDebug() << "Key released: " << ke->nativeScanCode();
            auto sc = ke->nativeScanCode();

            switch (sc){
            case UP_SCANCODE: upReleased(); break;
            case DOWN_SCANCODE: downReleased(); break;
            case LEFT_SCANCODE: leftReleased(); break;
            case RIGHT_SCANCODE: rightReleased(); break;
            case LCTRL_SCANCODE: fireReleased(); break;
            default: keyReleased(sc);
            }
        }
    return false;
}
#pragma pack(push, 1)
struct SNAHeader
{
    uint8_t I;
    uint16_t HL_, DE_,
             BC_, AF_;
    uint16_t HL, DE, BC, IY, IX;
    uint8_t IFF2;
    uint8_t R;
    uint16_t AF, SP;
    uint8_t IM;
    uint8_t BDR;
};
struct Z80Header
{
    uint16_t AF;
    uint16_t BC;
    uint16_t HL;
    uint16_t PC;
    uint16_t SP;
    uint8_t I;
    uint8_t R;
    uint8_t B12;
    uint16_t DE;
    uint16_t BC_;
    uint16_t DE_;
    uint16_t HL_;
    uint16_t AF_;
    uint16_t IY;
    uint16_t IX;
    uint8_t IFF1;
    uint8_t IFF2;
    uint8_t IM;
};
#pragma pack(pop)

void MainWindow::load_sna(const QString &filename)
{
    QFile sna(filename);
    if (sna.open(QIODevice::ReadOnly)){
        QByteArray buffer;
        buffer = sna.readAll();
        SNAHeader * sna_hdr = reinterpret_cast<SNAHeader *>(buffer.data());
        uint8_t * sna_memory = reinterpret_cast<uint8_t *>(buffer.data() + sizeof(SNAHeader));


        cpustate.state.i = sna_hdr->I;
        cpustate.state.hl_.value_uint16 = sna_hdr->HL_;
        cpustate.state.de_.value_uint16 = sna_hdr->DE_;
        cpustate.state.bc_.value_uint16 = sna_hdr->BC_;
        cpustate.state.af_.value_uint16 = sna_hdr->AF_;
        cpustate.state.hl.value_uint16 = sna_hdr->HL;
        cpustate.state.de.value_uint16 = sna_hdr->DE;
        cpustate.state.bc.value_uint16 = sna_hdr->BC;
        cpustate.state.iy.value_uint16 = sna_hdr->IY;
        cpustate.state.ix.value_uint16 = sna_hdr->IX;
        cpustate.state.internal.iff2 = sna_hdr->IFF2 >> 2;
        cpustate.state.r = sna_hdr->R;
        cpustate.state.af.value_uint16 = sna_hdr->AF;
        cpustate.state.sp = sna_hdr->SP;
        cpustate.state.internal.im = sna_hdr->IM;
        bus->io_write8(0xfe, sna_hdr->BDR);
        for (int off = 0; off < 49152; ++off){
            bus->mem_write8(16384+off,sna_memory[off]);
        }


    }
}

void MainWindow::load_z80(const QString &filename)
{
    QFile z80(filename);
    if (z80.open(QIODevice::ReadOnly)){
        QByteArray buffer;
        buffer = z80.readAll();
        Z80Header * z80_hdr = reinterpret_cast<Z80Header *>(buffer.data());
        uint8_t * z80_memory = reinterpret_cast<uint8_t *>(buffer.data() + sizeof(Z80Header));

        if (z80_hdr->B12 == 0xFF) z80_hdr->B12 = 0b00000001;
        cpustate.state.i = z80_hdr->I;
        cpustate.state.hl_.value_uint16 = z80_hdr->HL_;
        cpustate.state.de_.value_uint16 = z80_hdr->DE_;
        cpustate.state.bc_.value_uint16 = z80_hdr->BC_;
        cpustate.state.af_.value_uint16 = z80_hdr->AF_;
        cpustate.state.hl.value_uint16 = z80_hdr->HL;
        cpustate.state.de.value_uint16 = z80_hdr->DE;
        cpustate.state.bc.value_uint16 = z80_hdr->BC;
        cpustate.state.iy.value_uint16 = z80_hdr->IY;
        cpustate.state.ix.value_uint16 = z80_hdr->IX;
        cpustate.state.internal.iff1 = z80_hdr->IFF1;
        cpustate.state.internal.iff2 = z80_hdr->IFF2;
        cpustate.state.r = (z80_hdr->R & 0b01111111) | ((z80_hdr->B12 & 0b00000001)<<7);
        cpustate.state.af.value_uint16 = z80_hdr->AF;
        cpustate.state.sp = z80_hdr->SP;
        cpustate.state.pc = z80_hdr->PC;


        cpustate.state.internal.im = z80_hdr->IM & 0b00000011;
        bus->io_write8(0xfe, (z80_hdr->B12 >> 1) & 0b00000111);



        if( z80_hdr->B12 & 0b00100000 ){
            QByteArray data;
            int status = 0;
            int size = buffer.size() - sizeof(Z80Header);

            unsigned r = 0;
            uint8_t *ptr = z80_memory;
            while (size!=0){
                size--;
                uint8_t byte = *(ptr++);
                if (status == 0 && byte == 0xED) {status = 1;continue;}
                if (status == 0) {data.append(byte); continue;}
                if (status == 1 && byte == 0xED) {status = 2; continue;}
                if (status == 1) {data.append(0xED); data.append(byte); status = 0; continue;}
                if (status == 2 && byte == 0) break;
                if (status == 2) { r = byte; status = 3; continue;}

                while (r!=0){r--; data.append(byte);}
                status = 0;

            }

            for (int off = 0; off < 49152; ++off){
                if (off >= data.size()) break;
                bus->mem_write8(16384+off,data[off]);
            }
        } else {

            for (int off = 0; off < 49152; ++off){
                bus->mem_write8(16384+off,z80_memory[off]);
            }
        }

    }
}
void MainWindow::load_scr(const QString &filename)
{
    QFile scrFile(filename);
    if (scrFile.open(QIODevice::ReadOnly)){
        QByteArray buffer;
        buffer = scrFile.readAll();
        uint8_t * scr = reinterpret_cast<uint8_t *>(buffer.data());
        for (int off = 0; off < 6912; ++off){
            bus->mem_write8(16384+off,scr[off]);
        }
    }
}

void MainWindow::frameRefresh()
{
    z80_run(&cpustate,3500000/50-28);
    z80_int(&cpustate,1);
    z80_run(&cpustate,28);
    z80_int(&cpustate,0);
//    if (isFullScreen()){pal.setColor(QPalette::Window, ui->screen->_borderColor);
//        setPalette(pal);}
    ui->screen->repaint();
}

void MainWindow::on_showControlsCB_stateChanged(int state)
{
    if (state == Qt::Checked)
        ui->controlsTab->setVisible(true);
    else
        ui->controlsTab->setVisible(false);






}

void MainWindow::reset()
{
    z80_reset(&cpustate);
    bus->reset();

}

void MainWindow::on_key_pressed(int row, int col)
{
    qDebug() << "Key pressed: " << row << " " << col;
    bus->key_press(row, col);
}

void MainWindow::on_key_released(int row, int col)
{
    qDebug() << "Key released: "<< row << " " << col;
    bus->key_release(row,col);
}

void MainWindow::on_action_Reset_triggered()
{
    reset();
}

void MainWindow::on_actionE_xit_triggered()
{
    exit(0);
}

void MainWindow::on_cbkbcapture_stateChanged(int state)
{
    if (state == Qt::Checked){
        installEventFilter(this);
        setFocus();
    }
    else {
        removeEventFilter(this);
    }



}

void MainWindow::on_action_Load_a_file_triggered()
{

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load File (snapshot / screenshot"), "/home/$USER/", tr("File (*.sna *.z80 *.scr)"));
    if ((QFileInfo(fileName).suffix())=="sna")
        load_sna(fileName);
    else if ((QFileInfo(fileName).suffix())=="z80")
        load_z80(fileName);
    else if ((QFileInfo(fileName).suffix())=="scr")
        load_scr(fileName);
    else qErrnoWarning("Error!");
}

void MainWindow::save_z80(const QString &fileName)
{
    Z80Header header;
    header.I = cpustate.state.i;
    header.HL_ = cpustate.state.hl_.value_uint16;
    header.DE_ = cpustate.state.de_.value_uint16;
    header.BC_ = cpustate.state.bc_.value_uint16;
    header.AF_ = cpustate.state.af_.value_uint16;
    header.HL = cpustate.state.hl.value_uint16;
    header.DE = cpustate.state.de.value_uint16;
    header.BC = cpustate.state.bc.value_uint16;
    header.IY = cpustate.state.iy.value_uint16;
    header.IX = cpustate.state.ix.value_uint16;
    header.IFF1 = cpustate.state.internal.iff1;
    header.IFF2 = cpustate.state.internal.iff2;
    header.PC = cpustate.state.pc;
    header.R = cpustate.state.r;
    header.AF = cpustate.state.af.value_uint16;
    header.SP = cpustate.state.sp;
    header.IM = cpustate.state.internal.im;
    header.B12 = bus->io_read8(0xfe) | cpustate.state.r;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
           QDataStream out(&file);
           file.write(reinterpret_cast<const char *>(&header), sizeof (header));
           for (int off = 16384; off <= 65535; ++off){
               out<<bus->mem_read8(off);
           }
           file.close();
    }
}

void MainWindow::save_scr(const QString &fileName)
{
    QFile scrFile(fileName);
    if (scrFile.open(QIODevice::WriteOnly)){
        QDataStream out(&scrFile);
        for (int off = 0; off < 6912; ++off){
            out<<bus->mem_read8(16384+off);
        }
       scrFile.close();
    }
}

void MainWindow::fullScreenSwitch()
{
    if (isFullScreen()){
        if(ui->showControlsCB->isChecked())ui->controlsTab->show();
        ui->showControlsCB->show();
        ui->cbkbcapture->show();
        ui->resetButton->show();
        ui->jsCombo->show();
        ui->menubar->show();
        pal.setColor(QPalette::Window, dColor);
        setPalette(pal);
        showNormal();
    }
    else {
        ui->controlsTab->hide();
        ui->menubar->hide();
        ui->showControlsCB->hide();
        ui->cbkbcapture->hide();
        ui->resetButton->hide();
        ui->jsCombo->hide();
        ui->menubar->hide();
        showFullScreen();
    }
}

void MainWindow::on_action_Save_a_snapshot_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Snapshot"), "/home/$USER/", tr("Snapshot File (*.z80)"));
    save_z80(fileName);

}

void MainWindow::on_action_NMI_triggered()
{
    z80_nmi(&cpustate);
}


void MainWindow::on_actionSpectrum_48k_triggered()
{
    ui->actionSpectrum_48k->setChecked(true);
    ui->actionSpectrum_128k->setChecked(false);
    setWindowTitle("My Little Speccy (48K)");
    auto new_bi = new BusInterface48();
    auto old_bi = bus;
    ui->screen->setBusInterface(new_bi);
    cpustate.context = new_bi;
    bus = new_bi;
    delete old_bi;
    reset();
}

void MainWindow::on_actionSpectrum_128k_triggered()
{
    ui->actionSpectrum_128k->setChecked(true);
    ui->actionSpectrum_48k->setChecked(false);
    setWindowTitle("My Little Speccy (128K)");
    auto new_bi = new BusInterface128();
    auto old_bi = bus;
    ui->screen->setBusInterface(new_bi);
    cpustate.context = new_bi;
    bus = new_bi;
    delete old_bi;
    reset();
}



void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, "About", "My Little Speccy emulator Ver. 0.001\n\nDeveloper:\nDmitriy Savin, USPU");
}

void MainWindow::on_actionMake_a_scrennshot_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Screenshot"), "/home/$USER/", tr("Snapshot File (*.scr)"));
    save_scr(fileName);
}

void MainWindow::on_actionPalette_1_triggered()
{
    ui->actionPalette_1->setChecked(true);
    ui->actionPalette_2->setChecked(false);
    ui->actionPalette_3->setChecked(false);
    ui->screen->setPalette(0);
}

void MainWindow::on_actionPalette_2_triggered()
{
    ui->actionPalette_2->setChecked(true);
    ui->actionPalette_1->setChecked(false);
    ui->actionPalette_3->setChecked(false);
    ui->screen->setPalette(1);
}

void MainWindow::on_actionPalette_3_triggered()
{
    ui->actionPalette_1->setChecked(false);
    ui->actionPalette_2->setChecked(false);
    ui->actionPalette_3->setChecked(true);
    ui->screen->setPalette(2);
}

void MainWindow::on_actionGamepad_toggled(bool arg1)
{
    if (arg1){
    gamepad = new QGamepad(*gamepads.begin(), this);
    connect(gamepad, &QGamepad::buttonAChanged, this, [this](bool pressed){
            if (pressed) firePressed();  else fireReleased();
        });
    connect(gamepad, &QGamepad::buttonLeftChanged, this, [this](bool pressed){
            if (pressed) leftPressed();  else leftReleased();
        });
    connect(gamepad, &QGamepad::buttonRightChanged, this, [this](bool pressed){
            if (pressed) rightPressed();  else rightReleased();
        });
    connect(gamepad, &QGamepad::buttonUpChanged, this, [this](bool pressed){
            if (pressed) upPressed();  else upReleased();
        });
    connect(gamepad, &QGamepad::buttonDownChanged, this, [this](bool pressed){
            if (pressed) downPressed();  else downReleased();
        });
    connect(gamepad, &QGamepad::axisLeftXChanged, this, [this](double value){
            if(value<0){leftPressed();rightReleased();}
            if(value>0.001){rightPressed();leftReleased();}
            if(value>0 and value<0.001){leftReleased();rightReleased();}
        });
    connect(gamepad, &QGamepad::axisLeftYChanged, this, [this](double value){
            if(value<0){upPressed();downReleased();}
            if(value>0.001){downPressed();upReleased();}
            if(value>0 and value<0.001){downReleased();upReleased();}
        });
    connect(gamepad, &QGamepad::buttonSelectChanged, this, [this](bool pressed){
            if (pressed) reset();
        });}
    else{
        gamepad->disconnect();
    }
}
