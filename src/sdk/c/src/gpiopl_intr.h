/*
 * gpiopl_intr.h
 *
 *  Created on: 2017Äê4ÔÂ24ÈÕ
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
 */

#ifndef GPIOPL_INTR_H_
#define GPIOPL_INTR_H_

#include <stdio.h>
#include "xadcps.h"
#include "xscugic.h"
#include "xil_types.h"
#include "xparameters.h"
#include "xgpio.h"


#define AXI_GPIO_DEV_ID	        XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_INTR_ID            XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR

#define GPIO_INTR_MASK          0x00000001

#define PS_BRAM_MASK            0x00000001
#define PL_INTR_MASK            0x00000002

#define DATA_DIRECTION_MASK     0x00000002

volatile u32 pl_bram_flag;


XGpio Gpio;  /* The Instance of the AXI GPIO Driver */

void GpioplIntrHandler(void *Callback);
void Gpiopl_Setup_Intr_System(XScuGic *GicInstancePtr, XGpio *InstancePtr, u16 IntrId);
int Gpiopl_init(XGpio *InstancePtr, u32 DeviceId);

#endif /* GPIOPL_INTR_H_ */
