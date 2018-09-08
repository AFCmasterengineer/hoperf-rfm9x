#include "rfm9x_test.h"

static vector<string> resp;
static rfm9x_t rfm9x;

TEST(Rfm9xTest, Init) {
  Test_Setup();

  RFM9X_Init(&rfm9x);

  ASSERT_EQ(resp.size(), 2);
  ASSERT_EQ(resp[0], "set reset pin");
  ASSERT_EQ(resp[1], "delay: 15");
}

TEST(Rfm9xTest, Reset) {
  Test_Setup();

  RFM9X_Reset(&rfm9x);

  ASSERT_EQ(resp.size(), 3);
  ASSERT_EQ(resp[0], "reset reset pin");
  ASSERT_EQ(resp[1], "delay: 15");
  ASSERT_EQ(resp[2], "set reset pin");
}

TEST(Rfm9xTest, GetVersion) {
  Test_Setup();

  uint8_t version = 0x00;
  RFM9X_GetVersion(&rfm9x, &version);

  ASSERT_EQ(resp.size(), 4);
  ASSERT_EQ(resp[0], "reset spi nss pin");
  ASSERT_EQ(resp[1], "spi transfer: 66");
  ASSERT_EQ(resp[2], "spi transfer: 0");
  ASSERT_EQ(resp[3], "set spi nss pin");
  ASSERT_EQ(version, 0xAA);
}

void Test_Setup() {
  resp.resize(0);
  
  rfm9x = {
    &Rfm9xSetResetPin,
    &Rfm9xResetResetPin,
    &Rfm9xSetSpiNssPin,
    &Rfm9xResetSpiNssPin,
    &Rfm9xDelay,
    &Rfm9xSpiTransfer
  };
}

void Rfm9xSetResetPin() {
  ostringstream os;

  os << "set reset pin";
  resp.push_back(os.str());
}

void Rfm9xResetResetPin() {
  ostringstream os;

  os << "reset reset pin";
  resp.push_back(os.str());
}

void Rfm9xSetSpiNssPin() {
  ostringstream os;

  os << "set spi nss pin";
  resp.push_back(os.str());
}

void Rfm9xResetSpiNssPin() {
  ostringstream os;

  os << "reset spi nss pin";
  resp.push_back(os.str());
}

void Rfm9xSpiTransfer(uint8_t* const data) {
  ostringstream os;

  os << "spi transfer: " << (int)*data;

  *data = 0xAA;
  resp.push_back(os.str());
}

void Rfm9xDelay(uint8_t millisec) {
  ostringstream os;

  os << "delay: " << (int)millisec;
  resp.push_back(os.str());
}