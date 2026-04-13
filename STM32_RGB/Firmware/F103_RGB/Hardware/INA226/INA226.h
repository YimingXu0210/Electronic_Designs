#ifndef INC_INA226_H_
#define INC_INA226_H_

void INA226_init(void);
//仅使用了获取电压电流功率3个功能
float INA226_GetBusV(void);
float INA226_GetCurrent(void);
float INA226_GetPower(void);

#endif /* INC_IIC_INA226_H_ */
