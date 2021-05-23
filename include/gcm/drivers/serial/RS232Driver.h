//
// Created by enver on 22.05.2021.
//

#ifndef SERIAL_RS232DRIVER_H
#define SERIAL_RS232DRIVER_H

#include "gcm/drivers/SerialNetworkDriver.h"
namespace gcm{
    class RS232Driver : public SerialNetworkDriver{
    public:
        virtual void listen();
        virtual void addCallback(CallbackFunction callback);
        virtual bool send();
    };
}

#endif //SERIAL_RS232DRIVER_H
