  
#define   Booting_Pin     3   
#define   Origin_Pin     4   
#define   En_Pin     5   
#define   Stp_Pin    6  
#define   Dir_Pin    7   
int Readied,Runing,ORIGIN,BOOTING;
unsigned int temp1=0,temp2=0, HV_TIME1=0,LV_TIME1=0,HV_TIME2=0,LV_TIME2=0,Y1=0,Y2=0;
                      
void setup() {

  
  // put your setup code here, to run once:

  pinMode(Booting_Pin , INPUT_PULLUP);
  pinMode(Origin_Pin , INPUT_PULLUP);
  pinMode(En_Pin , OUTPUT);       digitalWrite(En_Pin , LOW);       // initialize the En_Pin  as an output
  pinMode(Stp_Pin, OUTPUT);       digitalWrite(Stp_Pin, LOW);       // initialize the Stp_Pin as an output
  pinMode(Dir_Pin, OUTPUT);       digitalWrite(Dir_Pin, LOW);       // initialize the Dir_Pin as an output
  pinMode(LED_BUILTIN, OUTPUT);   digitalWrite(LED_BUILTIN  , LOW); // initialize the Led_Pin as an output

 
  delay(2000);

}
 
void loop() {
  // put your main code here, to run repeatedly:

  int i = 0,j=0;


 while((digitalRead(Booting_Pin)==0) && (digitalRead(Origin_Pin)==1) &&( Readied==0))
   {
    HOME();
   }
    if((digitalRead(Booting_Pin)==0) && ( Readied==1))//
    {
       /***********************************************************************************/
                if(temp1>=10)
                temp1=0;
                temp1++;
                switch(temp1)
                 {
                   case 1 :
                    {
                       HV_TIME1=23;
                       LV_TIME1=23;
                       Y1=8;
                    }break;

                    case 2 :
                    {
                        HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=10;
                    }break;
                    case 3 :
                    {
                       HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=15;
                    }break;
                    case 4 :
                    {
                       HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=7;
                    }break;
                    case 5 :
                    {
                       HV_TIME1=333;
                       LV_TIME1=333;
                       Y1=1;
                    }break;
                    case 6 :
                    {
                       HV_TIME1=333;
                       LV_TIME1=333;
                       Y1=1;
                    }break;
                    case 7 :
                    {
                      HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=5;
                    }break;
                    case 8 :
                    {
                        HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=15;
                    }break;
                    case 9 :
                    {
                      HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=10;
                    }break;
                    case 10 :
                    {
                       HV_TIME1=13;
                       LV_TIME1=13;
                       Y1=7;
                    }break;

                    default :break;

                    }
       /***********************************************************************************/
      digitalWrite(En_Pin , LOW); 
      digitalWrite(Dir_Pin, LOW);         
      digitalWrite(LED_BUILTIN, HIGH);    
  for(j=0; j < Y1; j++)             
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
  for(i=0; i < 3200; i++)             
  {
      digitalWrite(Stp_Pin, HIGH);      
      delayMicroseconds(HV_TIME1);           
      digitalWrite(Stp_Pin, LOW);       
      delayMicroseconds(LV_TIME1);           
  }
  }
    }
  
      delay(200);
    
    
 if((digitalRead(Booting_Pin)==0) && ( Readied==1))
    {
       /***********************************************************************************/
               if(temp2>=10)
                temp2=0;
                temp2++;
                switch(temp2)
                 {
                   case 1 :
                    {
                      HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=8;
                    }break;

                    case 2 :
                    {
                         HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=10;
                    }break;
                    case 3 :
                    {
                       HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=15;
                    }break;
                    case 4 :
                    {
                       HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=7;
                    }break;
                    case 5 :
                    {
                        HV_TIME2=333;
                       LV_TIME2=333;
                      Y2=1;
                    }break;
                    case 6 :
                    {
                       HV_TIME2=333;
                       LV_TIME2=333;
                      Y2=1;
                    }break;
                    case 7 :
                    {
                       HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=5;
                    }break;
                    case 8 :
                    {
                         HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=15;
                    }break;
                    case 9 :
                    {
                        HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=10;
                    }break;
                    case 10 :
                    {
                        HV_TIME2=13;
                       LV_TIME2=13;
                      Y2=7;
                    } break;

                    default : break;

                    }
       /***********************************************************************************/
      digitalWrite(Dir_Pin, HIGH);        
      digitalWrite(LED_BUILTIN, LOW);    
   for(j=0; j < Y2; j++)    
   {
     if(  digitalRead(Booting_Pin)==1)
    {
       digitalWrite(En_Pin , HIGH); 
       Readied=0;
       temp2=0;
       temp1=0;
       break;
    }
  for(i=0; i < 3200; i++)             
    {
        digitalWrite(Stp_Pin, HIGH);      
        delayMicroseconds(HV_TIME2);           
        digitalWrite(Stp_Pin, LOW);       
        delayMicroseconds(LV_TIME2);           
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
            
  
  
  digitalWrite(Dir_Pin, HIGH);        
  digitalWrite(LED_BUILTIN, LOW);     
  for(i=0; i < 3200; i++)             
  {
    
    digitalWrite(Stp_Pin, HIGH);      
    delayMicroseconds(100);          
    digitalWrite(Stp_Pin, LOW);       
    delayMicroseconds(100);           
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
    digitalWrite(Dir_Pin, LOW);         
    digitalWrite(LED_BUILTIN, HIGH);    
    for(j=0; j <8; j++)             
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
  for(i=0; i < 3200; i++)             
  {
    
    digitalWrite(Stp_Pin, HIGH);      
    delayMicroseconds(100);           
    digitalWrite(Stp_Pin, LOW);       
    delayMicroseconds(100);          
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
