#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "main.h"
#include "spi.h"
#include "Remote_ADC.h"
#include <stdbool.h>

// NRF24L01寄存器定义
#define NRF24_REG_CONFIG      0x00
#define NRF24_REG_EN_AA       0x01
#define NRF24_REG_EN_RXADDR   0x02
#define NRF24_REG_SETUP_AW    0x03
#define NRF24_REG_SETUP_RETR  0x04
#define NRF24_REG_RF_CH       0x05
#define NRF24_REG_RF_SETUP    0x06
#define NRF24_REG_STATUS      0x07
#define NRF24_REG_RX_ADDR_P0  0x0A
#define NRF24_REG_RX_ADDR_P1  0x0B
#define NRF24_REG_TX_ADDR     0x10
#define NRF24_REG_RX_PW_P0    0x11
#define NRF24_REG_RX_PW_P1    0x12
#define NRF24_REG_FIFO_STATUS 0x17

// 指令定义
#define NRF24_CMD_R_REGISTER    0x00
#define NRF24_CMD_W_REGISTER    0x20
#define NRF24_CMD_R_RX_PAYLOAD  0x61
#define NRF24_CMD_W_TX_PAYLOAD  0xA0
#define NRF24_CMD_FLUSH_TX      0xE1
#define NRF24_CMD_FLUSH_RX      0xE2
#define NRF24_CMD_NOP           0xFF

// 数据速率定义
#define NRF24_DATARATE_250KBPS 0
#define NRF24_DATARATE_1MBPS   1
#define NRF24_DATARATE_2MBPS   2

// 发射功率定义
#define NRF24_TXPOWER_18DBM    0
#define NRF24_TXPOWER_12DBM    1
#define NRF24_TXPOWER_6DBM     2
#define NRF24_TXPOWER_0DBM     3

// 发送状态
#define NRF24_TX_SUCCESS      0
#define NRF24_TX_TIMEOUT      1
#define NRF24_TX_MAX_RT       2


// 根据您的CubeMX配置修改这些宏定义
#define NRF24_CE_PORT    GPIOA
#define NRF24_CE_PIN     GPIO_PIN_0

#define NRF24_CSN_PORT   GPIOA
#define NRF24_CSN_PIN    GPIO_PIN_1

#define NRF24_IRQ_PORT   GPIOA
#define NRF24_IRQ_PIN    GPIO_PIN_2

#define NRF24_SPI_HANDLE hspi1  // 根据您的SPI配置修改

// 函数声明
void NRF24_Init(void);
void NRF24_Reset(void);

// 引脚控制
void NRF24_CE_High(void);
void NRF24_CE_Low(void);
void NRF24_CSN_High(void);
void NRF24_CSN_Low(void);
uint8_t NRF24_IRQ_Read(void);

// 寄存器操作
uint8_t NRF24_ReadReg(uint8_t reg);
void NRF24_WriteReg(uint8_t reg, uint8_t value);
void NRF24_ReadBuf(uint8_t reg, uint8_t *buf, uint8_t len);
void NRF24_WriteBuf(uint8_t reg, uint8_t *buf, uint8_t len);

// 配置函数
void NRF24_SetChannel(uint8_t channel);
void NRF24_SetDataRate(uint8_t data_rate);
void NRF24_SetTxPower(uint8_t power);
void NRF24_SetRetries(uint8_t delay, uint8_t count);
void NRF24_SetAddressWidth(uint8_t width);
void NRF24_SetRxAddress(uint8_t pipe, uint8_t *address);
void NRF24_SetTxAddress(uint8_t *address);
void NRF24_SetPayloadSize(uint8_t pipe, uint8_t size);

// 模式控制
void NRF24_StartListening(void);
void NRF24_StopListening(void);

// 数据收发
uint8_t NRF24_TxPacket(uint8_t *sendBuff, uint8_t length);
void NRF24_ReadData(uint8_t *data, uint8_t length);
void NRF24_FlushTx(void);
void NRF24_FlushRx(void);

// 状态检查
uint8_t NRF24_GetStatus(void);
bool NRF24_DataAvailable(void);

// 游戏手柄专用函数
void Gamepad_InitTransmitter(void);
void Gamepad_InitReceiver(void);
uint8_t Gamepad_SendData(Gamepad_Report_t *gamepad_data);
bool Gamepad_ReceiveData(Gamepad_Report_t *gamepad_data);

#endif /* __NRF24L01_H */
