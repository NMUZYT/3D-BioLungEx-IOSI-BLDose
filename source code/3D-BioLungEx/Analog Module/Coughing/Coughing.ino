  
#define   Booting_Pin     3   // the number of the Booting_Pin启动按钮
#define   Origin_Pin     4   // the number of the origin_Pin限位开关
#define   En_Pin     5   // the number of the En_pin
#define   Stp_Pin    6   // the number of the Stp_pin
#define   Dir_Pin    7   // the number of the Dir_pin
int Readied,Runing,ORIGIN,BOOTING;
unsigned int temp1=0,temp2=0, HV_TIME1=0,LV_TIME1=0,HV_TIME2=0,LV_TIME2=0,Y1=0,Y2=0;
                      
void setup() {

  
  // put your setup code here, to run once:
  // 初始化引脚
  pinMode(Booting_Pin , INPUT_PULLUP);
  pinMode(Origin_Pin , INPUT_PULLUP);
  pinMode(En_Pin , OUTPUT);       digitalWrite(En_Pin , LOW);       // initialize the En_Pin  as an output
  pinMode(Stp_Pin, OUTPUT);       digitalWrite(Stp_Pin, LOW);       // initialize the Stp_Pin as an output
  pinMode(Dir_Pin, OUTPUT);       digitalWrite(Dir_Pin, LOW);       // initialize the Dir_Pin as an output
  pinMode(LED_BUILTIN, OUTPUT);   digitalWrite(LED_BUILTIN  , LOW); // initialize the Led_Pin as an output

  // 上电延时2秒等待Emm_V5.0闭环初始化完毕
  delay(2000);

}
 
void loop() {
  // put your main code here, to run repeatedly:

  int i = 0,j=0;
//我们用的步进电机为57步进电机，步进电机通常接收一个脉冲，转动一个步距角，步距角可以根据细分进行配置
//比如未细分状态下一个步距角是1.8度，200个脉冲一圈。只需要200个脉冲电机就走完一圈，电机的精度很差，电机驱动电路板可以控制一个脉冲走的角度更少，比如设置8细分，那么步距角将会变成1.8/8 = 0.225度，那么将会需要200 * 8 = 1600个脉冲才能转动一圈。这里我们电机驱动里面默认设置了16细分。
  // 发送3200个脉冲，控制电机正转一圈。
  //一个脉冲：一个高电平加一个低电平
  // 脉冲频率（电机转速）通过延时函数delayMicroseconds()改变
  // 改小delayMicroseconds()的值转速变快，改大转速变慢，不能改的太小太小力矩不够，导致丢步（也就是没有走到一圈）

 while((digitalRead(Booting_Pin)==0) && (digitalRead(Origin_Pin)==1) &&( Readied==0))//在一直回原点，总开关断开或者限位开关闭合也还是一直转
   {
    HOME();
   }
    if((digitalRead(Booting_Pin)==0) && ( Readied==1))
    {
       /***********************************************************************************/
                if(temp1>=10)
                temp1=0;
                temp1++;
                switch(temp1)//每段的速度在这里设置
                 {
                   case 1 :
                    {
                       HV_TIME1=26;
                       LV_TIME1=26;
                       Y1=5;
                    }break;

                    case 2 :
                    {
                       HV_TIME1=26;
                       LV_TIME1=26;
                      Y1=7;
                    }break;
                    case 3 :
                    {
                       HV_TIME1=26;
                       LV_TIME1=26;
                      Y1=7;
                    }break;
                    case 4 :
                    {
                       HV_TIME1=26;
                       LV_TIME1=26;
                      Y1=15;
                    }break;
                    case 5 :
                    {
                      HV_TIME1=26;
                       LV_TIME1=26;
                      Y1=10;
                    }break;
                    case 6 :
                    {
                      HV_TIME1=26;
                       LV_TIME1=26;
                      Y1=7;
                    }break;
                    case 7 :
                    {
                      HV_TIME1=26;
                       LV_TIME1=26;
                      Y1=11;
                    }break;
                    case 8 :
                    {
                       HV_TIME1=2;
                       LV_TIME1=2;
                      Y1=6;
                    }break;
                    case 9 :
                    {
                      HV_TIME1=26;
                      LV_TIME1=26;
                      Y1=5;
                    }break;
                    case 10 :
                    {
                      HV_TIME1=26;
                      LV_TIME1=26;
                      Y1=8;
                    }break;

                    default :break;

                    }
       /***********************************************************************************/
      digitalWrite(En_Pin , LOW); 
      digitalWrite(Dir_Pin, LOW);         //方向信号输出低电平，电机正转
      digitalWrite(LED_BUILTIN, HIGH);    //点亮LED灯 
  for(j=0; j < Y1; j++)             //设置正转电机走多少圈
  {
    if(  digitalRead(Booting_Pin)==1)
    {
      digitalWrite(En_Pin , HIGH); 
      digitalWrite(LED_BUILTIN,LOW);
      Readied=0;
       temp2=0;
       temp1=0;
      break;
    }
  for(i=0; i < 3200; i++)             //输出3200个脉冲信号，最大只能输入32768
  {
      digitalWrite(Stp_Pin, HIGH);      //脉冲信号输出高电平 
      delayMicroseconds(HV_TIME1);           //延时100微秒，高电平延时100微秒
      digitalWrite(Stp_Pin, LOW);       //脉冲信号输出低电平 
      delayMicroseconds(LV_TIME1);           //延时100微秒，低电平延时100微秒
  }
  }
    }
    // 延时2秒电机反转
      delay(200);
    
      // 发送3200个脉冲，控制电机反转一圈
      // 脉冲频率（电机转速）通过延时函数delayMicroseconds()改变
      // 改小delayMicroseconds()的值转速变快，改大转速变慢
 if((digitalRead(Booting_Pin)==0) && ( Readied==1))
    {
       /***********************************************************************************/
               if(temp2>=10)
                temp2=0;
                temp2++;
                switch(temp2)//每段的速度在这里设置
                 {
                   case 1 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=5;
                    }break;

                    case 2 :
                    {
                       HV_TIME2=26;
                       LV_TIME2=26;
                       Y2=7;
                    }break;
                    case 3 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=7;
                    }break;
                    case 4 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=15;
                    }break;
                    case 5 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=3;
                    }break;
                    case 6 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=14;
                    }break;
                    case 7 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=2;
                    }break;
                    case 8 :
                    {
                      HV_TIME2=2;
                      LV_TIME2=2;
                      Y2=15;
                    }break;
                    case 9 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=3;
                    }break;
                    case 10 :
                    {
                      HV_TIME2=26;
                      LV_TIME2=26;
                      Y2=8;
                    } break;

                    default : break;

                    }
       /***********************************************************************************/
      digitalWrite(Dir_Pin, HIGH);        //方向信号输出高电平，电机反转
      digitalWrite(LED_BUILTIN, LOW);     //熄灭LED灯 
   for(j=0; j < Y2; j++)    //设置正转电机走多少圈
   {
     if(  digitalRead(Booting_Pin)==1)
    {
       digitalWrite(En_Pin , HIGH); 
       Readied=0;
       temp2=0;
       temp1=0;
       break;
    }
  for(i=0; i < 3200; i++)             //输出3200个脉冲信号
    {
        digitalWrite(Stp_Pin, HIGH);      //脉冲信号输出高电平 
        delayMicroseconds(HV_TIME2);           //延时100微秒，高电平延时100微秒
        digitalWrite(Stp_Pin, LOW);       //脉冲信号输出低电平 
        delayMicroseconds(LV_TIME2);           //延时100微秒，低电平延时100微秒
     }
   }  
 
   }
     if(  digitalRead(Booting_Pin)==1)
    {
        digitalWrite(En_Pin , HIGH); 
         Readied=0;
    }
  delay(200);/////////
}
void HOME()
{
  int i = 0,j=0;
  BOOTING=digitalRead(Booting_Pin);
  ORIGIN=digitalRead(Origin_Pin);
   Readied=0;
  if(BOOTING==0)
  {
    delay(500);
      if(BOOTING==0)
 {
    digitalWrite(En_Pin , LOW); 
   while(digitalRead(Origin_Pin)==1)
   {
    delay(20);
 if(digitalRead(Origin_Pin)==1)
  {
            // 发送3200个脉冲，控制电机反转一圈
  // 脉冲频率（电机转速）通过延时函数delayMicroseconds()改变
  // 改小delayMicroseconds()的值转速变快，改大转速变慢
  digitalWrite(Dir_Pin, HIGH);        //方向信号输出高电平，电机反转
  digitalWrite(LED_BUILTIN, LOW);     //熄灭LED灯 
  for(i=0; i < 3200; i++)             //输出3200个脉冲信号
  {
    
    digitalWrite(Stp_Pin, HIGH);      //脉冲信号输出高电平 
    delayMicroseconds(100);           //延时100微秒
    digitalWrite(Stp_Pin, LOW);       //脉冲信号输出低电平 
    delayMicroseconds(100);           //延时100微秒
   }
  
  BOOTING=digitalRead(Booting_Pin);
     ORIGIN=digitalRead(Origin_Pin);
     if(  digitalRead(Booting_Pin)==1)
    {
      digitalWrite(En_Pin , HIGH); 
       Readied=0;
       temp2=0;
       temp1=0;
      break;
    }
      if(ORIGIN==0)
   {
    delay(1000);
  if(ORIGIN==0)
    {
      
    Readied=1;
     digitalWrite(En_Pin , LOW); 
    digitalWrite(Dir_Pin, LOW);         //方向信号输出低电平，电机正转
    digitalWrite(LED_BUILTIN, HIGH);    //点亮LED灯
    for(j=0; j < 8; j++)             //往回拉后，设置正转电机走8圈
  {
    if(  digitalRead(Booting_Pin)==1)
    {
      digitalWrite(En_Pin , HIGH); 
      digitalWrite(LED_BUILTIN,LOW);
      Readied=0;
       temp2=0;
       temp1=0;
      break;
    }
  for(i=0; i < 3200; i++)             //输出3200个脉冲信号，最大只能输入32768
  {
    
    digitalWrite(Stp_Pin, HIGH);      //脉冲信号输出高电平 
    delayMicroseconds(100);           //延时100微秒，高电平延时100微秒
    digitalWrite(Stp_Pin, LOW);       //脉冲信号输出低电平 
    delayMicroseconds(100);           //延时100微秒，低电平延时100微秒
  }
  }
  delay(1000);
    break;
    }
   }
  }
  }
 
 }
 }
 return 0;
}
