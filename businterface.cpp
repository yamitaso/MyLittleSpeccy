#include "businterface.h"

BusInterface::BusInterface(QObject *parent) : QObject(parent)
{

}

void BusInterface::key_press(int row, int col)
{
    portfe.press_key(row, col);
}

void BusInterface::key_release(int row, int col)
{
    portfe.release_key(row, col);
}

void BusInterface::tape_in(int value)
{
    portfe.tape_in(value);
}
