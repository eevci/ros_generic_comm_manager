//
// Created by enver on 22.05.2021.
//

#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H
#include "gcm/NetworkDriver.h"
#include <boost/asio/serial_port.hpp>
#include <boost/shared_array.hpp>
namespace gcm {

    typedef boost::asio::serial_port BOOST_SERIAL_PORT;
    typedef boost::asio::serial_port_base BOOST_SERIAL_PORT_BASE;


    class SerialNetworkDriver : public NetworkDriver {
        public:
            void setBaudrate(uint32_t baudrate){
                this->baudrate = baudrate;
            }
            void setDevice(std::string device){
                this->device = device;
            }
        protected:
            bool isOpen; ///< True if port open
            bool error; ///< Error flag

            uint32_t baudrate;
            std::string device;
            std::shared_ptr<BOOST_SERIAL_PORT> serialPort;
    };
}


#endif //SERIALDRIVER_H
