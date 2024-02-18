#include "mbed.h"
#include "drivers/UnbufferedSerial.h"
#include "string.h"
#include <string>
#include "Servo.h"

//Defining servo motor
Servo myservo(PTD0);

// Define UART pins (TX, RX)
UnbufferedSerial uart(PTC4, PTC3);  
UnbufferedSerial pc(USBTX, USBRX);

Ticker t,x, vrata;
AnalogIn dht11(PTC2);
DigitalOut dioda(PTC12), dc(PTD5);
PwmOut zvono(PTA13), zvono2(PTD3);

InterruptIn doorSignal(PTD2);
int signal = doorSignal;

//Function that reads sensor value, and sends it to MQTT broker
void stanjeSenzora(){
    double vr = dht11.read()*100*3.3;
    char radi[35];
    sprintf (radi, "AT+MQTTPUB=0,\"fromesp\",\"%d\",1,0\r\n" , int(vr)); 
    uart.write(&radi, 35);
}

//Function that reads signal comming from LPC114ETF and opens/closes door depending on incoming signal
void stanjeVrata(){
    if(signal == 0) 
    myservo.position(90);
    else
    myservo.position(0);
}

//Function that reads signal for door from MQTT broker
void stanjeVrata2(){
    signal = doorSignal;
    if(signal == 0) 
    myservo.position(90);
    else
    myservo.position(0);
}

//function that uses PWM pin to control buzzer sound
void pozvoni(){
    zvono.period(0.0019f);
    zvono.write(0.50f);
    wait_us(500000);
    zvono.period(0.0051f);
    zvono.write(0.50f);
    wait_us(1000000);
    zvono.write(0.00f);
}

int main() {
    dc = 1;
    zvono = 0;
    pc.baud(115200);
    uart.baud(115200);
    char a;
    while (uart.readable()) {
        uart.read(&a, 1);
        pc.write(&a, 1);
    }

    char kraj[] = "\r\n";
    pc.write(&kraj, 2);
    thread_sleep_for(5000);

    dioda = 1;
    uart.baud(115200);  // Adjust baud rate as needed
    uart.format(8, SerialBase::None, 1);
    thread_sleep_for(1000);

    //Begging of functions used to connect ESP32 to WiFi and MQTT broker
    char c[] = "Slanje AT+CWMODE=1\r\n";
    pc.write(&c, 20);
    char textToSend[] = "AT+CWMODE=1\r\n";
    uart.write((const uint8_t*)textToSend, 13);
    thread_sleep_for(5000);
    while (uart.readable()) {
        uart.read(&a, 1);
        pc.write(&a, 1);
    }
    char d[] = "Slanje AT+CWJAP\r\n";
    pc.write(&d, 17);
    char textToSendd[] = "AT+CWJAP=\"Amina\",\"amina2106\"\r\n";
    uart.write((const uint8_t*)textToSendd, 30);
    thread_sleep_for(20000);
    while (uart.readable()) {
        uart.read(&a, 1);
        pc.write(&a, 1);
    }
    
    char a1[] = "Slanje ATE0\r\n";
    pc.write(&a1, 13);
    char ttextToSend1[] = "ATE0\r\n";
    uart.write((const uint8_t*)ttextToSend1, 13);
    thread_sleep_for(1000);
    while (uart.readable()) {
        uart.read(&a, 1);
        pc.write(&a, 1);
    }
    char e[] = "Slanje AT+MQTTUSERCFG\r\n";
    pc.write(&e, 23);
    char ttextToSend[] = "AT+MQTTUSERCFG=0,1,\"unikat\",\"\",\"\",0,0,\"\"\r\n";
    uart.write((const uint8_t*)ttextToSend, 42);
    thread_sleep_for(10000);
    while (uart.readable()) {
        uart.read(&a, 1);
        pc.write(&a, 1);
    }
    char f[] = "Slanje AT+MQTTCONN\r\n";
    pc.write(&f, 20);
    char textTo[] = "AT+MQTTCONN=0,\"broker.hivemq.com\",1883,0\r\n";
    uart.write((const uint8_t*)textTo, 42);
    thread_sleep_for(20000);
    while (uart.readable()) {
        uart.read(&a, 1);
        pc.write(&a, 1);
    }

    //Subscribing to MQTT channel
    char k[] = "AT+MQTTSUB=0,\"leddioda\",1\r\n";
    uart.write((const uint8_t*)k, 27);

    x.attach(&stanjeSenzora, 15000ms);
    vrata.attach(&stanjeVrata, 1000ms);
    myservo.calibrate(0.001, 90);
    doorSignal.rise(&stanjeVrata2);
    doorSignal.fall(&stanjeVrata2);

    while (1) {
        while(uart.readable()) {
            uart.read(&a, 1);
            pc.write(&a, 1);
            switch(a){  
            case 'j': dioda = 1; break;
            case 'g': dioda = 0; break;
            case 'h': signal = 1; break;
            case 'z': signal = 0; break;
            case 'f': dc = 1; break;
            case 'k': dc = 0; break;
            case 'n': pozvoni(); break;
            case 'p': zvono.write(0.00f); break;
            default: break;}
        }
        
    }
}