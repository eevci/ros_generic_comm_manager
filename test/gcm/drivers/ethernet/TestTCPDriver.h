//
// Created by enver on 23.05.2021.
//

#ifndef SRC_TESTUDPDRIVER_H
#define SRC_TESTUDPDRIVER_H
#include "gtest/gtest.h"
#include "gcm/drivers/ethernet/TCPDriver.h"

class TestTCPDriver : public ::testing::Test{
public:
    TestTCPDriver(){};
    ~TestTCPDriver(){};
protected:
    std::shared_ptr<gcm::TCPDriver> listenerTCPDriver, senderTCPDriver;

    void SetUp(){
        listenerTCPDriver = std::make_shared<gcm::TCPDriver>();
        senderTCPDriver = std::make_shared<gcm::TCPDriver>();
        listenerTCPDriver->setReceiveAddress("127.0.0.1", 13333);
        senderTCPDriver->setTargetAddress("127.0.0.1", 13333);
    }
    void TearDown(){
        listenerTCPDriver.reset();
        senderTCPDriver.reset();
    }

    void sendMessage(const std::string& messageToSend, int count){
        for(int i = 0; i<count; i++){
            gcm::NetworkMessage nm;
            nm.data.assign(messageToSend.begin(), messageToSend.end());
            nm.size = messageToSend.size();
            senderTCPDriver->send(nm);
        }
    }
};


#endif //SRC_TESTUDPDRIVER_H
