#include <stdio.h>
#include <wiringPi.h> 
#include <stdlib.h>  
#include <stdint.h>  
#include <string.h>

#define DHT11_MAX_TIME 40  
#define DHT11PIN 7 

#define LEDG1	 23
#define LEDY1	 24
#define LEDR1	 25	

#define LEDG2	 27
#define LEDY2	 28
#define LEDR2	 29

#define Slant	 26
#define HC_SR501 1		

int dht11_val[5]={0,0,0,0,0}; 


void init(void);
void LED(int number);
void Fan(int number);
void Bell();
void dht11_read_val(int *Humidity,int *Temperature);



int main(void)
{
    int Humidity    = 0;
    int Temperature = 0;
    int HC_SR501_Flag = 0;
    int Slant_Flag = 0;
    FILE *fp; 
    char buf[100] = "";
    
    init();

    while(1)
    {
      delay(3000);
      dht11_read_val(&Humidity,&Temperature);
      
      if(Temperature <= 25)
      {
        LED(1);
        Fan(0);
      }
      else if(Temperature <= 30)
      {
        LED(2);
        Fan(1);  
      }
      else if(Temperature > 30)
      {
        LED(3);
        Fan(1);  
      }
      
      if(digitalRead(HC_SR501) == HIGH)
      {
        printf("Someone is closing! \n");
        HC_SR501_Flag = 1;
        LED(5);
        if(digitalRead(Slant) == LOW)
        {
          printf("Something is slanting! \n"); 
          LED(7);
          Slant_Flag = 1;
          Bell();
        }
        else
        {
          Slant_Flag = 0;
        }
      }
      
      else
      {
        printf("Noanybody! \n");
        HC_SR501_Flag = 0;
        if(digitalRead(Slant) == LOW)
        {
          printf("Something is slanting! \n"); 
          LED(6);
          Slant_Flag = 1;
          Bell();
        }
        else
        {
          LED(4);
          Slant_Flag = 0;
        }  
      }
      printf("[%d,%d,%d,%d]\n",dht11_val[2],dht11_val[0],HC_SR501_Flag,Slant_Flag); 

      if((fp=fopen("data.txt","w+"))==NULL)//如果文件不存在，新建文件，写数据 
      { 
        //printf("Can not open file.\n"); 
        exit(0); 
      } 
      sprintf(buf, "%d,%d,%d,%d", Temperature, Humidity, HC_SR501_Flag, Slant_Flag);
      fwrite(buf,strlen(buf),1 ,fp);//试验数据，文件存在时尝试，文件不存在时尝试 
      fclose(fp);
    }
    

}

void init(void)
{
    wiringPiSetup() ; 
    pinMode(0 ,OUTPUT);
    pinMode(22,OUTPUT);
 
    pinMode(LEDG1,OUTPUT);
    pinMode(LEDY1,OUTPUT);
    pinMode(LEDR1,OUTPUT);
    
    pinMode(LEDG2,OUTPUT);
    pinMode(LEDY2,OUTPUT);
    pinMode(LEDR2,OUTPUT);
    
    pinMode(Slant,INPUT);
    pinMode(HC_SR501,INPUT);
    
    LED(1);
    LED(4);
    Fan(0);
    digitalWrite(0,HIGH);
}
void LED(int number)
{
    switch(number)
    {
        case 1: digitalWrite(LEDG1, HIGH);
                digitalWrite(LEDY1,  LOW);
                digitalWrite(LEDR1,  LOW);
                break;
        case 2: digitalWrite(LEDG1, LOW);
                digitalWrite(LEDY1, HIGH);
                digitalWrite(LEDR1, LOW);
                break;
        case 3: digitalWrite(LEDG1, LOW);
                digitalWrite(LEDY1, LOW);
                digitalWrite(LEDR1, HIGH);
                break;
                
        case 4: digitalWrite(LEDG2, HIGH);
                digitalWrite(LEDY2,  LOW);
                digitalWrite(LEDR2,  LOW);
                break;
        case 5: digitalWrite(LEDG2, LOW);
                digitalWrite(LEDY2, HIGH);
                digitalWrite(LEDR2, LOW);
                break;
        case 6: digitalWrite(LEDG2, LOW);
                digitalWrite(LEDY2, LOW);
                digitalWrite(LEDR2, HIGH);
                break;
        case 7: digitalWrite(LEDG2, LOW);
                digitalWrite(LEDY2, HIGH);
                digitalWrite(LEDR2, HIGH);
                break;
        default:break;
    }
}

void Fan(int number)
{
    switch(number)
    {
        case 0: digitalWrite(22, HIGH );
                break;
        case 1: digitalWrite(22, LOW);
                break;
        default:break;
    }
}

void Bell()
{
    int i;
    for(i = 0; i < 5; i++)
    {
        printf ("the Buzzer will make sound\n");
        digitalWrite(0,  LOW);
        delay (500) ;
        digitalWrite(0, HIGH);
        delay (500) ;    
    }
}

void dht11_read_val(int *Humidity,int *Temperature)  
{  
  int counter=0;  
  uint8_t i;   
  for(i = 0; i < 5; i++)  
     dht11_val[i]=0;  

  pinMode(DHT11PIN,OUTPUT);  
  digitalWrite(DHT11PIN,LOW);  
  delay(18);  
  digitalWrite(DHT11PIN,HIGH);  
  delayMicroseconds(40);  
  pinMode(DHT11PIN,INPUT);  
  while(digitalRead(DHT11PIN)==LOW){ 
    continue; 
  }  
  while(digitalRead(DHT11PIN)==HIGH){ 
    continue; 
  }
             
  for(i=0;i<DHT11_MAX_TIME;i++)  
  {  
    counter=0;  
    while(digitalRead(DHT11PIN)==LOW){
      continue;   
    }
    while(digitalRead(DHT11PIN)==HIGH){  
      counter++; 
      delayMicroseconds(1);
      if(counter > 100){
        break;  
      }   
    }
    if(counter < 35){
      dht11_val[i/8] = dht11_val[i/8] + ( 0 << (7-i%8));  
    }
    else{
      dht11_val[i/8] = dht11_val[i/8] + ( 1 << (7-i%8));
    } 
  }
  
  // verify cheksum and print the verified data 
  if(dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF)) 
  {  
    *Humidity    = dht11_val[0];
    *Temperature = dht11_val[2];
    printf("Humidity = %d.%d %% Temperature = %d.%d \n",dht11_val[0],dht11_val[1],dht11_val[2],dht11_val[3]);  
    //return 1;
  }  
  else  
  {
    printf("Invalid Data!!\n");
    //return 0;
  }
}
