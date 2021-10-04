//
// Created by enver on 23.05.2021.
//
#include "TestTCPDriver.h"


TEST_F(TestTCPDriver, MultipleShortMessageToFourThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerTCPDriver->setListenerThreadCount(4);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}
TEST_F(TestTCPDriver, emptyEndpointTest) {
    EXPECT_EQ (listenerTCPDriver->getSenderEndPoint(),  gcm::BOOST_TCP_ENDPOINT());
}

TEST_F(TestTCPDriver, singleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerTCPDriver->setListenerThreadCount(1);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestTCPDriver, singleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerTCPDriver->setListenerThreadCount(12);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestTCPDriver, singleLongMessageToOneThreadListener) {
    std::string messageToSend = std::string("1",1023);
    listenerTCPDriver->setListenerThreadCount(1);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestTCPDriver, singleLongMessageToTwelveThreadListener) {
    std::string messageToSend = std::string("1",1023);
    listenerTCPDriver->setListenerThreadCount(12);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestTCPDriver, MultipleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerTCPDriver->setListenerThreadCount(1);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}



TEST_F(TestTCPDriver, MultipleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerTCPDriver->setListenerThreadCount(12);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestTCPDriver, MultipleLongMessageToOneThreadListener) {
    std::string messageToSend = std::string("1",1023);
    listenerTCPDriver->setListenerThreadCount(1);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestTCPDriver, MultipleLongMessageToFourThreadListener) {
    std::string messageToSend = std::string("1",1023);
    listenerTCPDriver->setListenerThreadCount(4);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestTCPDriver, MultipleLongMessageToTwelveThreadListener) {
    std::string messageToSend = std::string("1",1023);
    listenerTCPDriver->setListenerThreadCount(12);
    listenerTCPDriver->listen();
    listenerTCPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend,  std::string((char*)nm.data.data(), nm.size));
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}
