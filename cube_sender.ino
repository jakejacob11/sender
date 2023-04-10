#include "LoRaWan_APP.h"
#include "Arduino.h"
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif
#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       8       // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here
char txpacket[BUFFER_SIZE];
char txpacket2[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
double txNumber;
int16_t rssi,rxSize;
void  DoubleToString( char *str, double double_num,unsigned int len);

int c_state = 0;
int p_state = 0;
void setup() {
    Serial.begin(115200);
    pinMode(GPIO5, INPUT);
    txNumber=0;
    rssi=0;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 
   }
void loop()
{
    int val = digitalRead(GPIO5);
  //Serial.print("Pin Value: ");
  //Serial.println(val);
  if(val == 0){
    p_state = c_state;
    c_state = 1;
  }
  else{
    p_state = c_state;
    c_state = 0;
  }
  String s = "Sending...";
  char const *mes = s.c_str();;
  if(p_state != c_state && c_state == 1 ){
    for(int i = 0; i<15; i++){
      Serial.print("Pin Value: ");
      Serial.println(val);
      Serial.print("Sending packet: ");
      Serial.println("OFF");
      //sprintf(txpacket,"%s","@#$%");
     // Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
      sprintf(txpacket2,"%s","@^&^");
      Radio.Send( (uint8_t *)txpacket2, strlen(txpacket2) ); //send the package out
      s = "Sending OFF";
      mes = s.c_str(); 
    }
  }
  else if(p_state != c_state && c_state == 0){
    for(int i = 0; i<15; i++){
      Serial.print("Pin Value: ");
      Serial.println(val);
      Serial.print("Sending packet: ");
      Serial.println("ON");
     // sprintf(txpacket,"%s","@#$%");
     // Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
      sprintf(txpacket2,"%s","@&^&");
      Radio.Send( (uint8_t *)txpacket2, strlen(txpacket2) ); //send the package out
      s = "Sending ON";
      mes = s.c_str(); 
    }
  }
  else{
    if(c_state == 1){
      Serial.print("Pin Value: ");
      Serial.println(val);
      Serial.print("Sending packet: ");
      Serial.println("OFF");
      //sprintf(txpacket,"%s","@#$%");
     // Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
      sprintf(txpacket2,"%s","@^&^");
      Radio.Send( (uint8_t *)txpacket2, strlen(txpacket2) ); //send the package out
      s = "Sending OFF";
      mes = s.c_str(); 
    }
    else if(c_state == 0){
      Serial.print("Pin Value: ");
      Serial.println(val);
      Serial.print("Sending packet: ");
      Serial.println("ON");
     // Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out
      sprintf(txpacket2,"%s","@&^&");
      Radio.Send( (uint8_t *)txpacket2, strlen(txpacket2) ); //send the package out
      s = "Sending ON";
      mes = s.c_str(); 
    }
    // LoRaWAN.display1(mes);
    delay(2000);
  }
	
}
