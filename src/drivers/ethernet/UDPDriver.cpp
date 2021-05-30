//
// Created by enver on 22.05.2021.
//
#include "gcm/drivers/ethernet/UDPDriver.h"

using boost::asio::ip::address;
namespace gcm{
    UDPDriver::UDPDriver(){
        this->socket = std::make_unique<BOOST_UDP_SOCKET>(this->ioContext);
        this->socket->open(boost::asio::ip::udp::v4());
    }

    UDPDriver::~UDPDriver(){
        this->socket->close();
        this->ioContext.stop();
        this->workerThreads.join_all();
    }

    void UDPDriver::listen() {
        this->socket->bind(BOOST_UDP_ENDPOINT(this->receiveAddress, this->receivePort));
        this->doReceive();
        for(int x = 0; x < this->listenerThreadCount; ++x)
            this->workerThreads.create_thread(boost::bind(&boost::asio::io_context::run, &ioContext));
        std::cout << "Listening with "<< (int)this->listenerThreadCount <<" threads\n";
    }

    void UDPDriver::doReceive() {
        this->socket->async_receive_from(boost::asio::buffer(recv_buffer),
                                   this->senderEndpoint,
                                  boost::bind(&UDPDriver::handleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void UDPDriver::handleReceive(const boost::system::error_code& error, size_t bytesTransferred) {
        this->doReceive();
        if (error) {
            std::cout << "Receive failed: " << error.message() << "\n";
            return;
        }
        NetworkMessage networkMessage;
        networkMessage.data.assign(recv_buffer.begin(), bytesTransferred);
        this->notify(networkMessage);
    }

    bool UDPDriver::send(NetworkMessage networkMessage) {
        boost::system::error_code err;
        BOOST_UDP_ENDPOINT endpoint {this->targetAddress, this->targetPort};
        bool sent = socket->send_to(boost::asio::buffer(networkMessage.data,networkMessage.data.size()), endpoint, 0, err);
        std::this_thread::sleep_for(std::chrono::microseconds (100));
        return sent>0;
    }

}