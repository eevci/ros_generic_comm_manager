//
// Created by enver on 22.05.2021.
//
#include<ros/ros.h>
#include<gcm/drivers/ethernet/UDPDriver.h>



int main(){
    gcm::UDPDriver listener, sender;
    listener.setReceiveAddress("127.0.0.1",13333);
    sender.setTargetAddress("127.0.0.1",13333);
    listener.listen();
    listener.addCallback(handleCallback1);
    listener.addCallback(handleCallback2);
    listener.setListenerThreadCount(1);
    NetworkMessage nm;
    std::string data = "AAA!Helloaskdaşlsdkaşlsdkasldasdasdlaksdşlasdlkalsşdkaşlsdasdasdasdasd!Helloaskdaşlsdkaşlsdkasldasdasdlaksdşlasdlkalsşdkaşlsdasdasdasdasd!";
    nm.data = (void*)data.data();
    nm.size = data.size();

    for (int i = 0; i < 10; ++i) {
        sender.send(nm);
        std::this_thread::sleep_for(std::chrono::seconds (1));

    }
    std::cout<<"HELLOaaO"<<std::endl;
}

