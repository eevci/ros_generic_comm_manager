//
// Created by enver on 22.05.2021.
//
#include "gcm/drivers/ethernet/TCPDriver.h"
namespace gcm{
    TCPDriver::TCPDriver(){
        
        this->isConnected = false;
    }
    TCPDriver::~TCPDriver(){
        this->acceptor->close();
        this->ioContext.stop();
        this->workerThreads.join_all();
    }

    /* void TCPDriver::doAccept(){
        try {
            this->acceptor->accept(*this->socket);
            std::cout<<"Accepted\n";
        }
        catch (boost::system::system_error &e) {
            std::cerr << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
            return;
        }
        
    }

    void TCPDriver::listen(){
        this->socket = std::make_shared<BOOST_TCP_SOCKET>(this->ioContext);
        unsigned short port_num = this->receivePort;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(),port_num);
        this->acceptor = std::make_shared<BOOST_TCP_ACCEPTOR>(this->ioContext, ep.protocol());
        acceptor->bind(ep);
        acceptor->listen(BACKLOG_SIZE);
        std::thread(&TCPDriver::doAccept,this).detach();       
    } */
    
    void TCPDriver::doAccept(){
        try {
            BOOST_TCP_SOCKET _socket = std::make_shared<BOOST_TCP_SOCKET>(this->ioContext);
            acceptor->async_accept( _socket, boost::bind( &http_server::handle_accept, this, ph::_1 ) );

            std::cout<<"Accepted\n";
        }
        catch (boost::system::system_error &e) {
            std::cerr << "Error occured! Error code = " << e.code()
            << ". Message: " << e.what();
            return;
        }
        
    }

    void TCPDriver::listen(){
        unsigned short port_num = this->receivePort;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(),port_num);
        this->acceptor = std::make_shared<BOOST_TCP_ACCEPTOR>(this->ioContext, ep.protocol());
        acceptor->bind(ep);
        acceptor->listen(BACKLOG_SIZE);
        std::thread(&TCPDriver::doAccept,this).detach();       
    }

    void TCPDriver::doReceive(){
        this->socket->async_receive(boost::asio::buffer(recv_buffer),
                                        boost::bind(&TCPDriver::handleReceive,
                                            this,
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
