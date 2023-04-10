
#include "LoRaWan_APP.h"
#include "Arduino.h"
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif
#define RGB_READY   0xaaaa00  // Yellow
#define RGB_ON      0x00ff00  // Green
#define RGB_OFF     0xff0000  // Red
#define RGB_PENDING 0x0000ff  // Blue
#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       8         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define RX_TIMEOUT_VALUE                            100
#define BUFFER_SIZE                                 30 // Define the payload size here
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
int16_t txNumber;
int16_t rssi,rxSize;
String lstate = "";
String state ="";
int count = 0;
void setup() {
    Serial.begin(115200);
    pinMode(GPIO5,OUTPUT);
    pinMode(GPIO6,OUTPUT);
    txNumber=0;
    rssi=0;
	  RadioEvents.RxDone = OnRxDone;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    
	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
   //turnOnRGB(COLOR_SEND,0); //change rgb color
   //turnOnRGB(40*256*256 ,0); //change rgb color
   turnOnRGB(RGB_READY ,0); //change rgb color
   Serial.println("into RX mode");
   
   }
   
void loop()
{
	Radio.Rx( 0 );
  delay(100);
  Radio.IrqProcess( );

  if(rxSize == 0){
    count += 1;
  }
  else{
    count = 0;
  }
  Serial.print("Count: ");
  Serial.println(count);
  if(count==200){
    digitalWrite(GPIO5,HIGH);
    delay(2000);
    //digitalWrite(GPIO6,LOW);
    String s = "No Receive Mode";
    char const *mes = s.c_str(); 
    //LoRaWAN.display1(mes);
    lstate = "ON";
  }
  rxSize = 0;
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    //turnOnRGB(RGB_PENDING,0);
    
    Radio.Sleep( );
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,rssi,rxSize);
    if(String(rxpacket) == "@^&^"){
      state = "OFF";
    }
    else if(String(rxpacket) == "@&^&"){
      state = "ON";
    }
    String s = "RSSI: "+String(rssi)+' '+state;
    char const *mes = s.c_str(); 
    //LoRaWAN.display1(mes);
    //Serial.println(rxpacket);
    //Serial.println(state);
    if(lstate != state){
      //delay(2000);    
      if(lstate == "OFF" && state == "ON"){
        //Inverter switching delay
        turnOnRGB(RGB_PENDING,0);
        delay(2000);
        Serial.println("Inverter is ON with 2 sec delay");
        digitalWrite(GPIO5,HIGH);
        //delay(1000);
        //digitalWrite(GPIO6,LOW);
        turnOnRGB(RGB_ON,0);
      }
      else if(state == "OFF"){
        Serial.println("Inverter is OFF");
        digitalWrite(GPIO5,LOW);
        //digitalWrite(GPIO5,LOW);
        turnOnRGB(RGB_OFF,0);
        delay(15000);
        digitalWrite(GPIO5,HIGH);
        turnOnRGB(RGB_READY,0);
        state="ON";
      }
     else if(state == "ON"){
        Serial.println("Inverter is ON");
        digitalWrite(GPIO5,HIGH);
        //delay(1000);
        //digitalWrite(GPIO6,LOW);
        turnOnRGB(RGB_ON,0);
      }
      //delay(1000);
    lstate = state;  
    } 
}
