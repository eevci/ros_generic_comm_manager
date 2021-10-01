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

    class Session : public std::enable_shared_from_this<Session>
    {
    public:

        Session(boost::asio::ip::tcp::socket&& socket)
        : socket(std::move(socket))
        {
        }
        ~Session(){
            socket.close();
        }
        void start()
        {
            std::cout<<"started!\n";
            boost::asio::async_read_until(socket, streambuf, '\n', [self = shared_from_this()] (boost::system::error_code error, std::size_t bytes_transferred)
            {
                std::cout << std::istream(&self->streambuf).rdbuf();
            });
        }

    private:
        BOOST_TCP_SOCKET socket;
        boost::asio::streambuf streambuf;
    };



    
    class TCPDriver : public EthernetNetworkDriver {
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
        std::shared_ptr<BOOST_TCP_ACCEPTOR> acceptor;
        BOOST_TCP_ENDPOINT senderEndpoint;
        bool isConnected = false;
        void doAccept();
        void doReceive();
        boost::thread_group workerThreads;
    };
}
#endif //ETH_TCPDRIVER_H
