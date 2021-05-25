//
// Created by enver on 25.05.2021.
//
#include "gcm/drivers/ethernet/TestUDPDriver.h"
#include "gcm/drivers/ethernet/TestTCPDriver.h"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
