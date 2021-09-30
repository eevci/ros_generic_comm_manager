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
#include <string>
namespace gcm {
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
        std::pair<std::string, int> getReceiveAddress(){return {this->receiveAddress.to_string(), this->receivePort};}
        std::pair<std::string, int> getTargetAddress(){return {this->targetAddress.to_string(), this->targetPort};}
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
        void handleReceive(const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter){
            std::cout << "bytesTransferred: " << bytesTransferred << "\n";
            if (error) {
                std::cout << "Receive failed: " << error.message() << "\n";
                return;
            }
            NetworkMessage networkMessage;
            networkMessage.data.assign(recv_buffer_iter, bytesTransferred);
            networkMessage.size = bytesTransferred;
            this->notify(networkMessage);
        }
    };
}


#endif //ETHERNETNETWORKDRIVER_H
