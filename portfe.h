#ifndef PORTFE_H
#define PORTFE_H

#include "busdevice.h"

class PortFE : public BusDevice
{
    Q_OBJECT
public:
    PortFE();

    uint8_t read8(uint32_t addr) override;
    void write8(uint32_t addr, uint8_t value) override;

    int border() const { return _fe_data & 0b00000111; }
    int tape_out() const { return !!(_fe_data & 0b00001000);}
    int beeper_out() const { return !!(_fe_data & 0b00010000);}

    void press_key(int row, int col);
    void release_key(int row, int col);

private:
    uint8_t _key_matrix[8] { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    uint8_t _fe_data { 0 };
};

#endif // PORTFE_H
