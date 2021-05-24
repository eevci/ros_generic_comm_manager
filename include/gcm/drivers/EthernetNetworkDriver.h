//
// Created by enver on 22.05.2021.
//

#ifndef ETHERNETNETWORKDRIVER_H
#define ETHERNETNETWORKDRIVER_H

#include "gcm/NetworkDriver.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <iostream>
class EthernetNetworkDriver : public gcm::NetworkDriver {
public:
    void setTargetAddress(std::string ipAddress, int port){
        this->targetAddress = boost::asio::ip::address::from_string(ipAddress);
        this->targetPort = port;
    }
    void setReceiveAddress(std::string ipAddress, int port){
        this->receiveAddress = boost::asio::ip::address::from_string(ipAddress);
        this->receivePort = port;
    }
    void setListenerThreadCount(uint8_t listenerThreadCount) {
        this->listenerThreadCount = listenerThreadCount;
    }
protected:
    boost::asio::io_context ioContext;
    boost::asio::ip::address targetAddress;
    boost::asio::ip::address receiveAddress;
    unsigned short receivePort;
    unsigned short targetPort;
    uint8_t listenerThreadCount = std::thread::hardware_concurrency(); //Number of cores
};

#endif //ETHERNETNETWORKDRIVER_H
