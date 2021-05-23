//
// Created by enver on 22.05.2021.
//

#ifndef NETWORKDRIVER_H
#define NETWORKDRIVER_H

#include "gcm/NetworkMessage.h"
#include <vector>
#include <functional>
#include <algorithm>
namespace gcm{
    typedef std::function<void(NetworkMessage)> CallbackFunction;
    class NetworkDriver{
    public:
        virtual void listen() = 0;
        virtual void addCallback(CallbackFunction f) {this->callbackList.push_back(f);};
        virtual bool send(NetworkMessage) = 0;

    private:
        std::vector<CallbackFunction> callbackList;
    protected:
        virtual void notify(const NetworkMessage message) const {
            std::for_each(this->callbackList.begin(),this->callbackList.end(),
                          [message] (const CallbackFunction& f) { f(message);});
        };
        NetworkDriver(){}
        ~NetworkDriver(){}
    };
}

#endif //NETWORKDRIVER_H
