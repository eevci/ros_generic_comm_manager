//
// Created by enver on 22.05.2021.
//
#include "gcm/drivers/ethernet/TCPDriver.h"
namespace gcm{
    TCPDriver::TCPDriver(){
        this->socket = std::make_shared<BOOST_TCP_SOCKET>(this->ioContext);
        this->socket->open(BOOST_TCP::v4());
        this->isConnected = false;
    }
    TCPDriver::~TCPDriver(){
        this->socket->close();
        this->ioContext.stop();
    }

    void TCPDriver::doAccept(){
        //acceptor->async_accept(*this->socket, [this](boost::system::error_code ec){});
        this->acceptor->async_accept(*this->socket,
            [this](boost::system::error_code ec)
            {
             if (!ec)
            {
                //std::make_shared<session>(std::move(socket_))->start();
                this->doReceive();
            }

            this->doAccept(); 
            }
        ); 
    }

    void TCPDriver::handleAccept(BOOST_TCP::socket* client, const boost::system::error_code& error) {

    }
    void TCPDriver::listen(){
        this->acceptor = std::make_shared<BOOST_TCP_ACCEPTOR>(this->ioContext, BOOST_TCP_ENDPOINT(BOOST_TCP::v4(), this->receivePort));
        this->doAccept();
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
