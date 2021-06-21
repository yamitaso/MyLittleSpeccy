#include "ramdevice.h"

RAMDevice::RAMDevice(int width)
{
    Q_ASSERT(width >= 0);
    _data.resize(1 << width);

    //TODO: заполнить начальными значениями
    for (int addr = 0;addr < _data.size(); ++addr){
        _data[addr] = 0b00000000;
    }
}

uint8_t RAMDevice::read8(uint32_t addr)
{
    return _data[addr % _data.size()];
}

void RAMDevice::write8(uint32_t addr, uint8_t value)
{
    _data[addr % _data.size()] = value;

}

const uint8_t *RAMDevice::getBuffer(uint32_t addr) const
{
    return &_data[addr % _data.size()];
}
