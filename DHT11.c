#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#define MAX_TIME 85
#define MAX_TRIES 100
#define DHT11PIN 7

int dht11_val[5]={0,0,0,0,0};

int dht11_read_val(int *h, int *t) {
    uint8_t lststate=HIGH;
    uint8_t counter=0;
    uint8_t j=0,i;
    float   f; /* fahrenheit */ 
    
    for (i=0;i<5;i++) {
         dht11_val[i]=0;
    }

    pinMode(DHT11PIN,OUTPUT);
    digitalWrite(DHT11PIN,LOW);
    delay(18);
    digitalWrite(DHT11PIN,HIGH);
    delayMicroseconds(40);
    pinMode(DHT11PIN,INPUT);

    for (i=0;i<MAX_TIME;i++) {
        counter=0;
        while (digitalRead(DHT11PIN)==lststate){
            counter++;
            delayMicroseconds(1);
            if (counter==255)
                break;
        }
        lststate=digitalRead(DHT11PIN);
        if (counter==255)
             break;
        // top 3 transitions are ignored
        if ((i>=4) && (i%2==0)) {
            dht11_val[j/8]<<=1;
            if(counter>16)
                dht11_val[j/8]|=1;
            j++;
        }
    }

    // verify cheksum and print the verified data
    if ((j>=40) && (dht11_val[4]==((dht11_val[0]+dht11_val[1]+dht11_val[2]+dht11_val[3])& 0xFF))) {
        // Only return the integer part of humidity and temperature. The sensor
        // is not accurate enough for decimals anyway 
        *h = dht11_val[0];
        *t = dht11_val[2];
         f = dht11_val[2] * 9. / 5. + 32; 
        printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",  
            dht11_val[0], dht11_val[1], dht11_val[2], dht11_val[3], f ); 
        return 0;
    }
    else {
        // invalid data
        printf( "Data not good, skip\n" );
        f = dht11_val[2] * 9. / 5. + 32; 
        printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",  
            dht11_val[0], dht11_val[1], dht11_val[2], dht11_val[3], f ); 
        return 1;
    }
}

int main(int argc, char *argv[]) {
    int h; //humidity
    int t; //temperature in degrees Celsius
  
    
    // error out if wiringPi can't be used
    if (wiringPiSetup()==-1) {
        printf("Error interfacing with WiringPi\n");
        exit(1);
    }
    
    while ( 1 )  
    {  
        dht11_read_val(&h, &t);  
        delay( 1000 ); /* wait 1sec to refresh */  
    }  

}
