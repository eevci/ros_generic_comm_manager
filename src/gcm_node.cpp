//
// Created by enver on 22.05.2021.
//
#include<ros/ros.h>
#include"gcm/CreateUDPComm.h"
#include"gcm/NetworkMessage.h"
#include"gcm/drivers/ethernet/UDPDriver.h"
#include"gcm/drivers/EthernetNetworkDriver.h"

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

void prepareSendCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm::CreateUDPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setTargetAddress(req.targetAddress,req.targetPort);
    driverWithTopic.sb = generateSubscriber(req.sendMessageTopicName);
}

void prepareReceiveCapability(std::shared_ptr<gcm::EthernetNetworkDriver>& ethernetNetworkDriver,
                           const gcm::CreateUDPComm::Request& req,
                           DriverWithTopic& driverWithTopic){
    ethernetNetworkDriver->setReceiveAddress(req.receiveAddress,req.receivePort);
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
    prepareSendCapability(udpDriver, req, driverWithTopic);
    if(req.shouldListen)
        prepareReceiveCapability(udpDriver, req, driverWithTopic);
    driverWithTopic.driver = udpDriver;
    drivers[req.sendMessageTopicName] = driverWithTopic;
    res.result = true;
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "gcm");
    nh = boost::make_shared<ros::NodeHandle>();
    ros::ServiceServer service = nh->advertiseService("registerUDPComm", registerUDPComm);
    ros::spin();
}

