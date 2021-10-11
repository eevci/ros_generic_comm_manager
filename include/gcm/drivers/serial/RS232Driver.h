//
// Created by enver on 22.05.2021.
//

#ifndef SERIAL_RS232DRIVER_H
#define SERIAL_RS232DRIVER_H

#include "gcm/drivers/SerialNetworkDriver.h"
namespace gcm{
    class RS232Driver : public SerialNetworkDriver{
    public:
        RS232Driver();
        ~RS232Driver();
        bool open();
        virtual void listen();
        virtual bool send(gcm_msgs::NetworkMessage);
    protected:
        boost::array<char, 1024> recv_buffer;
        void doReceive();
        void handleReceive(const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter);
        boost::asio::io_context ioContext;
    
    };
}

#endif //SERIAL_RS232DRIVER_H
