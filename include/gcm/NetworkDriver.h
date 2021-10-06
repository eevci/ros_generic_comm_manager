//
// Created by enver on 22.05.2021.
//

#ifndef NETWORKDRIVER_H
#define NETWORKDRIVER_H

#include "gcm/NetworkMessage.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
namespace gcm{
    typedef std::function<void(const NetworkMessage&)> CallbackFunction;
    class NetworkDriver{
    public:
        virtual bool open() = 0;
        virtual void listen() = 0;
        virtual void addCallback(CallbackFunction f) {this->callbackList.push_back(f);};
        virtual bool send(NetworkMessage) = 0;
        virtual void setListenerThreadCount(uint8_t listenerThreadCount) {
            this->listenerThreadCount = listenerThreadCount;
        }
        uint8_t listenerThreadCount = 1; 
    private:
        std::vector<CallbackFunction> callbackList;
    protected:
        virtual void notify(const NetworkMessage& message) const {
            std::for_each(this->callbackList.begin(),this->callbackList.end(),
                          [message] (const CallbackFunction& f) { f(message);});
        }
        virtual void handleReceive(const boost::system::error_code& error, size_t bytesTransferred, char* recv_buffer_iter){
            if (error) {
                std::cout << "Receive failed: " << error.message() << "\n";
            }
            else {
                NetworkMessage networkMessage;
                networkMessage.data.assign(recv_buffer_iter, bytesTransferred);
                this->notify(networkMessage);
            }
        }
        NetworkDriver(){}
        ~NetworkDriver(){}

        boost::thread_group workerThreads;
    };
}

#endif //NETWORKDRIVER_H
