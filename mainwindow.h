#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "businterface.h"
#include "emulation/CPU/Z80.h"
#include <QTimer>
#include <QGamepad>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressed(int sc);

    void keyReleased(int sc);





public slots:
    void upPressed();

    void downPressed();

    void leftPressed();

    void rightPressed();

    void firePressed();

    void upReleased();

    void downReleased();

    void leftReleased();

    void rightReleased();

    void fireReleased();




protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;
    void load_sna(const QString &filename);
    void load_z80(const QString &filename);
    void load_scr(const QString &filename);
    void save_z80(const QString &fileName);
    void save_scr(const QString &fileName);
    void fullScreenSwitch();
private slots:
    void frameRefresh();

    void on_showControlsCB_stateChanged(int state);

    void reset();

    void on_key_pressed(int row, int col);
    void on_key_released(int row, int col);

    void on_action_Reset_triggered();

    void on_actionE_xit_triggered();

    void on_cbkbcapture_stateChanged(int arg1);

    void on_action_Save_a_snapshot_triggered();

    void on_action_NMI_triggered();


    void on_actionSpectrum_48k_triggered();

    void on_actionSpectrum_128k_triggered();

    void on_action_Load_a_file_triggered();

    void on_action_About_triggered();

    void on_actionMake_a_scrennshot_triggered();

    void on_actionPalette_1_triggered();

    void on_actionPalette_2_triggered();

    void on_actionPalette_3_triggered();

    void on_actionGamepad_toggled(bool arg1);

private:
    Ui::MainWindow *ui;

    BusInterface * bus {nullptr};

    Z80 cpustate {};
    QTimer * frame_timer;
    QTimer * flash_timer;
    QGamepad * gamepad;
    QList<int> gamepads;


};
#endif // MAINWINDOW_H
