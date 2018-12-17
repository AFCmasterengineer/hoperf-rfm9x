#ifndef __rfm9x_test_h__
#define __rfm9x_test_h__
#include <string>
#include <vector>
#include <iostream>
#include "gtest/gtest.h"
#include "rfm9x.h"

using namespace std;

void Test();
void Test_Setup();
rfm9x_result_t Rfm9xSetResetPin();
rfm9x_result_t Rfm9xResetResetPin();
rfm9x_result_t Rfm9xSetSpiNssPin();
rfm9x_result_t Rfm9xResetSpiNssPin();
rfm9x_result_t Rfm9xSpiTransfer(uint8_t* const data);
rfm9x_result_t Rfm9xDelay(uint8_t millisec);
#endif
