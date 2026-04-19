#include<lpc21xx.h>
#include<stdio.h>
#include<string.h>
#include "header.h"
void uart0_init(unsigned int baud)
{
	unsigned int result=0;
	unsigned int a[]={15,60,30,15,15};
	unsigned int pclk;
	pclk=a[VPBDIV]*1000000;
	
	result=pclk/(16*baud);
	PINSEL0|=0x5;
	U0LCR=0x83;
	U0DLL=result&0xFF;
	U0DLM=(result>>8)&0xFF;
	U0LCR=0x03;
}
 
#define THRE ((U0LSR>>5)&1)
void uart0_tx(unsigned char data)
{
	U0THR=data;
 	while(THRE==0);
}	

#define RDR (U0LSR&1)
unsigned char uart0_rx(void)
{
while (RDR==0);
return U0RBR;
} 

void uart0_tx_string(char *ptr)																																															
{
while(*ptr)
{
U0THR=*ptr;
while(THRE==0);
ptr++;
}
} 

void uart0_rx_string(char *ptr,char len)
{
int i;
for(i=0;i<len;i++)
{
while(RDR==0);
ptr[i]=U0RBR;
if(ptr[i]=='\r')
break;
}
ptr[i++]=' ';
ptr[i]='\0';

}

void uart0_tx_float(float num)
{
char arr[10];
sprintf(arr,"%1f",num);
uart0_tx_string(arr);
}
 
void uart0_tx_integer(int num)
{
int a[10],i=0;
if(num==0)
uart0_tx('0');
if(num<0)
uart0_tx('-');
while(num>0)
{
a[i++]=num%10;
num=num/10;
}
for(--i;i>=0;i--)
uart0_tx(a[i]+48);
}  
          