//
// Created by enver on 23.05.2021.
//
#include "TestUDPDriver.h"


TEST_F(TestUDPDriver, emptyEndpointTest) {
    EXPECT_EQ (listenerUDPDriver->getSenderEndPoint(),  gcm::BOOST_UDP_ENDPOINT());
}

TEST_F(TestUDPDriver, singleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerUDPDriver->setListenerThreadCount(1);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestUDPDriver, singleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerUDPDriver->setListenerThreadCount(12);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestUDPDriver, singleLongMessageToOneThreadListener) {
    std::string messageToSend = std::string(1024,'1');
    listenerUDPDriver->setListenerThreadCount(1);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestUDPDriver, singleLongMessageToTwelveThreadListener) {
    std::string messageToSend = std::string(1024,'1');
    listenerUDPDriver->setListenerThreadCount(12);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1);
}

TEST_F(TestUDPDriver, MultipleShortMessageToOneThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerUDPDriver->setListenerThreadCount(1);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestUDPDriver, MultipleShortMessageToFourThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerUDPDriver->setListenerThreadCount(4);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestUDPDriver, MultipleShortMessageToTwelveThreadListener) {
    std::string messageToSend = "HelloWorld!";
    listenerUDPDriver->setListenerThreadCount(12);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestUDPDriver, MultipleLongMessageToOneThreadListener) {
    std::string messageToSend = std::string(1024,'1');
    listenerUDPDriver->setListenerThreadCount(1);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestUDPDriver, MultipleLongMessageToFourThreadListener) {
    std::string messageToSend = std::string(1024,'1');
    listenerUDPDriver->setListenerThreadCount(4);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);

                ;}
    );
    this->sendMessage(messageToSend, 1000);
}

TEST_F(TestUDPDriver, MultipleLongMessageToTwelveThreadListener) {
    std::string messageToSend = std::string(1024,'1');
    listenerUDPDriver->setListenerThreadCount(12);
    listenerUDPDriver->listen();
    listenerUDPDriver->addCallback(
            [messageToSend] (gcm::NetworkMessage nm) {
                EXPECT_EQ(messageToSend, nm.data);
                ;}
    );
    this->sendMessage(messageToSend, 1000);
}
