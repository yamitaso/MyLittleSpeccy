#include "port7ffd.h"

Port7FFD::Port7FFD()
{

}

uint8_t Port7FFD::read8(uint32_t addr)
{
    Q_UNUSED(addr);
    return 0;
}

void Port7FFD::write8(uint32_t addr, uint8_t value)
{
    Q_UNUSED(addr);
    if (!locked())
        _port_7ffd_data = value;
}
