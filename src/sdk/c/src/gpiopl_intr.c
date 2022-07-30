/*
 * gpiopl_intr.c
 *
 *  Created on: 2017Äê4ÔÂ24ÈÕ
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
 */

#include "gpiopl_intr.h"

void GpioplIntrHandler(void *Callback)
{
	XGpio *GpioPtr = (XGpio *)Callback;
	u32 IrqStatus;
	u32 gpio;

	IrqStatus = XGpio_InterruptGetStatus(GpioPtr);

	if((IrqStatus & GPIO_INTR_MASK) == GPIO_INTR_MASK)
	{
		gpio = XGpio_DiscreteRead(GpioPtr, 1);
		pl_bram_flag = 1;
	}

//	gpio_isr_cnt++;

	XGpio_InterruptClear(GpioPtr, IrqStatus);

}


void Gpiopl_Setup_Intr_System(XScuGic *GicInstancePtr, XGpio *InstancePtr, u16 IntrId)
{
        XScuGic_Connect(GicInstancePtr, IntrId,
                        (Xil_ExceptionHandler)GpioplIntrHandler,//set up the timer interrupt
                        (void *)InstancePtr);

        XScuGic_Enable(GicInstancePtr, IntrId);//enable the interrupt for the Timer at GIC

    	XGpio_InterruptEnable(InstancePtr, GPIO_INTR_MASK);
    	XGpio_InterruptGlobalEnable(InstancePtr);

 }

int Gpiopl_init(XGpio *InstancePtr, u32 DeviceId)
{

	int Status;
	/* Initialize AXI GPIO */
	Status = XGpio_Initialize(InstancePtr, DeviceId);
	if (Status != XST_SUCCESS) {
		xil_printf("AXI GPIO config failed!\r\n");
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(InstancePtr, 1, DATA_DIRECTION_MASK);

	XGpio_DiscreteClear(InstancePtr, 1, 0x00000001);

	pl_bram_flag = 0;

    return 1;
}
