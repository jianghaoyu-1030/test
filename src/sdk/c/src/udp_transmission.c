/*
 * udp_transmission.c
 *
 * Created on: 
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
*/


#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/udp.h"
#include "lwipopts.h"
#include "netif/xadapter.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "sleep.h"
#include "udp_transmission.h"

#include "stdlib.h"
#include "math.h"

// #define ddr_baseaddr  0x10000000

// #define FILE_BASE_ADDR		0x10000000
// #define READ_BASE_ADDR		0x11000000
// #define WRITE_BASE_ADDR		0x12000000
#define XPAR_BRAM_BASEADDR 0x40000000


struct udp_pcb *connected_pcb = NULL;
static struct pbuf *pbuf_to_be_sent = NULL;

static unsigned local_port = 1000;	/* server port */
static unsigned remote_port = 1000;
volatile u32 file_length = 0;


u8 *udp_recdata;
extern char udp_datarec_over_flag;
unsigned char data_cmd;
unsigned short data_len;
extern char bram_rec_done=0;

void udp_recv_callback(void *arg, struct udp_pcb *tpcb,
                               struct pbuf *p, struct ip_addr *addr, u16_t port)
{
	struct pbuf *q;
	q = p;
	udp_recdata = (char*)q->payload;
	int len = q->len;
	int i;
//////////////////////////////解协议///////////////////////////////////
	if(udp_recdata[0] == 0xFE && udp_recdata[1] == 0xEF)
	{
		memcpy(&data_len,&udp_recdata[2],2);
//		data_len = (data_len << 8 )| udp_recdata[3] ;
		data_cmd = udp_recdata[4] ;
//		data_cmd = (data_cmd << 8 )| udp_recdata[5] ;
//		 Process_udp_Cmd();
//	    udp_datarec_over_flag = 1;
		xil_printf("data len is %d\r\n",data_len);
		usleep(1);
		xil_printf("data cmd is %d\r\n",data_cmd);
		usleep(1);
		for(i = 0; i<data_len;i++)
		{
		Xil_Out32(XPAR_BRAM_BASEADDR  + 4*i, udp_recdata[i]);
		}
		bram_rec_done=1;
		pbuf_free(p);
//		udp_printf("666666",6);
	}
/////////////////////////////////////////////////////////////////
		pbuf_free(p);

    return;
}


int udp_recv_init()
{
	struct udp_pcb *pcb;
	struct ip_addr ipaddr;
	err_t err;

	/* create new UDP PCB structure */
	pcb = udp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\r\n");
		return -1;
	}

	/* bind to local port */
	err = udp_bind(pcb, IP_ADDR_ANY, local_port);
	if (err != ERR_OK) {
		xil_printf("udp_recv_init: Unable to bind to port %d: err = %d\r\n", local_port, err);
		return -2;
	}


	IP4_ADDR(&ipaddr,  192, 168,  1, 82);  
	err = udp_connect(pcb, &ipaddr, remote_port);
	if (err != ERR_OK)
		xil_printf("error on udp_connect: %x\n\r", err);

	udp_recv(pcb, udp_recv_callback, NULL);

	connected_pcb = pcb;

	return 0;
}

void udp_printf(const char8 *ctrl1,unsigned int len)
{
	char * message;
	u16 length;
	int i;
	err_t err;
	struct udp_pcb *tpcb = connected_pcb;

	if (!tpcb)
	{
		xil_printf("error return\r\n");
		return;
	}

	message = (char *)ctrl1;
	length = len;
	pbuf_to_be_sent = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL);
	memset(pbuf_to_be_sent->payload, 0, length);
	memcpy(pbuf_to_be_sent->payload, (u8 *)message, length);

	err = udp_send(tpcb, pbuf_to_be_sent);
	if (err != ERR_OK)
	{
		xil_printf("Error on udp_send: %d\r\n", err);
		pbuf_free(pbuf_to_be_sent);
		return;
	}
	pbuf_free(pbuf_to_be_sent);

}

///////////////////////////校验和////////////////////////
uint8_t Send_CheckSum(uint8_t *str, int str_length)
{
    uint8_t CheckSum_Value = 0;
    int i = 0;
    for(i = 0; i < str_length; i++)
    {
       CheckSum_Value += str[i];
    }
    CheckSum_Value = ~CheckSum_Value;
    return CheckSum_Value;
}

uint8_t Recv_CheckSum(uint8_t *str, int str_length)
{
    uint8_t CheckSum_Value = 0;
    int i = 0;
    for(i = 0; i < str_length; i++)
    {
       CheckSum_Value += str[i];
    }
    return CheckSum_Value;
}
/////////////////////////////////////////////////////////
