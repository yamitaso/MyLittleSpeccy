#include "businterface128.h"

BusInterface128::BusInterface128()
{

}

uint8_t BusInterface128::mem_read8(uint32_t addr)
{
    if (addr < 0x4000){ //window 0
        return rom.read8(addr + 0x4000 * mapper.rom_page());
    }
    if (addr < 0x8000){ //window 1
        uint32_t off = addr - 0x4000;
        return ram.read8(off + 0x4000 * mapper.vram_page());
    }
    if (addr < 0xC000){ //window 2
        return ram.read8(addr);
    }
    uint32_t off = addr - 0xC000; //window 3
    return ram.read8(off + 0x4000 * mapper.ram_page());
}

void BusInterface128::mem_write8(uint32_t addr, uint8_t value)
{
    if (addr < 0x4000){ //window 0
       return rom.write8(addr + 0x4000 * mapper.rom_page(),value);
    }
    if (addr < 0x8000){ //window 1
        uint32_t off = addr - 0x4000;
        return ram.write8(off + 0x4000 * mapper.vram_page(),value);
    }
    if (addr < 0xC000){ //window 2
        return ram.write8(addr,value);
    }
    uint32_t off = addr - 0xC000; //window 3
        return ram.write8(off + 0x4000 * mapper.ram_page(),value);
}

uint8_t BusInterface128::io_read8(uint32_t addr)
{
    if ((addr & 1) == 0)
        return portfe.read8(addr);
    if ((addr & 0b100000) == 0)
        return port1f.read8(addr);
    return 0xff;
}

void BusInterface128::io_write8(uint32_t addr, uint8_t value)
{
    if ((addr & 0b1000000000000010) == 0)
        mapper.write8(addr,value);
    if ((addr & 1) == 0)
        portfe.write8(addr, value);
}
