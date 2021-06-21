#ifndef BUSINTERFACE_H
#define BUSINTERFACE_H

#include <QObject>
#include "romdevice.h"
#include "ramdevice.h"
#include "portfe.h"
#include "port1f.h"
class BusInterface : public QObject
{
    Q_OBJECT
public:
    explicit BusInterface(QObject *parent = nullptr);

    virtual uint8_t mem_read8(uint32_t addr) = 0;
    virtual void mem_write8(uint32_t addr, uint8_t value) = 0;

    virtual uint8_t io_read8(uint32_t addr) = 0;
    virtual void io_write8(uint32_t addr, uint8_t value) = 0;

    int border() const { return portfe.border(); }
    virtual const uint8_t * frameBuffer() const = 0;

    void key_press(int row, int col);
    void key_release(int row, int col);

    void kj_button_press(int btn)  { port1f.press_button(btn);   }
    void kj_button_release(int btn){ port1f.release_button(btn); }

    virtual void reset() {};


signals:
protected:
    PortFE portfe;
    Port1F port1f;
};

#endif // BUSINTERFACE_H
