#include "rfm9x_test.h"

static vector<string> resp;
static rfm9x_t rfm9x;
static vector<uint8_t> spi_resp;

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

TEST(Rfm9xTest, GetBitrate) {
  Test_Setup();
  spi_resp.push_back(0x00);
  spi_resp.push_back(0x1a);
  spi_resp.push_back(0x0b);

  uint32_t bitrate = 0;
  RFM9X_GetBitrate(&rfm9x, &bitrate);

  ASSERT_EQ(resp.size(), 5);
  ASSERT_EQ(resp[0], "reset spi nss pin");
  ASSERT_EQ(resp[1], "spi transfer: 2");
  ASSERT_EQ(resp[2], "spi transfer: 0");
  ASSERT_EQ(resp[3], "spi transfer: 26");
  ASSERT_EQ(resp[4], "set spi nss pin");
  ASSERT_EQ(bitrate, 4799);
}

TEST(Rfm9xTest, SetBitrate) {
  Test_Setup();

  uint32_t bitrate = 100000;
  RFM9X_SetBitrate(&rfm9x, &bitrate);

  ASSERT_EQ(resp.size(), 5);
  ASSERT_EQ(resp[0], "reset spi nss pin");
  ASSERT_EQ(resp[1], "spi transfer: 130");
  ASSERT_EQ(resp[2], "spi transfer: 1");
  ASSERT_EQ(resp[3], "spi transfer: 64");
  ASSERT_EQ(resp[4], "set spi nss pin");
}

TEST(Rfm9xTest, GetPowerConfig) {
  Test_Setup();
  spi_resp.push_back(0x00);
  spi_resp.push_back(0x8a);

  uint8_t outputPower;

  RFM9X_GetPower(&rfm9x, &outputPower);

  ASSERT_EQ(resp.size(), 4);
  ASSERT_EQ(resp[0], "reset spi nss pin");
  ASSERT_EQ(resp[1], "spi transfer: 9");
  ASSERT_EQ(resp[2], "spi transfer: 0");
  ASSERT_EQ(resp[3], "set spi nss pin");
  ASSERT_EQ(outputPower, 12);
}

TEST(Rfm9xTest, SetPowerConfig) {
  Test_Setup();

  uint8_t outputPower = 4;

  RFM9X_SetPower(&rfm9x, &outputPower);

  ASSERT_EQ(resp.size(), 4);
  ASSERT_EQ(resp[0], "reset spi nss pin");
  ASSERT_EQ(resp[1], "spi transfer: 137");
  ASSERT_EQ(resp[2], "spi transfer: 130");
  ASSERT_EQ(resp[3], "set spi nss pin");
}

void Test_Setup() {
  resp.resize(0);
  spi_resp.resize(0);

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

  os << "spi transfer: " << (unsigned int)*data;

  uint8_t tmp = 0xAA;

  if (spi_resp.size() > 0) {
    tmp = spi_resp.front();
    spi_resp.erase(spi_resp.begin());
  }

  *data = tmp;
  resp.push_back(os.str());
}

void Rfm9xDelay(uint8_t millisec) {
  ostringstream os;

  os << "delay: " << (int)millisec;
  resp.push_back(os.str());
}