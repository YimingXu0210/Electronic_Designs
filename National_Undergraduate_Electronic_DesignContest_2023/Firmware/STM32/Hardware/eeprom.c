#include "eeprom.h"

#define I2C_EEPROM_DEVICE_ADDRESS		0x50

//	P1.6 -> SDA
//	P1.7 -> SCL

void EEPROM_Init(void)
{
	eUSCI_I2C_MasterConfig i2c_cfg;

	GPIO_setAsPeripheralModuleFunctionOutputPin(
		GPIO_PORT_P1, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	i2c_cfg.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
	i2c_cfg.i2cClk = 48000000;
	i2c_cfg.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
	i2c_cfg.byteCounterThreshold = 0;
	i2c_cfg.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
	MAP_I2C_initMaster(EUSCI_B0_BASE, &i2c_cfg);
	MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, I2C_EEPROM_DEVICE_ADDRESS);
	MAP_I2C_enableModule(EUSCI_B0_BASE);
}

void EE_Write(uint16_t add, const uint8_t *data, uint32_t len)
{
	uint32_t moduleInstance = EUSCI_B0_BASE;
	
	if (len == 0)
		return;

	if (len) {
		//1次Start -> n-2次Next -> 1次Finish
		MAP_I2C_masterSendMultiByteStart(moduleInstance, (add >> 8));
		MAP_I2C_masterSendMultiByteNext(moduleInstance, (add & 0xFF));
		
		for (int i = 0; i < len - 1; i++) {
			MAP_I2C_masterSendMultiByteNext(moduleInstance, data[i]);
		}
		MAP_I2C_masterSendMultiByteFinish(moduleInstance, data[len - 1]);
	}
}

void EE_Read(uint16_t add, uint8_t *data, uint32_t len) 
{
	uint32_t moduleInstance = EUSCI_B0_BASE;
	
	if (len == 0)
		return;
	
	MAP_I2C_masterSendMultiByteStart(moduleInstance, (add >> 8));
	MAP_I2C_masterSendMultiByteFinish(moduleInstance, (add & 0xff));
	while (!MAP_I2C_getInterruptStatus(moduleInstance, EUSCI_B_I2C_STOP_INTERRUPT));
	
	if (len == 1) {
		data[0] = MAP_I2C_masterReceiveSingleByte(moduleInstance);
	} else {
		MAP_I2C_masterReceiveStart(moduleInstance);
		for (int i = 0; i < len - 1; i++) {
			while (!MAP_I2C_getInterruptStatus(moduleInstance, EUSCI_B_I2C_RECEIVE_INTERRUPT0));
			data[i] = MAP_I2C_masterReceiveMultiByteNext(moduleInstance);
		}
		data[len - 1] = MAP_I2C_masterReceiveMultiByteFinish(moduleInstance);
	}
}
