#include "header.h"
#include<string.h>
#include<lpc21xx.h>
#include<stdio.h>

char arr[30],flag=0;
int main()
{
	unsigned char date,month,year;
 	char arr1[16],s,m,h,h1;
	lcd_init();
	uart0_init(9600);
	i2c_init();
	config_vic_uart();
	en_uart0_intr();
	i2c_byte_write_frame(0xD0,0x4,0x16);
	i2c_byte_write_frame(0xD0,0x5,0x07);
	i2c_byte_write_frame(0xD0,0x6,0x24);
	i2c_byte_write_frame(0xD0,0x2,0x71);
	i2c_byte_write_frame(0xD0,0x1,0x59);
	i2c_byte_write_frame(0xD0,0x0,0x48);
	while(1){
		date=i2c_byte_read_frame(0xD0,0x4);
		month=i2c_byte_read_frame(0xD0,0x5);
		year=i2c_byte_read_frame(0xD0,0x6);
		h1=i2c_byte_read_frame(0xD0,0x2);
		m=i2c_byte_read_frame(0xD0,0x1);
		s=i2c_byte_read_frame(0xD0,0x0);
		if(flag)
		  {
		  flag=0;
		 //h=0x1f&h1;
		arr1[0]=((h1&0x1f)/0x10)+48;
		arr1[1]=(h1%0x10)+48;
		arr1[2]=':';
		arr1[3]=(m/0x10)+48;
		arr1[4]=(m%0x10)+48;
		arr1[5]=' ';
		if((h1>>5&1)){
		arr1[6]='P';
		arr1[7]='M';}
		else{
		arr1[6]='A';
		arr1[7]='M'; }
		arr1[8]=' ';
		arr1[9]=(date/0x10)+48;
		arr1[10]=(date%0x10)+48;
		arr1[11]='/';
		arr1[12]=(month/0x10)+48;
		arr1[13]=(month%0x10)+48;
		arr1[14]='/';
		arr1[15]=(year/0x10)+48;
		arr1[16]=(year%0x10)+48;
		arr1[17]='\0';
		strcat(arr,arr1);
		uart0_tx_string(arr);
		uart0_tx_string("\r\n");
		}
lcd_cmd(0x80);
lcd_data(((h1&0x1f)/0x10)+48);
lcd_data((h1%0x10)+48);
lcd_data(':');
lcd_data((m/0x10)+48);
lcd_data((m%0x10)+48);
if((h1>>5)&1)
lcd_string("PM");
else
lcd_string("AM");

lcd_data((date/0x10)+48);
lcd_data((date%0x10)+48);
lcd_data('/');
lcd_data((month/0x10)+48);
lcd_data((month%0x10)+48);
lcd_data('/');
lcd_data((year/0x10)+48);
lcd_data((year%0x10)+48);
//}
//} 
}
} 
