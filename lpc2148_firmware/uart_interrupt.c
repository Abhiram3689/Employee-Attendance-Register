#include<lpc21xx.h>
#include "header.h"
extern char arr[30],flag;
void UART0_Handler(void)__irq{
	int num,i;
	num=U0IIR&0x0E;
	if(num==0x4){
	for(i=0;i<12;i++)
	{
		while((U0LSR&1)==0);
		arr[i]=U0RBR;
		if(arr[i]=='\r')
		break;
	}
	arr[i++]=' ';
	arr[i]='\0';
	flag=1;
	}
   VICVectAddr=0;
}

void config_vic_uart(void){
 	VICIntSelect=0;
	VICVectCntl1=6|(1<<5);
	VICVectAddr1=(int)UART0_Handler;
	VICIntEnable|=(1<<6);
}
void en_uart0_intr(void){
 	U0IER=3;
}
