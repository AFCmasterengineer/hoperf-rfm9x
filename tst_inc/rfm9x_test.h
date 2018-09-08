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
void Rfm9xSetResetPin();
void Rfm9xResetResetPin();
void Rfm9xSetSpiNssPin();
void Rfm9xResetSpiNssPin();
void Rfm9xSpiTransfer(uint8_t* const data);
void Rfm9xDelay(uint8_t millisec);
#endif
