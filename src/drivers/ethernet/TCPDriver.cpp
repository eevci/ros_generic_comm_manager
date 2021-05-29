//
// Created by enver on 22.05.2021.
//
#include "gcm/drivers/ethernet/TCPDriver.h"
namespace gcm{
    TCPDriver::TCPDriver(){
        this->socket = std::make_unique<BOOST_TCP_SOCKET>(this->ioContext);
        this->socket->open(boost::asio::ip::tcp::v4());
    }
    TCPDriver::~TCPDriver(){
        this->socket->close();
        this->ioContext.stop();
        this->workerThreads.join_all();
    }

    void TCPDriver::listen(){
        this->socket->bind(BOOST_TCP_ENDPOINT(this->receiveAddress, this->receivePort));
        this->doReceive();
        for(int x = 0; x < this->listenerThreadCount; ++x)
            this->workerThreads.create_thread(boost::bind(&boost::asio::io_context::run, &ioContext));
        std::cout << "Listening with "<< (int)this->listenerThreadCount <<" threads\n";
    }

    void TCPDriver::handleReceive(const boost::system::error_code& error, size_t bytesTransferred){

    }
    void TCPDriver::doReceive(){
        this->socket->async_receive(boost::asio::buffer(recv_buffer),
                                        boost::bind(&TCPDriver::handleReceive,
                                            this,boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
    }

    bool TCPDriver::send(NetworkMessage networkMessage){
        boost::system::error_code err;
        BOOST_TCP_ENDPOINT endpoint {this->targetAddress, this->targetPort};
        bool sent = socket->send(boost::asio::buffer(networkMessage.data,networkMessage.data.size()), 0, err);
        std::this_thread::sleep_for(std::chrono::microseconds (100));
        return sent>0;
    }
}
