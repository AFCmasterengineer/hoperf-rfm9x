#include "rfm9x.h"

const uint8_t RFM9X_READ = 0x00;
const uint8_t RFM9X_WRITE = 0x80;

const uint8_t RFM9X_REG_FIFO = 0x00;
const uint8_t RFM9X_REG_MODE = 0x01;
const uint8_t RFM9X_REG_FREQUENCY = 0x06;
const uint8_t RFM9X_REG_SYNC_CONFIG = 0x27;
const uint8_t RFM9X_REG_VERSION = 0x42;
const uint8_t RFM9X_REG_IRQ_FLAGS = 0x3e;

void RFM9X_ReadMessage(const rfm9x_t* const rfm9x, read_func callback) {
  uint8_t com = RFM9X_READ | RFM9X_REG_FIFO;
  uint8_t length = 0;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&length);

  uint8_t data[length];

  for (uint8_t cntr = 0; cntr < length; cntr++) {
    rfm9x->spi_transfer(data + cntr);
  }

  rfm9x->set_spi_nss_pin();

  callback(data, length);
}

void RFM9X_GetVersion(const rfm9x_t* const rfm9x, uint8_t* const version) {
  uint8_t com = RFM9X_READ | RFM9X_REG_VERSION;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(version);
  rfm9x->set_spi_nss_pin();
}

void RFM9X_Init(const rfm9x_t* const rfm9x) {
  rfm9x->set_reset_pin();
  rfm9x->delay(15);
}

void RFM9X_Reset(const rfm9x_t* const rfm9x) {
  rfm9x->reset_reset_pin();
  rfm9x->delay(15);
  rfm9x->set_reset_pin();
}

void RFM9X_GetFrequency(const rfm9x_t* const rfm9x, uint32_t* const frequencyHz) {
  uint8_t com = RFM9X_READ | RFM9X_REG_FREQUENCY;
  uint8_t val;

  *frequencyHz = 0;
  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&val);
  *frequencyHz += val << 16;
  rfm9x->spi_transfer(&val);
  *frequencyHz += val << 8;
  rfm9x->spi_transfer(&val);
  *frequencyHz += val;
  rfm9x->set_spi_nss_pin();
  *frequencyHz *= 61;
}

void RFM9X_GetSyncWord(const rfm9x_t* const rfm9x, uint8_t syncWord[8], uint8_t* const length) {
  uint8_t com = RFM9X_READ | RFM9X_REG_SYNC_CONFIG;
  uint8_t syncConfig;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&syncConfig);
  
  for (uint8_t cntr = 0; cntr < 8; cntr++) {
    rfm9x->spi_transfer(syncWord + cntr);
  }
  
  rfm9x->set_spi_nss_pin();
  *length = (syncConfig & 0b00000111) + 1;
}

void RFM9X_SetSyncWord(const rfm9x_t* const rfm9x, const uint8_t* const syncWord, uint8_t length) {
  uint8_t com = RFM9X_READ | RFM9X_REG_SYNC_CONFIG;
  uint8_t syncConfig;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&syncConfig);
  rfm9x->set_spi_nss_pin();
  length -= 1;
  length &= 0b00000111;
  syncConfig &= 0b11111000;
  syncConfig |= length;
  com = RFM9X_WRITE | RFM9X_REG_SYNC_CONFIG;
  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&syncConfig);

  for (uint8_t cntr = 0; cntr < length + 1; cntr++) {
    uint8_t syncWordByte = *(syncWord + cntr);
    rfm9x->spi_transfer(&syncWordByte);
  }
  
  rfm9x->set_spi_nss_pin();
}

void RFM9X_GetMode(const rfm9x_t* const rfm9x, rfm9x_mode_t* const mode) {
  uint8_t com = RFM9X_READ | RFM9X_REG_MODE;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer((uint8_t *)mode);
  rfm9x->set_spi_nss_pin();

  *mode = (rfm9x_mode_t)(*mode & 0b00000111);
}

void RFM9X_SetMode(const rfm9x_t* const rfm9x, const rfm9x_mode_t* const mode) {
  uint8_t com = RFM9X_READ | RFM9X_REG_MODE;
  uint8_t oldMode = 0;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&oldMode);
  rfm9x->set_spi_nss_pin();

  oldMode &= 0b11111000;
  oldMode |= *mode;
  com = RFM9X_WRITE | RFM9X_REG_MODE;

  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&oldMode);
  rfm9x->set_spi_nss_pin();
}

void RFM9X_GetFlags(const rfm9x_t* const rfm9x, uint16_t* const flags) {
  uint8_t com = RFM9X_READ | RFM9X_REG_IRQ_FLAGS;
  uint8_t flagByte = 0;

  *flags = 0;
  rfm9x->reset_spi_nss_pin();
  rfm9x->spi_transfer(&com);
  rfm9x->spi_transfer(&flagByte);
  *flags += flagByte << 8;
  rfm9x->spi_transfer(&flagByte);
  *flags += flagByte;
  rfm9x->set_spi_nss_pin();
}
