//
// Created by enver on 23.05.2021.
//

#ifndef SRC_TESTUDPDRIVER_H
#define SRC_TESTUDPDRIVER_H
#include "gtest/gtest.h"
#include "gcm/drivers/ethernet/UDPDriver.h"

class TestUDPDriver : public ::testing::Test{
public:
    TestUDPDriver(){};
    ~TestUDPDriver(){};
protected:
    std::shared_ptr<gcm::UDPDriver> listenerUDPDriver, senderUDPDriver;

    void SetUp(){
        listenerUDPDriver = std::make_shared<gcm::UDPDriver>();
        senderUDPDriver = std::make_shared<gcm::UDPDriver>();
        listenerUDPDriver->setReceiveAddress("127.0.0.1", 13333);
        senderUDPDriver->setTargetAddress("127.0.0.1", 13333);
    }
    void TearDown(){
        listenerUDPDriver.reset();
        senderUDPDriver.reset();
    }

    void sendMessage(const std::string& messageToSend, int count){
        for(int i = 0; i<count; i++){
            NetworkMessage nm;
            nm.data = (void*)(messageToSend+std::string(std::to_string(i))).data();
            nm.size = messageToSend.size();
            senderUDPDriver->send(nm);
        }
    }
};


#endif //SRC_TESTUDPDRIVER_H
