//
// Created by enver on 22.05.2021.
//

#include "gcm/drivers/serial/RS232Driver.h"

namespace gcm{

    RS232Driver::RS232Driver(){
        this->serialPort = std::make_shared<BOOST_SERIAL_PORT>(this->ioContext);
    }

    RS232Driver::~RS232Driver(){
        if (!this->serialPort->is_open())
            return;
        this->serialPort->cancel();
        if(!this->ioContext.stopped())
            this->ioContext.stop();
        if(this->workerThreads.size())
            this->workerThreads.join_all(); 
        this->serialPort->close();
    }

    bool RS232Driver::open(){
        boost::system::error_code ec;  
        this->serialPort->open(this->device, ec);
        if (!ec)
        {
            BOOST_SERIAL_PORT_BASE::baud_rate _baudrate(this->baudrate);
            BOOST_SERIAL_PORT_BASE::character_size _charSize(8);
            BOOST_SERIAL_PORT_BASE::flow_control _flowControl(BOOST_SERIAL_PORT_BASE::flow_control::none);
            BOOST_SERIAL_PORT_BASE::parity _parity(BOOST_SERIAL_PORT_BASE::parity::none);
            BOOST_SERIAL_PORT_BASE::stop_bits _stopBits(BOOST_SERIAL_PORT_BASE::stop_bits::one);
            this->serialPort->set_option(_baudrate);
            this->serialPort->set_option(_charSize);
            this->serialPort->set_option(_flowControl);
            this->serialPort->set_option(_parity);
            this->serialPort->set_option(_stopBits);
            return true;
        } else {
            std::cerr<<"serial port could not be opened:\n"<<ec.message()<<"\n";
            return false;
        }
    }
    void RS232Driver::handleReceive(const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter){
        NetworkDriver::handleReceive(error, bytesTransferred, recv_buffer_iter);
        if(!error)
            this->doReceive();
    }

    void RS232Driver::doReceive(){
        this->serialPort->async_read_some(
                    boost::asio::buffer(recv_buffer),boost::bind(&RS232Driver::handleReceive,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred,
                                            &(*recv_buffer.begin())));
    }
    
    void RS232Driver::listen() {
        this->doReceive();
        for(int x = 0; x < this->listenerThreadCount; ++x)
            this->workerThreads.create_thread(boost::bind(&boost::asio::io_context::run, &ioContext));
        std::cout << "Listening with "<< (int)this->listenerThreadCount <<" threads\n";
    }

    bool RS232Driver::send(gcm_msgs::NetworkMessage nm){
        boost::system::error_code err;
        bool sent = this->serialPort->write_some(boost::asio::buffer(nm.data,nm.data.size()));
        std::this_thread::sleep_for(std::chrono::microseconds (1));
        return sent>0;
    }
}

