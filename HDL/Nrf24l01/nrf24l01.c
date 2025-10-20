#include "nrf24l01.h"

// 根据您的CubeMX配置修改这些宏定义
#define NRF24_CE_PORT    GPIOA
#define NRF24_CE_PIN     GPIO_PIN_0

#define NRF24_CSN_PORT   GPIOA
#define NRF24_CSN_PIN    GPIO_PIN_1

#define NRF24_IRQ_PORT   GPIOA
#define NRF24_IRQ_PIN    GPIO_PIN_2

#define NRF24_SPI_HANDLE hspi1  // 根据您的SPI配置修改

extern SPI_HandleTypeDef NRF24_SPI_HANDLE;

// 引脚控制函数
void NRF24_CE_High(void) {
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);
}

void NRF24_CE_Low(void) {
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET);
}

void NRF24_CSN_High(void) {
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

void NRF24_CSN_Low(void) {
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}

uint8_t NRF24_IRQ_Read(void) {
    return HAL_GPIO_ReadPin(NRF24_IRQ_PORT, NRF24_IRQ_PIN);
}

// SPI读写函数
uint8_t NRF24_SPI_Transfer(uint8_t data) {
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&NRF24_SPI_HANDLE, &data, &rx_data, 1, HAL_MAX_DELAY);
    return rx_data;
}

// 寄存器操作函数
uint8_t NRF24_ReadReg(uint8_t reg) {
    uint8_t value;
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_R_REGISTER | reg);
    value = NRF24_SPI_Transfer(0xFF);
    NRF24_CSN_High();
    return value;
}

void NRF24_WriteReg(uint8_t reg, uint8_t value) {
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_W_REGISTER | reg);
    NRF24_SPI_Transfer(value);
    NRF24_CSN_High();
}

void NRF24_ReadBuf(uint8_t reg, uint8_t *buf, uint8_t len) {
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_R_REGISTER | reg);
    for(uint8_t i = 0; i < len; i++) {
        buf[i] = NRF24_SPI_Transfer(0xFF);
    }
    NRF24_CSN_High();
}

void NRF24_WriteBuf(uint8_t reg, uint8_t *buf, uint8_t len) {
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_W_REGISTER | reg);
    for(uint8_t i = 0; i < len; i++) {
        NRF24_SPI_Transfer(buf[i]);
    }
    NRF24_CSN_High();
}

// NRF24L01初始化
void NRF24_Init(void) {
    // 等待NRF24L01上电稳定
    HAL_Delay(5);
    
    // 禁用芯片
    NRF24_CE_Low();
    NRF24_CSN_High();
    
    // 配置为发射模式，使能CRC(2字节)，上电
    NRF24_WriteReg(NRF24_REG_CONFIG, 0x0E);
    
    // 使能自动应答
    NRF24_WriteReg(NRF24_REG_EN_AA, 0x01);
    
    // 使能接收管道0
    NRF24_WriteReg(NRF24_REG_EN_RXADDR, 0x01);
    
    // 设置地址宽度为5字节
    NRF24_SetAddressWidth(5);
    
    // 设置自动重传：延迟250us，重试15次
    NRF24_SetRetries(5, 15);
    
    // 设置RF频道
    NRF24_SetChannel(76);
    
    // 设置数据速率2Mbps，发射功率0dBm
    NRF24_SetDataRate(NRF24_DATARATE_2MBPS);
    NRF24_SetTxPower(NRF24_TXPOWER_0DBM);
    
    // 设置载荷长度
    NRF24_SetPayloadSize(0, 8); // 管道0载荷长度为8字节
    
    // 清空FIFO和状态寄存器
    NRF24_FlushTx();
    NRF24_FlushRx();
    NRF24_WriteReg(NRF24_REG_STATUS, 0x70);
}

// 设置RF频道
void NRF24_SetChannel(uint8_t channel) {
    NRF24_WriteReg(NRF24_REG_RF_CH, channel & 0x7F);
}

// 设置数据速率
void NRF24_SetDataRate(uint8_t data_rate) {
    uint8_t rf_setup = NRF24_ReadReg(NRF24_REG_RF_SETUP) & 0xD7;
    
    switch(data_rate) {
        case NRF24_DATARATE_250KBPS:
            rf_setup |= (1 << 5);
            break;
        case NRF24_DATARATE_2MBPS:
            rf_setup |= (1 << 3);
            break;
        default: // 1MBPS
            break;
    }
    NRF24_WriteReg(NRF24_REG_RF_SETUP, rf_setup);
}

// 设置发射功率
void NRF24_SetTxPower(uint8_t power) {
    uint8_t rf_setup = NRF24_ReadReg(NRF24_REG_RF_SETUP) & 0xF9;
    rf_setup |= (power << 1);
    NRF24_WriteReg(NRF24_REG_RF_SETUP, rf_setup);
}

// 设置重传参数
void NRF24_SetRetries(uint8_t delay, uint8_t count) {
    NRF24_WriteReg(NRF24_REG_SETUP_RETR, (delay << 4) | (count & 0x0F));
}

// 设置地址宽度
void NRF24_SetAddressWidth(uint8_t width) {
    NRF24_WriteReg(NRF24_REG_SETUP_AW, (width - 2) & 0x03);
}

// 设置接收地址
void NRF24_SetRxAddress(uint8_t pipe, uint8_t *address) {
    NRF24_WriteBuf(NRF24_REG_RX_ADDR_P0 + pipe, address, 5);
}

// 设置发射地址
void NRF24_SetTxAddress(uint8_t *address) {
    NRF24_WriteBuf(NRF24_REG_TX_ADDR, address, 5);
}

// 设置有效数据长度
void NRF24_SetPayloadSize(uint8_t pipe, uint8_t size) {
    NRF24_WriteReg(NRF24_REG_RX_PW_P0 + pipe, size & 0x3F);
}

// 开始监听
void NRF24_StartListening(void) {
    // 上电并设置为接收模式
    NRF24_WriteReg(NRF24_REG_CONFIG, 0x0F);
    NRF24_CE_High();
    HAL_Delay(1);
}

// 停止监听
void NRF24_StopListening(void) {
    NRF24_CE_Low();
    // 上电并设置为发射模式
    NRF24_WriteReg(NRF24_REG_CONFIG, 0x0E);
    HAL_Delay(1);
}

// 清空TX FIFO
void NRF24_FlushTx(void) {
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_FLUSH_TX);
    NRF24_CSN_High();
}

// 清空RX FIFO
void NRF24_FlushRx(void) {
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_FLUSH_RX);
    NRF24_CSN_High();
}

// 获取状态寄存器
uint8_t NRF24_GetStatus(void) {
    NRF24_CSN_Low();
    uint8_t status = NRF24_SPI_Transfer(NRF24_CMD_NOP);
    NRF24_CSN_High();
    return status;
}

// 检查是否有数据可读
bool NRF24_DataAvailable(void) {
    uint8_t status = NRF24_GetStatus();
    return (status & (1 << 6)); // RX_DR位
}

// 启动NRF24L01发送一次数据
// sendBuff:待发送数据首地址
// length:数据长度
// 返回值:发送完成状况
uint8_t NRF24_TxPacket(uint8_t *sendBuff, uint8_t length) {
    NRF24_CE_Low();
    
    // 写入发射数据
    NRF24_WriteBuf(NRF24_CMD_W_TX_PAYLOAD, sendBuff, length);
    
    NRF24_CE_High(); // 启动发送
    
    // 等待IRQ引脚变低（发送完成）
    while(NRF24_IRQ_Read() != 0);
    
    // 读取状态寄存器
    uint8_t status = NRF24_GetStatus();
    
    // 清除中断标志
    NRF24_WriteReg(NRF24_REG_STATUS, 0x70);
    
    if(status & (1 << 5)) { // TX_DS - 数据发送成功
        return NRF24_TX_SUCCESS;
    }
    else if(status & (1 << 4)) { // MAX_RT - 达到最大重试次数
        NRF24_FlushTx(); // 清空TX FIFO
        return NRF24_TX_MAX_RT;
    }
    
    return NRF24_TX_TIMEOUT;
}

// 读取接收到的数据
void NRF24_ReadData(uint8_t *data, uint8_t length) {
    NRF24_CSN_Low();
    NRF24_SPI_Transfer(NRF24_CMD_R_RX_PAYLOAD);
    for(uint8_t i = 0; i < length; i++) {
        data[i] = NRF24_SPI_Transfer(0xFF);
    }
    NRF24_CSN_High();
    
    // 清除RX_DR中断标志
    NRF24_WriteReg(NRF24_REG_STATUS, (1 << 6));
}

// 复位模块
void NRF24_Reset(void) {
    NRF24_Init();
}

// 游戏手柄发射端初始化
void Gamepad_InitTransmitter(void) {
    NRF24_Init();
    
    // 设置发射地址和接收地址（管道0用于ACK）
    uint8_t tx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t rx_address[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
    
    NRF24_SetTxAddress(tx_address);
    NRF24_SetRxAddress(0, rx_address);
    NRF24_SetPayloadSize(0, sizeof(Gamepad_Report_t));
    
    NRF24_StopListening();
}

// 游戏手柄接收端初始化
void Gamepad_InitReceiver(void) {
    NRF24_Init();
    
    // 设置接收地址
    uint8_t rx_address[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    
    NRF24_SetRxAddress(0, rx_address);
    NRF24_SetPayloadSize(0, sizeof(Gamepad_Report_t));
    
    NRF24_StartListening();
}

// 发送游戏手柄数据
uint8_t Gamepad_SendData(Gamepad_Report_t *gamepad_data) {
    return NRF24_TxPacket((uint8_t*)gamepad_data, sizeof(Gamepad_Report_t));
}

// 接收游戏手柄数据
bool Gamepad_ReceiveData(Gamepad_Report_t *gamepad_data) {
    if(NRF24_DataAvailable()) {
        NRF24_ReadData((uint8_t*)gamepad_data, sizeof(Gamepad_Report_t));
        return true;
    }
    return false;
}
