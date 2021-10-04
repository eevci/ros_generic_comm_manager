//
// Created by enver on 22.05.2021.
//
#include<ros/ros.h>
#include"gcm/CreateUDPComm.h"
#include"gcm/CreateTCPComm.h"
#include"gcm/NetworkMessage.h"
#include"gcm/drivers/ethernet/UDPDriver.h"
#include"gcm/drivers/ethernet/TCPDriver.h"

ros::NodeHandlePtr nh;
typedef struct DriverWithTopic{
    ros::Subscriber sb;
    ros::Publisher pb;
    std::shared_ptr<gcm::NetworkDriver> driver;
} DriverWithTopic;

std::unordered_map<std::string, DriverWithTopic> drivers;

ros::Subscriber generateSubscriber(const std::string& topicName){
    return nh->subscribe<gcm::NetworkMessage>(topicName, 10, [topicName] (const gcm::NetworkMessageConstPtr& nm){
        drivers[topicName].driver->send(*nm);
    });
}

void prepareUDPSendCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm::CreateUDPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setTargetAddress(req.targetAddress,req.targetPort);
    driverWithTopic.sb = generateSubscriber(req.sendMessageTopicName);
}

void prepareTCPSendCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm::CreateTCPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setTargetAddress(req.targetAddress,req.targetPort);
    driverWithTopic.sb = generateSubscriber(req.sendMessageTopicName);
}

void prepareUDPReceiveCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm::CreateUDPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setReceiveAddress("127.0.0.1",req.receivePort);
    ros::Publisher listenerResultPublisher = nh->advertise<gcm::NetworkMessage>(req.receiveMessageTopicName, 1000);
    driverWithTopic.pb = listenerResultPublisher;
    ethernetNetworkDriver->setListenerThreadCount(req.receiveThreadCount);
    ethernetNetworkDriver->addCallback([req] (const gcm::NetworkMessage& nm){
        drivers[req.sendMessageTopicName].pb.publish(nm);
    });
    ethernetNetworkDriver->listen();
}

void prepareTCPReceiveCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm::CreateTCPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setReceiveAddress("127.0.0.1",req.receivePort);
    ros::Publisher listenerResultPublisher = nh->advertise<gcm::NetworkMessage>(req.receiveMessageTopicName, 1000);
    driverWithTopic.pb = listenerResultPublisher;
    ethernetNetworkDriver->setListenerThreadCount(req.receiveThreadCount);
    ethernetNetworkDriver->addCallback([req] (const gcm::NetworkMessage& nm){
        drivers[req.sendMessageTopicName].pb.publish(nm);
    });
    ethernetNetworkDriver->listen();
}

bool registerUDPComm(gcm::CreateUDPComm::Request  &req,
                     gcm::CreateUDPComm::Response &res){
    DriverWithTopic driverWithTopic;
    std::shared_ptr<gcm::EthernetNetworkDriver> udpDriver = std::make_shared<gcm::UDPDriver>();
    prepareUDPSendCapability(udpDriver, req, driverWithTopic);
    if(req.shouldListen)
        prepareUDPReceiveCapability(udpDriver, req, driverWithTopic);
    driverWithTopic.driver = udpDriver;
    drivers[req.sendMessageTopicName] = driverWithTopic;
    res.result = true;
    return true;
}

bool registerTCPComm(gcm::CreateTCPComm::Request  &req,
                     gcm::CreateTCPComm::Response &res){
    DriverWithTopic driverWithTopic;
    std::shared_ptr<gcm::EthernetNetworkDriver> tcpDriver = std::make_shared<gcm::TCPDriver>();
    prepareTCPSendCapability(tcpDriver, req, driverWithTopic);
    if(req.shouldListen)
        prepareTCPReceiveCapability(tcpDriver, req, driverWithTopic);
    driverWithTopic.driver = tcpDriver;
    drivers[req.sendMessageTopicName] = driverWithTopic;
    res.result = true;
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "gcm");
    nh = boost::make_shared<ros::NodeHandle>();
    ros::ServiceServer service_udp = nh->advertiseService("registerUDPComm", registerUDPComm);
    ros::ServiceServer service_tcp = nh->advertiseService("registerTCPComm", registerTCPComm);
    ros::spin();
}

