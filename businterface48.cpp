#include "businterface48.h"

BusInterface48::BusInterface48()
{

}

uint8_t BusInterface48::mem_read8(uint32_t addr)
{
    if (addr < 0x4000) return rom.read8(addr);
    return ram.read8(addr);
}

void BusInterface48::mem_write8(uint32_t addr, uint8_t value)
{
     if (addr >= 0x4000) ram.write8(addr,value);
}

uint8_t BusInterface48::io_read8(uint32_t addr)
{
    if ((addr & 1) == 0)
        return portfe.read8(addr);
    if ((addr & 0b100000) == 0)
        return port1f.read8(addr);
    return 0xff;
}

void BusInterface48::io_write8(uint32_t addr, uint8_t value)
{
    if ((addr & 1) == 0)
        portfe.write8(addr, value);
}
