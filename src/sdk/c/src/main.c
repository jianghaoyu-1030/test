
/*
 *
 * www.osrc.cn
 * copyright by liyang mi lian dian zi www.osrc.cn
 * axi dma test
 *
*/


#include "dma_intr.h"
#include "timer_intr.h"
#include "sys_intr.h"
#include "xgpio.h"
#include "udp_transmission.h"
#include "gpiopl_intr.h"

static XScuGic Intc; //GIC
static  XAxiDma AxiDma;
static  XScuTimer Timer;//timer

volatile u32 RX_success;
volatile u32 TX_success;

volatile u32 RX_ready=1;
volatile u32 TX_ready=1;


#define ddr_baseaddr  0x10000000  //ddr 基地址


u8 *TxBufferPtr= (u8 *)TX_BUFFER_BASE;
u8 *RxBufferPtr=(u8 *)RX_BUFFER_BASE;


//static XGpio Gpio;
#define AXI_GPIO_DEV_ID	        XPAR_AXI_GPIO_0_DEVICE_ID

/////////////////////////////////////////////////////////////////////////////
struct netif *netif, server_netif;
struct ip_addr ipaddr, netmask, gw;
unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };
///////////////////////////////////////////////////////////////////////////////
u8 test_data[2048];
u8 data_ready=0;
char bram_rec_done;

int axi_dma_test()
{
	int Status;
	char speed_r;
	char speed_t;
	TxDone = 0;
	RxDone = 0;
	Error = 0;

	xil_printf( "----DMA Test----\r\n");

	xil_printf("PKT_LEN=%d\r\n",MAX_PKT_LEN);

	/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((u32)TxBufferPtr, MAX_PKT_LEN);
	// Timer_start(&Timer);
	while(1)
	//for(i = 0; i < Tries; i ++)
	{
		xemacif_input(netif);
	    if(bram_rec_done)
	    {
			XGpio_DiscreteWrite(&Gpio, 2, ~PS_BRAM_MASK);
			usleep(1);
			XGpio_DiscreteWrite(&Gpio, 2, PS_BRAM_MASK);
			bram_rec_done = 0;
	    }
		//RX DMA Transfer
		if(RX_ready)
		{
		   RX_ready=0;
		   Status = XAxiDma_SimpleTransfer(&AxiDma,(u32)RxBufferPtr,
					(u32)(MAX_PKT_LEN), XAXIDMA_DEVICE_TO_DMA);

		   if (Status != XST_SUCCESS) {return XST_FAILURE;}
		}

		//TX DMA Transfer
		// if(TX_ready)
		// {
		// 	TX_ready=0;
		// 	Status = XAxiDma_SimpleTransfer(&AxiDma,(u32) TxBufferPtr,
		// 			(u32)(MAX_PKT_LEN), XAXIDMA_DMA_TO_DEVICE);

		// 	if (Status != XST_SUCCESS) {return XST_FAILURE;}
		// }

		if(RxDone)
		{
			//确保cache的数据都进入DDR
			Xil_DCacheInvalidateRange((u32)RxBufferPtr, MAX_PKT_LEN);
			// for(i = 0; i<MAX_PKT_LEN; i++)
			// {
			// 	Xil_Out32(ddr_baseaddr  + (i*4) , *(RxBufferPtr+ (i*4)));
			// }
			data_ready=1;
			memcpy(test_data,RxBufferPtr,2048);
			RxDone=0;
			RX_ready=1;
			// RX_success++;
		}

		if(data_ready)
		{
			data_ready=0;

			
			// udp_printf(test_data,1024);
			// xemacif_input(netif);
			// usleep(10);
			// udp_printf(&test_data[1024],1024);
			// xemacif_input(netif);
			// usleep(10);
		}

		// if(TxDone)
		// {
		// 	TxDone=0;
		// 	TX_ready=1;
		// 	TX_success++;
		// }
		if (Error) {
			xil_printf("Failed test transmit%s done, "
			"receive%s done\r\n", TxDone? "":" not",
							RxDone? "":" not");
			goto Done;
		}

	}

	/* Disable TX and RX Ring interrupts and return success */
	DMA_DisableIntrSystem(&Intc, TX_INTR_ID, RX_INTR_ID);
Done:
	xil_printf("--- Exiting Test --- \r\n");

	return XST_SUCCESS;

}

int init_intr_sys(void)
{
	Gpiopl_init(&Gpio, AXI_GPIO_DEV_ID);
	Init_Intr_System(&Intc); // initial DMA interrupt system
	Gpiopl_Setup_Intr_System(&Intc, &Gpio, GPIO_INTR_ID);
	Setup_Intr_Exception(&Intc);
	DMA_Intr_Init(&AxiDma,0);//initial interrupt system
	DMA_Setup_Intr_System(&Intc,&AxiDma,TX_INTR_ID,RX_INTR_ID);//setup dma interrpt system
	DMA_Intr_Enable(&Intc,&AxiDma);
	//XGpio_Initialize(&Gpio, AXI_GPIO_DEV_ID);

}

int main(void)
{
	init_intr_sys();
	netif = &server_netif;
	IP4_ADDR(&ipaddr,  192, 168,   1,  10);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      192, 168,   1,  1);
	/*lwip library init*/
	lwip_init();
	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, XPAR_XEMACPS_0_BASEADDR)) {
		xil_printf("Error adding N/W interface\r\n");
		return -1;
	}
	netif_set_default(netif);
	/* specify that the network if is up */
	netif_set_up(netif);
	/* initialize tcp pcb */
	udp_recv_init();

	XGpio_SetDataDirection(&Gpio, 1, 0);
	XGpio_DiscreteWrite(&Gpio, 1, 1);
	// while(1)
	// {
	// 	xemacif_input(netif);
	// 	//udp_printf(mac_ethernet_address,6);
	// 	usleep(1);
	// }
	axi_dma_test();


}


