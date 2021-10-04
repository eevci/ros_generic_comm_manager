//
// Created by enver on 22.05.2021.
//
#include "gcm/drivers/ethernet/TCPDriver.h"
namespace gcm{
    TCPDriver::TCPDriver(){
        this->isConnected = false;
        this->socket = std::make_shared<BOOST_TCP_SOCKET>(this->ioContext);
    }
    TCPDriver::~TCPDriver(){
        if(this->acceptor->is_open())
            this->acceptor->close();
        if(!this->ioContext.stopped())
            this->ioContext.stop();
        for(auto socketPtr : clientSocketList)
            socketPtr->close();
        this->workerThreads.join_all();
    }

    void TCPDriver::handleAccept(std::shared_ptr<BOOST_TCP_SOCKET> socket , boost::system::error_code const & err ){
        this->doReceive(socket);
        this->doAccept();
    }

    void TCPDriver::doAccept(){
        std::shared_ptr<BOOST_TCP_SOCKET> _socket; 
        clientSocketList.push_back(_socket);
        try {
            _socket = std::make_shared<BOOST_TCP_SOCKET>(this->ioContext);
            acceptor->async_accept( *_socket, boost::bind( &TCPDriver::handleAccept, shared_from_this(), _socket, boost::asio::placeholders::error) );
        }
        catch (boost::system::system_error &e) {
            std::cerr << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
            std::remove(clientSocketList.begin(),clientSocketList.end(),_socket);
            return;
        }
    }

    void TCPDriver::listen(){
        unsigned short port_num = this->receivePort;
        BOOST_TCP_ENDPOINT ep(boost::asio::ip::address_v4::any(),port_num);
        this->acceptor = std::make_shared<BOOST_TCP_ACCEPTOR>(this->ioContext, ep.protocol());
        acceptor->bind(ep);
        acceptor->listen(BACKLOG_SIZE);
        std::cout << "Listening with "<< (int)this->listenerThreadCount <<" threads\n";
        this->doAccept();   
        for(int x = 0; x < this->listenerThreadCount; ++x)
            this->workerThreads.create_thread(boost::bind(&boost::asio::io_context::run, &ioContext));    
    }

    void TCPDriver::handleReceive(std::shared_ptr<BOOST_TCP_SOCKET> socket, const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter){
        EthernetNetworkDriver::handleReceive(error, bytesTransferred, recv_buffer_iter);
        if(!error)
            this->doReceive(socket);
        else{
            socket->close();
            std::remove(clientSocketList.begin(),clientSocketList.end(),socket);
            std::cout<<"Connection Closed!\n";
        }
    }
    void TCPDriver::doReceive(std::shared_ptr<BOOST_TCP_SOCKET> socket){
        socket->async_receive(boost::asio::buffer(recv_buffer),
                                        boost::bind(&TCPDriver::handleReceive,
                                            shared_from_this(),
                                            socket,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred,
                                            &(*recv_buffer.begin())));
    }

    bool TCPDriver::send(NetworkMessage networkMessage){
        BOOST_TCP_ENDPOINT endpoint {this->targetAddress, this->targetPort};
        if(!this->isConnected){
            this->isConnected = true;
            this->socket->connect(endpoint);
        }
        boost::system::error_code err;
        bool sent = socket->send(boost::asio::buffer(networkMessage.data,networkMessage.data.size()), 0, err);
        std::this_thread::sleep_for(std::chrono::microseconds (1));
        return sent>0;
    }
}
