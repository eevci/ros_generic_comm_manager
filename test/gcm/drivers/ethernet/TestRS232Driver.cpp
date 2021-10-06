//
// Created by enver on 06.10.2021.
//
#include "TestRS232Driver.h"

TEST_F(TestRS232Driver, singleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!\n\rtest";
    runTestsWithParams(messageToSend, 1, 1);
}

TEST_F(TestRS232Driver, singleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!\n\rtest";
    runTestsWithParams(messageToSend, 1, 12);
}

TEST_F(TestRS232Driver, singleLongMessageToOneThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 1, 1);
}

TEST_F(TestRS232Driver, singleLongMessageToTwelveThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 1, 12);
}

TEST_F(TestRS232Driver, MultipleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!";
    runTestsWithParams(messageToSend, 100, 1);
}

 TEST_F(TestRS232Driver, MultipleShortMessageToFourThreadListener) {
    std::string messageToSend = "HelloWorld!";
    runTestsWithParams(messageToSend, 100, 4);
}

TEST_F(TestRS232Driver, MultipleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!";
    runTestsWithParams(messageToSend, 100, 12);
}

TEST_F(TestRS232Driver, MultipleLongMessageToOneThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 100, 1);
}

TEST_F(TestRS232Driver, MultipleLongMessageToFourThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 100, 4);
}

TEST_F(TestRS232Driver, MultipleLongMessageToTwelveThreadListener) {
    std::string messageToSend = genRandomString(10000);
    runTestsWithParams(messageToSend, 100, 12);
}

void TestRS232Driver::runTestsWithParams(const std::string& messageToSend, const int& numberOfSent, const int& numberOfThread){
    std::stringstream truthStream;
    std::stringstream responseStream;
    for(int i = 0; i<numberOfSent; i++){
        truthStream<<messageToSend;
    }
    listenerRS232Driver->setListenerThreadCount(numberOfThread);
    listenerRS232Driver->listen();
    listenerRS232Driver->addCallback(
            [&] (gcm::NetworkMessage nm) {
                responseStream<<nm.data;
                ;}
    );
    this->sendMessage(messageToSend, numberOfSent);
    std::this_thread::sleep_for(std::chrono::milliseconds (100));
    EXPECT_EQ(truthStream.str(),  responseStream.str());
}