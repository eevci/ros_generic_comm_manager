//
// Created by enver on 22.05.2021.
//

#ifndef ETH_UDPDRIVER_H
#define ETH_UDPDRIVER_H
#include "gcm/drivers/EthernetNetworkDriver.h"
namespace gcm {
    typedef boost::asio::ip::udp::socket BOOST_UDP_SOCKET;
    typedef boost::asio::ip::udp::endpoint BOOST_UDP_ENDPOINT;
    class UDPDriver : public EthernetNetworkDriver {
    public:
        UDPDriver();
        ~UDPDriver();
        virtual void listen();
        virtual bool send(NetworkMessage);
        BOOST_UDP_ENDPOINT getSenderEndPoint(){return this->senderEndpoint;}
    private:
        boost::array<char, 1024> recv_buffer;
        std::unique_ptr<BOOST_UDP_SOCKET> socket;
        void handleReceive(const boost::system::error_code& error, size_t bytesTransferred);
        void doReceive();
        BOOST_UDP_ENDPOINT senderEndpoint;
        boost::thread_group workerThreads;
    };

}
#endif //ETH_UDPDRIVER_H
