//
// Created by enver on 22.05.2021.
//

#ifndef ETH_TCPDRIVER_H
#define ETH_TCPDRIVER_H
#include "gcm/drivers/EthernetNetworkDriver.h"

namespace gcm {
    typedef boost::asio::ip::tcp BOOST_TCP;
    typedef BOOST_TCP::socket BOOST_TCP_SOCKET;
    typedef BOOST_TCP::endpoint BOOST_TCP_ENDPOINT;
    typedef BOOST_TCP::acceptor BOOST_TCP_ACCEPTOR;
    static const int BACKLOG_SIZE = 30;
    
    class TCPDriver : public std::enable_shared_from_this<TCPDriver>, public EthernetNetworkDriver {
    public:
        TCPDriver();
        ~TCPDriver();
        virtual void listen();
        virtual bool send(NetworkMessage);
        void setTargetAddress(std::string ipAddress, int port){
            this->targetAddress = boost::asio::ip::address::from_string(ipAddress);
            this->targetPort = port;
        }
        BOOST_TCP_ENDPOINT getSenderEndPoint(){return this->senderEndpoint;}
    private:
        boost::array<char, 10240> recv_buffer;
        std::shared_ptr<BOOST_TCP_SOCKET> socket;
        std::vector<std::shared_ptr<BOOST_TCP_SOCKET>> clientSocketList;
        std::shared_ptr<BOOST_TCP_ACCEPTOR> acceptor;
        BOOST_TCP_ENDPOINT senderEndpoint;
        bool isConnected = false;
        void doAccept();
        void doReceive(std::shared_ptr<BOOST_TCP_SOCKET> _socket);
        boost::thread_group workerThreads;
        void handleAccept(std::shared_ptr<BOOST_TCP_SOCKET> socket , boost::system::error_code const & err );
        void handleReceive(std::shared_ptr<BOOST_TCP_SOCKET> socket, const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter);
    };
}
#endif //ETH_TCPDRIVER_H
