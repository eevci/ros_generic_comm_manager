//
// Created by enver on 06.10.2021.
//

#ifndef SRC_TESTRS232DRIVER_H
#define SRC_TESTRS232DRIVER_H
#include "gtest/gtest.h"
#include "gcm/drivers/serial/RS232Driver.h"

class TestRS232Driver : public ::testing::Test{
public:
    TestRS232Driver(){};
    ~TestRS232Driver(){};
protected:
    std::shared_ptr<gcm::RS232Driver> listenerRS232Driver, senderRS232Driver;
    void runTestsWithParams(const std::string& messageToSend, const int& numberOfSent, const int& numberOfThread);
    void SetUp(){
        listenerRS232Driver = std::make_shared<gcm::RS232Driver>();
        senderRS232Driver = std::make_shared<gcm::RS232Driver>();
        listenerRS232Driver->setBaudrate(9600);
        listenerRS232Driver->setDevice("/dev/pts/4");
        listenerRS232Driver->setListenerThreadCount(1);
        senderRS232Driver->setBaudrate(9600);
        senderRS232Driver->setDevice("/dev/pts/5");
        bool isListenerOpened = listenerRS232Driver->open();
        bool isSenderOpened = senderRS232Driver->open();
        if(!isListenerOpened || !isSenderOpened)
            GTEST_SKIP();
    }
    void TearDown(){
        listenerRS232Driver.reset();
        senderRS232Driver.reset();
    }

    void sendMessage(const std::string& messageToSend, int count){
        for(int i = 0; i<count; i++){
            gcm_msgs::NetworkMessage nm;
            nm.data.assign(messageToSend.begin(), messageToSend.end());
            senderRS232Driver->send(nm);
        }
    }

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


#endif //SRC_TESTRS232DRIVER_H
