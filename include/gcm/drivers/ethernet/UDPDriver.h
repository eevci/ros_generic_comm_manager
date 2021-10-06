//
// Created by enver on 22.05.2021.
//

#ifndef ETH_UDPDRIVER_H
#define ETH_UDPDRIVER_H
#include "gcm/drivers/EthernetNetworkDriver.h"
#include "gcm/NetworkMessage.h"
namespace gcm {
    typedef boost::asio::ip::udp::socket BOOST_UDP_SOCKET;
    typedef boost::asio::ip::udp::endpoint BOOST_UDP_ENDPOINT;
    class UDPDriver : public EthernetNetworkDriver, public std::enable_shared_from_this<UDPDriver> {
    public:
        UDPDriver();
        ~UDPDriver();
        virtual void listen();
        virtual bool send(NetworkMessage);
        BOOST_UDP_ENDPOINT getSenderEndPoint(){return this->senderEndpoint;}
    private:
        boost::array<char, 1024> recv_buffer;
        std::unique_ptr<BOOST_UDP_SOCKET> socket;
        void doReceive();
        void handleReceive(const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter);
        BOOST_UDP_ENDPOINT senderEndpoint;
    };

}
#endif //ETH_UDPDRIVER_H
