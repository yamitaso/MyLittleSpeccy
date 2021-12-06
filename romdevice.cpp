#include "romdevice.h"
#include <QFile>
#include <QErrorMessage>
#include <QApplication>
ROMDevice::ROMDevice(const QString &filename)
{
    QFile romfile(filename);

    if (romfile.open(QIODevice::ReadOnly)){
        _data = romfile.readAll();
         Q_ASSERT(_data.size() > 0);
    } else {
       QErrorMessage em;
       em.setModal(true);
       em.showMessage("Can't load a ROM file: "+ filename);
       em.exec();
       exit(1); //FIXME: тут нечисто
    }
}

uint8_t ROMDevice::read8(uint32_t addr)
{

   return _data[addr % _data.size()];
}

void ROMDevice::write8(uint32_t addr, uint8_t value)
{
    Q_UNUSED(addr);
    Q_UNUSED(value);
}
