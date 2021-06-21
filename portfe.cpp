#include "portfe.h"

PortFE::PortFE()
{

}

uint8_t PortFE::read8(uint32_t addr)
{
    uint8_t col = (addr >> 8) & 0xff;

    int i = 0;
    uint8_t col_mask = 1;


    uint8_t result = 0b11111111;
    for (; i < 8; ++i){
        if ( !(col & col_mask) ){
            result &= _key_matrix[i];
        }
        col_mask <<= 1;
    }

    return result;
}

void PortFE::write8(uint32_t addr, uint8_t value)
{
    Q_UNUSED(addr);
    _fe_data = value;
}

void PortFE::press_key(int row, int col)
{
    _key_matrix[col-8] &= ~(1 << row);
}

void PortFE::release_key(int row, int col)
{
    _key_matrix[col-8] |= (1 << row);
}
