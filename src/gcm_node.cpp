//
// Created by enver on 22.05.2021.
//
#include<ros/ros.h>
#include "gcm_msgs/CreateUDPComm.h"
#include "gcm_msgs/CreateTCPComm.h"
#include "gcm_msgs/CreateRS232Comm.h"
#include "gcm_msgs/NetworkMessage.h"
#include "gcm/drivers/ethernet/UDPDriver.h"
#include "gcm/drivers/ethernet/TCPDriver.h"
#include "gcm/drivers/serial/RS232Driver.h"
#include <unordered_map>

ros::NodeHandlePtr nh;
typedef struct DriverWithTopic{
    ros::Subscriber sb;
    ros::Publisher pb;
    std::shared_ptr<gcm::NetworkDriver> driver;
} DriverWithTopic;

std::unordered_map<std::string, DriverWithTopic> drivers;

ros::Subscriber generateSubscriber(const std::string& topicName){
    return nh->subscribe<gcm_msgs::NetworkMessage>(topicName, 10, [topicName] (const gcm_msgs::NetworkMessageConstPtr& nm){
        drivers[topicName].driver->send(*nm);
    });
}

void prepareUDPSendCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm_msgs::CreateUDPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setTargetAddress(req.targetAddress,req.targetPort);
    driverWithTopic.sb = generateSubscriber(req.sendMessageTopicName);
}

void prepareTCPSendCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm_msgs::CreateTCPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setTargetAddress(req.targetAddress,req.targetPort);
    driverWithTopic.sb = generateSubscriber(req.sendMessageTopicName);
}

void prepareRS232SendCapability(std::shared_ptr<gcm::RS232Driver>& rs232Driver,
                           const gcm_msgs::CreateRS232Comm::Request& req){
    drivers[req.sendMessageTopicName].sb = generateSubscriber(req.sendMessageTopicName);
}

void prepareUDPReceiveCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm_msgs::CreateUDPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setReceiveAddress("127.0.0.1",req.receivePort);
    ros::Publisher listenerResultPublisher = nh->advertise<gcm_msgs::NetworkMessage>(req.receiveMessageTopicName, 1000);
    driverWithTopic.pb = listenerResultPublisher;
    ethernetNetworkDriver->setListenerThreadCount(req.receiveThreadCount);
    ethernetNetworkDriver->addCallback([req] (const gcm_msgs::NetworkMessage& nm){
        drivers[req.sendMessageTopicName].pb.publish(nm);
    });
    ethernetNetworkDriver->open();
    ethernetNetworkDriver->listen();
}

void prepareTCPReceiveCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& tcpDriver,
                           const gcm_msgs::CreateTCPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    tcpDriver->setReceiveAddress("127.0.0.1",req.receivePort);
    ros::Publisher listenerResultPublisher = nh->advertise<gcm_msgs::NetworkMessage>(req.receiveMessageTopicName, 1000);
    driverWithTopic.pb = listenerResultPublisher;
    tcpDriver->setListenerThreadCount(req.receiveThreadCount);
    tcpDriver->addCallback([req] (const gcm_msgs::NetworkMessage& nm){
        drivers[req.sendMessageTopicName].pb.publish(nm);
    });
    tcpDriver->open();
    tcpDriver->listen();
}

void prepareRS232ReceiveCapability(std::shared_ptr<gcm::RS232Driver>& rs232Driver,
                           const gcm_msgs::CreateRS232Comm::Request& req){

    rs232Driver->setBaudrate(req.baudrate);
    rs232Driver->setDevice(req.device);
    rs232Driver->setListenerThreadCount(req.receiveThreadCount);

    ros::Publisher listenerResultPublisher = nh->advertise<gcm_msgs::NetworkMessage>(req.receiveMessageTopicName, 1000);
    drivers[req.sendMessageTopicName].pb = listenerResultPublisher;
    rs232Driver->addCallback([req] (const gcm_msgs::NetworkMessage& nm){
        drivers[req.sendMessageTopicName].pb.publish(nm);
    });
    rs232Driver->open();
    rs232Driver->listen();
}

bool registerUDPComm(gcm_msgs::CreateUDPComm::Request  &req,
                     gcm_msgs::CreateUDPComm::Response &res){
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

bool registerTCPComm(gcm_msgs::CreateTCPComm::Request  &req,
                     gcm_msgs::CreateTCPComm::Response &res){
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

bool registerRS232Comm(gcm_msgs::CreateRS232Comm::Request  &req,
                     gcm_msgs::CreateRS232Comm::Response &res){
    DriverWithTopic driverWithTopic;
    std::shared_ptr<gcm::RS232Driver> rs232Driver = std::make_shared<gcm::RS232Driver>();
    driverWithTopic.driver = rs232Driver;
    drivers[req.sendMessageTopicName] = driverWithTopic;
    prepareRS232ReceiveCapability(rs232Driver, req);
    prepareRS232SendCapability(rs232Driver, req);
    res.result = true;
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "gcm");
    nh = boost::make_shared<ros::NodeHandle>();
    ros::ServiceServer service_udp = nh->advertiseService("registerUDPComm", registerUDPComm);
    ros::ServiceServer service_tcp = nh->advertiseService("registerTCPComm", registerTCPComm);
    ros::ServiceServer service_rs232 = nh->advertiseService("registerRS232Comm", registerRS232Comm);
    ros::spin();
}

