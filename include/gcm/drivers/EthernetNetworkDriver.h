//
// Created by enver on 22.05.2021.
//

#ifndef ETHERNETNETWORKDRIVER_H
#define ETHERNETNETWORKDRIVER_H

#include "gcm/NetworkDriver.h"
namespace gcm {
    class EthernetNetworkDriver : public gcm::NetworkDriver {
    public:
        virtual void setTargetAddress(std::string ipAddress, int port){
            this->targetAddress = boost::asio::ip::address::from_string(ipAddress);
            this->targetPort = port;
        }
        virtual void setReceiveAddress(std::string ipAddress, int port){
            this->receiveAddress = boost::asio::ip::address::from_string(ipAddress);
            this->receivePort = port;
        }
        std::pair<std::string, int> getReceiveAddress(){return {this->receiveAddress.to_string(), this->receivePort};}
        std::pair<std::string, int> getTargetAddress(){return {this->targetAddress.to_string(), this->targetPort};}
        bool open() {return true;};
    protected:
        boost::asio::io_context ioContext;
        boost::asio::ip::address targetAddress;
        boost::asio::ip::address receiveAddress;
        unsigned short receivePort;
        unsigned short targetPort;
    };
}


#endif //ETHERNETNETWORKDRIVER_H
