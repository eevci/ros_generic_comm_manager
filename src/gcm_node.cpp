//
// Created by enver on 22.05.2021.
//
#include<ros/ros.h>
#include<gcm/drivers/ethernet/UDPDriver.h>

void handleCallback1(NetworkMessage networkMessage){
    std::cout<<std::string(((char*)networkMessage.data), networkMessage.size) <<std::endl;
}

void handleCallback2(NetworkMessage networkMessage){
    std::cout<<"Size:"<<networkMessage.size<<"\n";
    std::cout<<std::string(((char*)networkMessage.data), networkMessage.size) <<std::endl;
}

int main(){
    gcm::UDPDriver listener, sender;
    listener.setReceiveAddress("127.0.0.1",13333);
    sender.setTargetAddress("127.0.0.1",13333);
    listener.listen();
    listener.addCallback(handleCallback1);
    listener.addCallback(handleCallback2);
    NetworkMessage nm;
    std::string data = "AAA!Helloaskdaşlsdkaşlsdkasldasdasdlaksdşlasdlkalsşdkaşlsdasdasdasdasd!Helloaskdaşlsdkaşlsdkasldasdasdlaksdşlasdlkalsşdkaşlsdasdasdasdasd!";
    nm.data = (void*)data.data();
    nm.size = data.size();

    for (int i = 0; i < 1000000; ++i) {
        sender.send(nm);
    }
    std::cout<<"HELLOaaO"<<std::endl;
}

