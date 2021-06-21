#ifndef ROMDEVICE_H
#define ROMDEVICE_H

#include "busdevice.h"

class ROMDevice : public BusDevice
{
    Q_OBJECT
public:
    ROMDevice(const QString &filename);

    uint8_t read8(uint32_t addr) override;
    void write8(uint32_t addr, uint8_t value) override;

private:
    QByteArray _data;
};

#endif // ROMDEVICE_H
