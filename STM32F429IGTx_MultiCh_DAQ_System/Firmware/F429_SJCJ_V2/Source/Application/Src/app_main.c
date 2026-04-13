#include "app_main.h"
#include "app_uart.h"
#include "app_math.h"

#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "max31856.h"
#include "INA226.h"

max31856_t max31856_therm[TMP_GP] = {
//	{&hspi1, 	
//		{MAX31856_CS_0_GPIO_Port, 		MAX31856_CS_0_Pin},
//		{MAX31856_FAULT_0_GPIO_Port, 	MAX31856_FAULT_0_Pin},
//		{MAX31856_DRDY_0_GPIO_Port, 	MAX31856_DRDY_0_Pin}
//	},
	{&hspi1, 	
		{MAX31856_CS_1_GPIO_Port, 		MAX31856_CS_1_Pin},
		{MAX31856_FAULT_1_GPIO_Port, 	MAX31856_FAULT_1_Pin},
		{MAX31856_DRDY_1_GPIO_Port, 	MAX31856_DRDY_1_Pin}
	},
	{&hspi1, 	
		{MAX31856_CS_2_GPIO_Port, 		MAX31856_CS_2_Pin},
		{MAX31856_FAULT_2_GPIO_Port, 	MAX31856_FAULT_2_Pin},
		{MAX31856_DRDY_2_GPIO_Port, 	MAX31856_DRDY_2_Pin}
	},
	{&hspi1, 	
		{MAX31856_CS_3_GPIO_Port, 		MAX31856_CS_3_Pin},
		{MAX31856_FAULT_3_GPIO_Port, 	MAX31856_FAULT_3_Pin},
		{MAX31856_DRDY_3_GPIO_Port, 	MAX31856_DRDY_3_Pin}
	},
	{&hspi1, 	
		{MAX31856_CS_4_GPIO_Port, 		MAX31856_CS_4_Pin},
		{MAX31856_FAULT_4_GPIO_Port, 	MAX31856_FAULT_4_Pin},
		{MAX31856_DRDY_4_GPIO_Port, 	MAX31856_DRDY_4_Pin}
	},
	{&hspi1, 	
		{MAX31856_CS_5_GPIO_Port, 		MAX31856_CS_5_Pin},
		{MAX31856_FAULT_5_GPIO_Port, 	MAX31856_FAULT_5_Pin},
		{MAX31856_DRDY_5_GPIO_Port, 	MAX31856_DRDY_5_Pin}
	},
	{&hspi1, 	
		{MAX31856_CS_6_GPIO_Port, 		MAX31856_CS_6_Pin},
		{MAX31856_FAULT_6_GPIO_Port, 	MAX31856_FAULT_6_Pin},
		{MAX31856_DRDY_6_GPIO_Port, 	MAX31856_DRDY_6_Pin}
	},
	{&hspi1, 	
		{MAX31856_CS_7_GPIO_Port, 		MAX31856_CS_7_Pin},
		{MAX31856_FAULT_7_GPIO_Port, 	MAX31856_FAULT_7_Pin},
		{MAX31856_DRDY_7_GPIO_Port, 	MAX31856_DRDY_7_Pin}
	}
};

Data_Docker_Struct Data_T_P = {0};
uint8_t Get_Tmp_Flag = 0, Get_Prs_Flag = 0;

void ADG1606_Addr(uint8_t add);
void ADG1409_Addr(uint8_t add);
void pack_data(const Data_Docker_Struct *data, uint8_t frame[1 + 1 + (TMP_CH + PRS_CH + FRT_CH) * 4 + 1], size_t *frame_size);


uint32_t uwTick_T_Get_Tmp = 0;
void Get_Tmp_Proc(void) {

	if(uwTick - uwTick_T_Get_Tmp < 25) return;
	uwTick_T_Get_Tmp = uwTick;

	static uint8_t STAT = 0;
	static uint8_t add = 0;
	static uint8_t Ch_Get_Stat = 0x00;
	static uint32_t uwTick_Ch_Get_T = 0;
	
	switch(STAT) {
		case 0:
			if(Get_Tmp_Flag) {
				
				add = 0;
				ADG1409_Addr(add);
				
				for(int i = 0; i < TMP_GP; i++) {
					max31856_trigger_one_shot(&max31856_therm[i]);
					max31856_clear_fault_status(&max31856_therm[i]);
				}
				
				uwTick_Ch_Get_T = uwTick;
				Ch_Get_Stat = 0x00;
				
				STAT++;
				
				Get_Tmp_Flag = 0;
			}

		break;
			
		case 1: case 2: case 3: case 4:
			
			for(int i = 0; i < TMP_GP; i++) {

				if(HAL_GPIO_ReadPin(max31856_therm[i].drdy_pin.gpio_port, max31856_therm[i].drdy_pin.gpio_pin) == GPIO_PIN_RESET) {	 // 数据准备就绪
					
					Ch_Get_Stat |= 0x01 << i;  // 对已就绪max做处理
					
					float tmp = max31856_read_TC_temp(&max31856_therm[i]);  // 读取温度
					
					max31856_read_fault(&max31856_therm[i]);  // 读取故障码
					
					if(max31856_therm[i].sr.val == 0) {  // 无故障
						
						Data_T_P.Temperature[add + 4 * i] = tmp;
						
					} else {  // 有故障，清零数据
						
						Data_T_P.Temperature[add + 4 * i] = 0;
						
					}
					
					ps("{T%d}%d %f 0x%02X\r\n", add + 4 * i, uwTick, Data_T_P.Temperature[add + 4 * i], max31856_therm[i].sr.val);

				}
				
			}
			
			if(Ch_Get_Stat == 0x7F || (uwTick - uwTick_Ch_Get_T >= 210)) {  // 如果所有max的数据都就绪，或者超时
				
				for(int i = 0; i < TMP_GP; i++) {
					if((Ch_Get_Stat & (0x01 << i)) == 0) {  // 对超时未就绪max的数据清零
						
						Data_T_P.Temperature[add + 4 * i] = 0;
						
					}
				}

				add++;

				if(add < TMP_CH / TMP_GP) {
					ADG1409_Addr(add);
					
					for(int i = 0; i < TMP_GP; i++) {
						max31856_trigger_one_shot(&max31856_therm[i]);
						max31856_clear_fault_status(&max31856_therm[i]);
					}
					
					uwTick_Ch_Get_T = uwTick;
					Ch_Get_Stat = 0x00;
				}
				
				STAT++;
			}
		
		break;
			
		case 5:
			
			STAT = 0;
		
		break;
		
		
	}

}

//double Pressure_Reg_Coefficients[PRS_CH][7] = {
//	
//	{	0.05845218343676353,
//		0.9657424788941145,
//		0.005133142908669566,
//		-0.0004218163092498804,
//		1.2905892718061132e-05,
//		8.074146931764824e-08,
//		-7.974580943983287e-09},
//	
//		
//	};

//double Pressure_Reg_M[PRS_CH] = {
//		0.9943,		// 1
//		0.993,		// 2
//		0.993,		// 3
//		0.993,		// 4
//		0.993,		// 5
//		0.993,		// 6
//		0.993,		// 7
//		0.993,		// 8
//		0.993,		// 9
//		0.993,		// 10
//		0.993,		// 11
//		0.993,		// 12
//		0.993,		// 13
//		0.993			// 14
//	};

double Pressure_Reg_Coefficients[PRS_CH][7] = {
        {       0.46147944060639556,
                0.8137288869667052,
                0.017807806564140955,
                -0.0002100563262724486,
                -2.4386395829692178e-05,
                -4.53384801508742e-07,
                4.7513445203000376e-08},
        {       0.47124073966146085,
                0.8101900792694458,
                0.01779171269259081,
                -0.00020257073382007202,
                -2.3999138742680185e-05,
                -4.522374063315753e-07,
                4.6078173888252465e-08},
        {       0.46065092027695,
                0.813754265329448,
                0.0177146664793686,
                -0.0002120087126865018,
                -2.4299434609589842e-05,
                -4.483278645313962e-07,
                4.7301048674105364e-08},
        {       0.4680429839411673,
                0.8095307933683297,
                0.017773599710702852,
                -0.00020142844170095914,
                -2.390486014972147e-05,
                -4.5115048848696727e-07,
                4.570508909617491e-08},
        {       0.46179570732830033,
                0.8125024016076007,
                0.017702372753001742,
                -0.00021052753136487185,
                -2.4212264491310124e-05,
                -4.471192659121325e-07,
                4.707156064125943e-08},
        {       0.4594465316128442,
                0.8130530691363309,
                0.017729145142729774,
                -0.00021000368081137917,
                -2.4228384126406416e-05,
                -4.49160375391596e-07,
                4.6960020285008274e-08},
        {       0.4591852806969339,
                0.8118571045580758,
                0.017731512780112934,
                -0.00020850821414918717,
                -2.4168892755907333e-05,
                -4.4851711507118784e-07,
                4.6867916141825e-08},
        {       0.46272575652280246,
                0.8120911999257512,
                0.01771002127711097,
                -0.0002089208800287644,
                -2.4149340947631593e-05,
                -4.476739614315242e-07,
                4.679604275117594e-08},
        {       0.4596311082791962,
                0.8122488937770843,
                0.017746663344450866,
                -0.0002087837087413914,
                -2.4213138498098342e-05,
                -4.500450606815858e-07,
                4.6924443679683896e-08},
        {       0.46288656297813535,
                0.811764941790504,
                0.01775487976806826,
                -0.00020708365612365104,
                -2.4148666601308024e-05,
                -4.5054074024768635e-07,
                4.6665331344716996e-08},
        {       0.4592282203625171,
                0.8124284455152317,
                0.017736951918011154,
                -0.00020924308164520342,
                -2.4213087063576773e-05,
                -4.4944782065553195e-07,
                4.6929901834095884e-08},
        {       0.467273712387656,
                0.8108589663362329,
                0.01771389844696686,
                -0.00020614153395950862,
                -2.4025747695409724e-05,
                -4.4803351484790186e-07,
                4.629178553808686e-08},
        {       0.46174174250121247,
                0.8134138996354918,
                0.017794615195875742,
                -0.00020983558362641492,
                -2.4358271074072243e-05,
                -4.532081231606569e-07,
                4.7373628862140136e-08},
        {       0.4617877938130084,
                0.8130339448915909,
                0.017816650953732197,
                -0.0002078479637292373,
                -2.429805065986214e-05,
                -4.5429522632715126e-07,
                4.708199766347216e-08}
        };

uint32_t uwTick_T_Get_Prs = 0;
void Get_Prs_Proc(void) {

	if(uwTick - uwTick_T_Get_Prs < 12) return;
	uwTick_T_Get_Prs = uwTick;
	
	static uint8_t STAT = 0;
	static uint8_t add = 0;
	
	static uint8_t ct = 0;
	static float tmp[PRS_GET_NUM] = {0};
	
	switch(STAT) {
		case 0:
			if(Get_Prs_Flag) {
				
				add = 0;
				ADG1606_Addr(add);
				
				STAT++;
				
				Get_Prs_Flag = 0;
			}

		break;
			
		case 1:
					
			tmp[ct] = INA226_GetCurrent() * 1000.f;
//			ps("{OI%d}%.4f\r\n", add, tmp[ct]);

			ct++;
			ct %= PRS_GET_NUM;
			if(ct == 0) {
			
				float tmp_avg = 0, tmp_max = tmp[0], tmp_min = tmp[0];
				for (int i = 0; i < PRS_GET_NUM; i++) {

					if (tmp[i] > tmp_max) tmp_max = tmp[i];
					if (tmp[i] < tmp_min) tmp_min = tmp[i];
					tmp_avg += tmp[i];
					
				}
				tmp_avg = (tmp_avg - tmp_max - tmp_min) / (PRS_GET_NUM - 2);
				
//				tmp_avg *= Pressure_Reg_M[add];
				
				tmp_avg = App_Math_Poly_Eval(Pressure_Reg_Coefficients[add], 6, tmp_avg);
				
				Data_T_P.Pressure[add] = (tmp_avg - 4) * 3.75f;			// 4-20mA <--> 0-60Bar
				if(Data_T_P.Pressure[add] < 0) Data_T_P.Pressure[add] = 0;
				
				ps("{I%d}%.4f\r\n", add, tmp_avg);
				
				add++;
				
				if(add < PRS_CH) {
					
					ADG1606_Addr(add);
					
				}
				
			}
			
			if(add == PRS_CH) STAT++;
		
		break;
			
		case 2:
			
			STAT = 0;
		
		break;
		
		
	}
	
//	tmp[ct] = INA226_GetCurrent() * 1000.f;
//	ps("{OI%d}%.4f\r\n", add, tmp[ct]);

//	ct++;
//	ct %= PRS_GET_NUM;
//	if(ct == 0) {
//	
//		float tmp_avg = 0;
//		for(int i = 0; i < PRS_GET_NUM; i++) tmp_avg += tmp[i];
//		tmp_avg /= PRS_GET_NUM;
//		
//		tmp_avg = App_Math_Poly_Eval(Pressure_Reg_Coefficients, 6, tmp_avg);
//		
//		Data_T_P.Pressure[add] = (tmp_avg - 4) * 3.75f;
//		if(Data_T_P.Pressure[add] < 0) Data_T_P.Pressure[add] = 0;
//		
//		ps("{I%d}%.4f\r\n", add, tmp_avg);
//		
//		add++;
//		add %= PRS_CH;
//		ADG1606_Addr(add);
//		
//	}
}

void Put_Data_Proc(void) {
	static uint32_t uwTick_T = 0;
	if(uwTick - uwTick_T < 1000) return;
	uwTick_T = uwTick;
	
    size_t frame_size;
    uint8_t data_frame[1 + 1 + (TMP_CH + PRS_CH + FRT_CH) * 4 + 1] = {0};
	
	pack_data(&Data_T_P, data_frame, &frame_size);
	
	HAL_UART_Transmit(&huart8, data_frame, frame_size, 0xFFFF);

	Get_Tmp_Flag = 1;
	Get_Prs_Flag = 1;
	
	uwTick_T_Get_Tmp = uwTick + 25;
	uwTick_T_Get_Prs = uwTick + 12;
	
	ps("Put_Data_Done\r\n");
	
}


int app_main(void) {
	Delay_Init(180);
	
	ADG1606_Addr(0);
	ADG1409_Addr(0);
	
	INA226_init();
								
	for(int i = 0; i < TMP_GP; i++) {
		max31856_init(&max31856_therm[i]);
		max31856_set_noise_filter(&max31856_therm[i], CR0_FILTER_OUT_50Hz);
		max31856_set_cold_junction_enable(&max31856_therm[i], CR0_CJ_ENABLED);
		max31856_set_thermocouple_type(&max31856_therm[i], CR1_TC_TYPE_K);
		max31856_set_average_samples(&max31856_therm[i], CR1_AVG_TC_SAMPLES_1);
		max31856_set_open_circuit_fault_detection(&max31856_therm[i], CR0_OC_DETECT_ENABLED_R_LESS_5k);
		max31856_set_conversion_mode(&max31856_therm[i], CR0_CONV_OFF);
		max31856_set_fault_mode(&max31856_therm[i], CR0_FAULT_INTERRUPT_MODE);
	}
	
	delay_ms(1000);
	ps("HELLO STM32F429IGT6\r\n");
	
	while (1) {
		
		Get_Tmp_Proc();
		Get_Prs_Proc();
		Put_Data_Proc();
		
	}
	
}

void ADG1606_Addr(uint8_t add) {
	HAL_GPIO_WritePin(ADG16_EN_GPIO_Port, ADG16_EN_Pin, GPIO_PIN_RESET);
	delay_ms(1);
	GPIOI->BSRR = 0x0F << 16U;
	GPIOI->BSRR = add & 0x0F;
	delay_ms(1);
	HAL_GPIO_WritePin(ADG16_EN_GPIO_Port, ADG16_EN_Pin, GPIO_PIN_SET);
	delay_ms(1);
}

void ADG1409_Addr(uint8_t add) {
	HAL_GPIO_WritePin(ADG_EN_GPIO_Port, ADG_EN_Pin, GPIO_PIN_RESET);
	delay_ms(1);
	GPIOI->BSRR = 0x30 << 16U;
	GPIOI->BSRR = (add << 4) & 0x30;
	delay_ms(1);
	HAL_GPIO_WritePin(ADG_EN_GPIO_Port, ADG_EN_Pin, GPIO_PIN_SET);
	delay_ms(1);
}

static uint32_t bswap32(uint32_t val) {
    return ((val >> 24) & 0x000000FF) | 
           ((val >> 8)  & 0x0000FF00) | 
           ((val << 8)  & 0x00FF0000) | 
           ((val << 24) & 0xFF000000);
}

void pack_data(const Data_Docker_Struct *data, uint8_t frame[1 + 1 + (TMP_CH + PRS_CH + FRT_CH) * 4 + 1], size_t *frame_size) {
    // Frame structure
//    static uint8_t frame[1 + 1 + (TMP_CH + PRS_CH + FRT_CH) * 4 + 1]; // Start flag + length + data + checksum
    size_t data_length = (TMP_CH + PRS_CH + FRT_CH) * 4;
    uint8_t checksum = 0;
    
    // Start Flag
    frame[0] = 0xAA;
    
    // Data Length
    frame[1] = data_length & 0xFF; // We use only the lower 8 bits

    // Pack data
    uint8_t *data_ptr = &frame[2];
    
    // Pack Temperature
    for (int i = 0; i < TMP_CH; i++) {
		
//		frame[2 + i*4 + 0] = (uint8_t)data->Temperature[i];
//		frame[2 + i*4 + 1] = (uint8_t)(data->Temperature[i] >> 8);
//		frame[2 + i*4 + 2] = (uint8_t)(data->Temperature[i] >> 16);
//		frame[2 + i*4 + 2] = (uint8_t)(data->Temperature[i] >> 24);
        uint32_t temp;
        memcpy(&temp, &data->Temperature[i], sizeof(float));
//		ps("A:%d\r\n", temp);
        temp = bswap32(temp); // Use custom byte swap function
//		ps("B:%d\r\n", temp);
        memcpy(data_ptr, &temp, sizeof(uint32_t));
        data_ptr += sizeof(uint32_t);
    }

    // Pack Pressure
    for (int i = 0; i < PRS_CH; i++) {
        uint32_t pres;
        memcpy(&pres, &data->Pressure[i], sizeof(float));
        pres = bswap32(pres); // Use custom byte swap function
        memcpy(data_ptr, &pres, sizeof(uint32_t));
        data_ptr += sizeof(uint32_t);
    }

    // Pack Flow Rate
    for (int i = 0; i < FRT_CH; i++) {
        uint32_t flow;
        memcpy(&flow, &data->Flow_Rate[i], sizeof(float));
        flow = bswap32(flow); // Use custom byte swap function
        memcpy(data_ptr, &flow, sizeof(uint32_t));
        data_ptr += sizeof(uint32_t);
    }

    // Calculate Checksum
    for (size_t i = 0; i < 2 + data_length; i++) {
        checksum += frame[i];
    }

    // Add Checksum
    frame[2 + data_length] = checksum;

    // Set the frame size
    *frame_size = data_length + 3;
    
//    return frame;
}
