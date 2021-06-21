#ifndef PORT7FFD_H
#define PORT7FFD_H

#include "busdevice.h"

class Port7FFD : public BusDevice
{
    Q_OBJECT
public:
    Port7FFD();

    virtual uint8_t read8(uint32_t addr) override;
    virtual void write8(uint32_t addr, uint8_t value) override;

    int ram_page() const { return _port_7ffd_data & 0b111; }
    int vram_page() const { if ((_port_7ffd_data & 0b00001000) != 0) return 7;
                          return 5;}
    int rom_page() const { return (_port_7ffd_data & 0b00010000) >> 4; }
    bool locked() const { return _port_7ffd_data & 0b00100000; }
    void reset() { _port_7ffd_data = 0; }
private:
    uint8_t _port_7ffd_data { 0 };
};

#endif // PORT7FFD_H
