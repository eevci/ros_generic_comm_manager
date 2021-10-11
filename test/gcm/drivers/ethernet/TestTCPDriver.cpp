//
// Created by enver on 23.05.2021.
//
#include "TestTCPDriver.h"

TEST_F(TestTCPDriver, singleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!\n\rtest";
    runTestsWithParams(messageToSend, 1, 1);
}

TEST_F(TestTCPDriver, singleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!\n\rtest";
    runTestsWithParams(messageToSend, 1, 12);
}

TEST_F(TestTCPDriver, singleLongMessageToOneThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 1, 1);
}

TEST_F(TestTCPDriver, singleLongMessageToTwelveThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 1, 12);
}

TEST_F(TestTCPDriver, MultipleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!";
    runTestsWithParams(messageToSend, 100, 1);
}

 TEST_F(TestTCPDriver, MultipleShortMessageToFourThreadListener) {
    std::string messageToSend = "HelloWorld!";
    runTestsWithParams(messageToSend, 100, 4);
}

TEST_F(TestTCPDriver, MultipleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!";
    runTestsWithParams(messageToSend, 100, 12);
}

TEST_F(TestTCPDriver, MultipleLongMessageToOneThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 100, 1);
}

TEST_F(TestTCPDriver, MultipleLongMessageToFourThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 100, 4);
}

TEST_F(TestTCPDriver, MultipleLongMessageToTwelveThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 100, 12);
}

void TestTCPDriver::runTestsWithParams(const std::string& messageToSend, const int& numberOfSent, const int& numberOfThread){
    std::stringstream truthStream;
    std::stringstream responseStream;
    for(int i = 0; i<numberOfSent; i++){
        truthStream<<messageToSend;
    }
    listenerTCPDriver->setListenerThreadCount(numberOfThread);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [&] (gcm_msgs::NetworkMessage nm) {
                responseStream<<nm.data;
                ;}
    );
    this->sendMessage(messageToSend, numberOfSent);
    std::this_thread::sleep_for(std::chrono::milliseconds (100));
    EXPECT_EQ(truthStream.str(),  responseStream.str());
}