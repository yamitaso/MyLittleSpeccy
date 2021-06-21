#ifndef PORT1F_H
#define PORT1F_H

#include "busdevice.h"

class Port1F : public BusDevice
{
    Q_OBJECT
public:
    Port1F();

    uint8_t read8(uint32_t addr) override;
    void write8(uint32_t addr, uint8_t value) override;

    void press_button(int btn);
    void release_button(int btn);

    enum {
      KJ_RIGHT = 0,
      KJ_LEFT  = 1,
      KJ_DOWN  = 2,
      KJ_UP    = 3,
      KJ_FIRE  = 4
    };

private:
    uint8_t _port_1f_data { 0x00 };
};

#endif // PORT1F_H
