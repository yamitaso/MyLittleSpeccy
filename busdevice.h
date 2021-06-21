#ifndef BUSDEVICE_H
#define BUSDEVICE_H

#include <QObject>

class BusDevice : public QObject
{
    Q_OBJECT
public:
    explicit BusDevice(QObject *parent = nullptr);
    virtual ~BusDevice() = default;

    virtual uint8_t read8(uint32_t addr) = 0; // pure virtual (чисто виртуальный) - абстрактный метод
    virtual void write8(uint32_t addr, uint8_t value) = 0;

signals:

};

#endif // BUSDEVICE_H
