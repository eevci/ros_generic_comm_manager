//
// Created by enver on 23.05.2021.
//

#ifndef SRC_TESTUDPDRIVER_H
#define SRC_TESTUDPDRIVER_H
#include "gtest/gtest.h"
#include <ctime>
#include <unistd.h>
#include "gcm/drivers/ethernet/TCPDriver.h"

class TestTCPDriver : public ::testing::Test{
public:
    TestTCPDriver(){};
    ~TestTCPDriver(){};
protected:
    std::shared_ptr<gcm::TCPDriver> listenerTCPDriver, senderTCPDriver;

    void SetUp(){
        listenerTCPDriver = std::make_shared<gcm::TCPDriver>();
        listenerTCPDriver->setReceiveAddress("127.0.0.1", 13320);
        senderTCPDriver = std::make_shared<gcm::TCPDriver>();
        senderTCPDriver->setTargetAddress("127.0.0.1", 13320);
    }
    void TearDown(){
        std::this_thread::sleep_for(std::chrono::microseconds (100));
    }

    void sendMessage(const std::string& messageToSend, int count){
        for(int i = 0; i<count; i++){
            gcm_msgs::NetworkMessage nm;
            nm.data.assign(messageToSend.begin(), messageToSend.end());
            senderTCPDriver->send(nm);
        }
    }

    void runTestsWithParams(const std::string& messageToSend, const int& numberOfSent, const int& numberOfThread);

    std::string genRandomString(const int len) {
        std::string tmp_s;
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        std::srand( (unsigned) std::time(NULL) * getpid());
        tmp_s.reserve(len);
        for (int i = 0; i < len; ++i) 
            tmp_s += alphanum[std::rand() % (sizeof(alphanum) - 1)];
        return tmp_s;
    }
};


#endif //SRC_TESTUDPDRIVER_H
