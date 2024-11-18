/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-28
 * @brief       SD�� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "sys.h"
#include "usart.h"/* Declarations of FatFs API */
#include "./SYSTEM/delay/delay.h"
#include "./USMART/usmart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./SYSTEM/time/bsp_tim.h"
#include "ff.h"		

#define FF_MAX_SS1		512



uint16_t Amplitude,min_num,Highest;
uint16_t Instal_distance=230;
uint16_t temp[1000]={0};
uint16_t xemp[1000]={0};
UART_HandleTypeDef g_uart3_handle;
extern uint16_t ms,t_flag;
extern  char mode,Z ,T;
extern  char node ;
extern  TIM_HandleTypeDef g_tim6_handle;  /* ��ʱ����� */
extern  TIM_HandleTypeDef g_tim8_handle;  /* ��ʱ����� */
uint16_t receive_cnt;//???????????
uint8_t confidence=0;
uint16_t distance=0,noise,reftof,Highest,BR,PTP0,knum1;
uint32_t  peak,intg,msp1=0;   
uint8_t dis;

FRESULT ret;
FATFS *fs_obj;
FIL *fil_obj;
uint8_t rd_buf[255] = {0};
uint16_t fsize = 0;
uint16_t rd_conut, wd_conut;

extern LidarPointTypedef Pack_sum;     /* ?????? */
void display(void)
{
	uint16_t t;

	lcd_fill(0,0,800,340,WHITE);
	g_point_color=GREEN;
	for(t  = 0;t<360;t=t+20)
	lcd_draw_line(0,t,800,t,BLUE);
	for(t  = 0;t<800;t=t+20)
	lcd_draw_line(t,0, t,340,BLUE);
	g_point_color=BLUE;
	lcd_draw_line(0, 180, 800, 180,BLUE);
	lcd_draw_line(400,0, 400,340,BLUE);
	lcd_show_num(150,400,Amplitude,3,24,BLUE);
}


void sd_test_fatfs(void)
{
    
	
    
    fs_obj = mymalloc(SRAMIN, sizeof(FATFS));
    fil_obj = mymalloc(SRAMIN, sizeof(FIL));


    /* 1 ����SD���豸 */
    ret = f_mount(fs_obj, "0:", 1); 
	f_unlink ("0:sd_test.txt");	
    
    /* 2 ���ļ�(��Ҫע��Ҫ�ر��ļ�f_close) */
    ret = f_open(fil_obj, "0:sd_test.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
    if (ret)
    {
        printf("open fail %d \r\n", ret);
    }
    else
    {
		
        printf("open OK\r\n");
    }
    
    /* 3 ��ȡ���� */
    fsize = f_size(fil_obj);
    f_read(fil_obj, rd_buf, fsize, (UINT *)&rd_conut);
    if (rd_conut != 0)
    {
        printf("rd_conut:%d rd_buf:%s \r\n",rd_conut, rd_buf);
    }
    
    /* 4 ����д�� */            /* ע�⣺��дָ�� */
    f_write(fil_obj, "xin", 3, (UINT *)&wd_conut);
	f_printf(fil_obj, " \r ");
    f_printf(fil_obj, "hello_world");
    
    f_lseek(fil_obj, 0);
    fsize = f_size(fil_obj);
    f_read(fil_obj, rd_buf, fsize, (UINT *)&rd_conut);
    if (rd_conut != 0)
    {
        printf("rd_conut:%d rd_buf:%s \r\n",rd_conut, rd_buf);
    }
    delay_ms(10);
    /* 5 �ر��ļ� */
    f_close(fil_obj);
	
}

uint8_t mf_scan_files(uint8_t * path)
{
    FRESULT res;    
    DIR *dir;
    FILINFO *fileinfo;
    
    dir = mymalloc(SRAMIN, sizeof(DIR));
    fileinfo = mymalloc(SRAMIN, sizeof(FILINFO));

    res = f_opendir(dir, (const TCHAR*)path); //��һ��Ŀ¼
    
    if (res == FR_OK) 
    {
        printf("\r\n"); 
        
        while(1)
        {
            res = f_readdir(dir, fileinfo);   /* ��ȡĿ¼�µ�һ���ļ� */
            if (res != FR_OK || fileinfo->fname[0] == 0) break; /* ������/��ĩβ��,�˳� */
            //if (fileinfo.fname[0] == '.') continue;           /* �����ϼ�Ŀ¼ */
            printf("%s/", path);                /* ��ӡ·�� */
            printf("%s\r\n",fileinfo->fname);   /* ��ӡ�ļ��� */
        } 
    }
    f_closedir(dir);
    
    return res;
}


int main(void)
{
    uint8_t buf[20] = {0};
	uint16_t tempNum1;
	uint8_t name[3]={"0:"};
	char str_buffer[100];
	
	
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
	usmart_dev.init(72); 					/* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    sram_init();                            /* SRAM��ʼ�� */
    my_mem_init(SRAMIN);                    /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                    /* ��ʼ���ⲿSRAM�ڴ�� */
    btim_timx_int_init(50- 1, 7200 - 1); /* 10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms */
    sd_test_fatfs();
    mf_scan_files(name);
    
    f_setlabel((const TCHAR *)"0:SD_card");
    f_getlabel((const TCHAR *)"0:", (TCHAR *)buf , 0);
    printf("label:%s \r\n", buf);
    
	lcd_display_dir(1);
	
	lcd_show_string(60,400,24*8,24,24,"highest:",BLUE);
	lcd_show_string(60,424,24*8,24,24," Actual:",BLUE);
	lcd_show_string(200,400,24*2,24,24,"mm",BLUE);
	lcd_show_string(200,424,24*2,24,24,"mm",BLUE);
	g_point_color=BLUE;
	HAL_TIM_Base_Start_IT(&g_tim6_handle);//��TIM6
	printf("���ڳ�ʼ��: %d\r\n",msp1);   /* ��ӡ�ļ��� */
    while (1)
    {
		printf("Z: %d\r\n",Z);   /* ��ӡ�ļ��� */
		printf("MS: %d\r\n",ms);   /* ��ӡ�ļ��� */
		if(Z==1)
			{
				f_open(fil_obj, "0:sd_test.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
				//TIM_ClearITPendingBit(TIM6, TIM_IT_Update);//��������жϱ�־
				__HAL_RCC_TIM8_CLK_DISABLE();
				
				Z = 0;
				printf("distance: %d\r\n",distance);   /* ��ӡ�ļ��� */
				tempNum1= Instal_distance-distance;
				temp[ms]=tempNum1+150;

				if(tempNum1>Highest) //record :Highest
						{
							Highest=tempNum1;
						}
				if(Highest<40)
						{
							if(Highest>28)
							{
							Amplitude=Highest;
							lcd_show_num(150,424,tempNum1 ,3,24,BLACK);
							}
							fsize = f_size(fil_obj);
							fsize=fsize+3;
							f_lseek(fil_obj, fsize);
							sprintf(str_buffer, "%d", tempNum1);
							f_write(fil_obj, "   ", 3, (UINT *)&wd_conut);
							    /* 4 ����д�� */            /* ע�⣺��дָ�� */
							f_write(fil_obj,str_buffer,strlen(str_buffer), (UINT *)&wd_conut);
						}
							__HAL_RCC_TIM8_CLK_ENABLE();
			}
		if(tempNum1<(Highest/20))
			{
				Highest=0;
			}
		if(ms == 799)
			{
				
				
				__HAL_RCC_TIM8_CLK_DISABLE();
				fsize = f_size(fil_obj);
				fsize=fsize+3;
				f_lseek(fil_obj, fsize);
				sprintf(str_buffer, "%d", tempNum1);
				f_write(fil_obj, "   ", 3, (UINT *)&wd_conut);
							    /* 4 ����д�� */            /* ע�⣺��дָ�� */
				f_write(fil_obj,str_buffer,strlen(str_buffer), (UINT *)&wd_conut);
				display ();
				for(ms=0;ms<800;ms++)xemp[ms]=temp[ms];
				g_point_color=BLACK;
				for(ms=0;ms<799;ms++)lcd_draw_line(ms, 480-xemp[ms]-150, ms+1, 480-xemp[ms+1]-150,BLACK);
				ms = 0;
				f_close(fil_obj);//????
				__HAL_RCC_TIM8_CLK_ENABLE();
				
			}
				
    }
}

