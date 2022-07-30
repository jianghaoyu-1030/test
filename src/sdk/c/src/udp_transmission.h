/*
 * udp_transmission.h
 *
 * Created on: 
 * www.osrc.cn
 * www.milinker.com
 * copyright by nan jin mi lian dian zi www.osrc.cn
*/

#ifndef UDP_TRANSMISSION_H_
#define UDP_TRANSMISSION_H_

#include "lwip/err.h"
#include "lwip/udp.h"
#include "lwipopts.h"

// #define ddr_pack_num   100     
// #define  udp_ptk_len  1409   
//u8 bram_rec_done=0;

void udp_recv_callback(void *arg, struct udp_pcb *tpcb,
                               struct pbuf *p, struct ip_addr *addr, u16_t port);
int udp_recv_init(void);
void udp_printf(const char8 *ctrl1,unsigned int len);


////////////////////Ð£ÑéºÍ//////////////////
uint8_t Send_CheckSum(uint8_t *str, int str_length);
uint8_t Recv_CheckSum(uint8_t *str, int str_length);






#endif /* UDP_TRANSMISSION_H_ */
