//
// Created by enver on 22.05.2021.
//

#ifndef ETH_TCPDRIVER_H
#define ETH_TCPDRIVER_H
#include "gcm/drivers/EthernetNetworkDriver.h"

namespace gcm {
    typedef boost::asio::ip::tcp::socket BOOST_TCP_SOCKET;
    typedef boost::asio::ip::tcp::endpoint BOOST_TCP_ENDPOINT;
    class TCPDriver : public EthernetNetworkDriver {
    public:
        TCPDriver();
        ~TCPDriver();
        virtual void listen();
        virtual bool send(NetworkMessage);
        BOOST_TCP_ENDPOINT getSenderEndPoint(){return this->senderEndpoint;}
    private:
        boost::array<char, 10240> recv_buffer;
        std::unique_ptr<BOOST_TCP_SOCKET> socket;
        BOOST_TCP_ENDPOINT senderEndpoint;
        void doReceive();
        boost::thread_group workerThreads;
    };
}
#endif //ETH_TCPDRIVER_H
