#ifndef RAMDEVICE_H
#define RAMDEVICE_H

#include "busdevice.h"

class RAMDevice : public BusDevice
{
    Q_OBJECT
public:
    RAMDevice(int width);

    uint8_t read8(uint32_t addr) override;
    void write8(uint32_t addr, uint8_t value) override;

    const uint8_t *getBuffer(uint32_t addr) const;

private:
    QVector<uint8_t> _data;
};

#endif // RAMDEVICE_H
