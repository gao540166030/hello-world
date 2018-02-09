import sys
import time  
import Adafruit_DHT
import RPi.GPIO as GPIO     
      
DataForQT = []
def init():  
    GPIO.setwarnings(False)  
    GPIO.setmode(GPIO.BOARD) 
    
    GPIO.setup(12, GPIO.IN)  
    GPIO.setup(11,GPIO.OUT)
    
    GPIO.setup(33,GPIO.OUT) 
    GPIO.setup(35,GPIO.OUT)
    GPIO.setup(37,GPIO.OUT)
    
    GPIO.setup(36,GPIO.OUT)
    GPIO.setup(38,GPIO.OUT)
    GPIO.setup(40,GPIO.OUT)
    
    GPIO.setup(31,GPIO.OUT)
    GPIO.setup(32, GPIO.IN)
    
    
    
    GPIO.output(33, GPIO.LOW)
    GPIO.output(35, GPIO.LOW) 
    GPIO.output(37, GPIO.LOW)
    GPIO.output(36, GPIO.LOW)
    GPIO.output(38, GPIO.LOW) 
    GPIO.output(40, GPIO.LOW)
    
    GPIO.output(31, GPIO.HIGH)
    
    pass  
  
  
# Parse command line parameters.
sensor_args = { '11': Adafruit_DHT.DHT11,
                '22': Adafruit_DHT.DHT22,
                '2302': Adafruit_DHT.AM2302 }
sensor = sensor_args['11']
DHT11pin = 4


#Temperature  Humidity
def GetTH():
    time.sleep(1)
    humidity, temperature = Adafruit_DHT.read_retry(sensor, DHT11pin)
    if humidity is not None and temperature is not None:
        print('Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temperature, humidity))
        DataForQT.append(temperature)  
        DataForQT.append(humidity)
    else:
        print('Failed to get reading. Try again!')
        sys.exit(1)
    
    if temperature <= 25:
        LED(1)
        GPIO.output(31, GPIO.HIGH)
    elif temperature <= 30:
        LED(2)
        GPIO.output(31, GPIO.LOW)
    elif temperature > 30:
        LED(3)
        GPIO.output(31, GPIO.LOW)
    time.sleep(1)
    
    
#LED      
def G1_LED():
    GPIO.output(33, GPIO.HIGH)
    GPIO.output(35, GPIO.LOW) 
    GPIO.output(37, GPIO.LOW)
    
def Y1_LED():
    GPIO.output(33, GPIO.LOW)
    GPIO.output(35, GPIO.HIGH) 
    GPIO.output(37, GPIO.LOW)
    
def R1_LED():
    GPIO.output(33, GPIO.LOW)
    GPIO.output(35, GPIO.LOW) 
    GPIO.output(37, GPIO.HIGH)
  

def G2_LED():
    GPIO.output(36, GPIO.HIGH)
    GPIO.output(38, GPIO.LOW) 
    GPIO.output(40, GPIO.LOW)
    
def Y2_LED():
    GPIO.output(36, GPIO.LOW)
    GPIO.output(38, GPIO.HIGH) 
    GPIO.output(40, GPIO.LOW)
    
def R2_LED():
    GPIO.output(36, GPIO.LOW)
    GPIO.output(38, GPIO.LOW) 
    GPIO.output(40, GPIO.HIGH)
    
def Y2R2_LED():
    GPIO.output(36, GPIO.LOW)
    GPIO.output(38, GPIO.HIGH) 
    GPIO.output(40, GPIO.HIGH)

def LED(argument):
    switcher = {
        1: G1_LED,
        2: Y1_LED,
        3: R1_LED,
        
        4: G2_LED,
        5: Y2_LED,
        6: R2_LED,
        7: Y2R2_LED,
    }
    func = switcher.get(argument)
    return func()

    
def beep():  
        for i in range(1,6):  
            GPIO.output(11, GPIO.LOW) 
            time.sleep(0.5)  
            GPIO.output(11, GPIO.HIGH)  
            time.sleep(0.5)  
            print "the Buzzer will make sound"  

#HC-SR501 + Slant           
def detct():  
    #for i in range(1, 31):  
    if GPIO.input(12) == True:  
        print time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+"  Someone is closing!"  
        LED(5)
        DataForQT.append(1)
        if GPIO.input(32) == False:
            print time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+"  Something is slanting!"
            LED(7)
            DataForQT.append(1)
            beep()  
        else:
            DataForQT.append(0)
            
    else:  
        GPIO.output(11, GPIO.HIGH)  
        print time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+"  Noanybody!"  
        DataForQT.append(0)
        if GPIO.input(32) == False:
            print time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))+"  Something is slanting!"
            LED(6)
            DataForQT.append(1)
            beep()  
        else:
            LED(4)
            DataForQT.append(0)
    time.sleep(2) 
        
init()  
LED(1)
LED(4)
             
while True:
    
    
    DataForQT = []
    
    GetTH()
    detct()
    
    print DataForQT
    
