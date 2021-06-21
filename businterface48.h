#ifndef BUSINTERFACE48_H
#define BUSINTERFACE48_H

#include "businterface.h"

class BusInterface48 : public BusInterface
{
    Q_OBJECT
public:
    BusInterface48();

    virtual uint8_t mem_read8(uint32_t addr) override;
    virtual void mem_write8(uint32_t addr, uint8_t value) override;

    virtual uint8_t io_read8(uint32_t addr) override;
    virtual void io_write8(uint32_t addr, uint8_t value) override;

    virtual const uint8_t * frameBuffer() const override { return ram.getBuffer(16384);}
protected:
    ROMDevice rom { ":/roms/48.rom" };
    RAMDevice ram { 16 };
};

#endif // BUSINTERFACE48_H
