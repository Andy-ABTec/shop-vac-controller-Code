/*
   Arduino based Shop Vacuum Controller
   ====================================

   Created by: Andy B
   Version:    1.4
   Date:       30/03/2024
   License:    GNU 3.0

   Built and tested using VSCode/Platform IO on an Elegoo Nano, this project
   uses a current transformer to detect if a wired power tool has been started
   or detects if a remote control keyfob has been pressed to control a shop vac 
   using a solid state relay.
*/

// Include Libaries...
#include <Arduino.h>
#include <LibPrintf.h>

//Debug Control...
#define DEBUG 0                                     // #define DEBUG 0 -> Debug off,
                                                    // #define DEBUG 1 -> Debug on

// Initialize delay times...
#define onToolDelay 3                               // Delay between detecting the tool turning
                                                    // on and energising the vac relay
#define offDelayMin 3                               // Value sets the minimum off time delay
#define offDelayMax 30                              // Value sets the maximum off time delay
#define offDelayPin A7                              // pot connection to adjust the "off" time delay
#define vacLedFlashsPerSecond 4                     // Vacuum LED flash rate

// Define Pins...
#define remoteInt 2                                 // ISR trigger for remote
#define toolInt 3                                   // ISR trigger for tool
#define powerLed 12                                 // Red "Power" LED
#define vacLed 11                                   // Yellow "Vacuum" LED
#define toolLed 10                                  // Green "Tool" LED
#define remoteLed 9                                 // Blue "Remote" LED


#if DEBUG == 0
  #define relayPin 7                                // Relay o/p -> SSR via  jumper J2
#else
  #define relayPin 13                               // Relay o/p -> on-board test LED
#endif

// Define Flags...
bool remoteFlag=false;
bool toolFlag=false;
bool vacFlag=false;

// Calculate the off time delay based on the min and max delays and the pot setting
int offDelay(){
int x=analogRead(offDelayPin);
int offDelay=round((offDelayMax-offDelayMin)/1032.0*x+offDelayMin);
return(offDelay);
}

// Flash the vacuum led...
  void flash(int flashesPerSecond, int seconds){
  int rate=500/flashesPerSecond;
  for(int i=0; i<flashesPerSecond*seconds*2; i++)
  {
    digitalWrite(vacLed,not(digitalRead(vacLed)));
    delay(rate);
  }
}

// Remote Interrupt Service Routines...
void remoteISR(void){
  remoteFlag=not(digitalRead(remoteInt));
  #if DEBUG==1
    printf("remoteISR sets remoteFlag = %d\n",remoteFlag);
  #endif
}

void toolISR(void){
  toolFlag=not(digitalRead(toolInt));
  #if DEBUG==1
    printf("toolISR sets toolFlag = %d\n",toolFlag);
  #endif
}

// Setup...
void setup()
{
  #if DEBUG==1                                      // DEBUG: set up the serial port
    Serial.begin(9600);
  #endif

  // Initialise LED pins and blink LEDs...
  int led[4]={powerLed, vacLed, toolLed, remoteLed};
  for (int pin=0; pin<4; pin++)
  {
    pinMode(led[pin],OUTPUT);
    digitalWrite(led[pin],HIGH);
    delay(1000);
    digitalWrite(led[pin],LOW);
  }

  // Initialize relay pin...
  digitalWrite(relayPin,LOW);
  pinMode(relayPin,OUTPUT);

  #if DEBUG==1                                      // DEBUG: test the relay
    digitalWrite(relayPin,HIGH);
    delay(1000);
    digitalWrite(relayPin,LOW);
  #endif

  #if DEBUG==1                                      // DEBUG: print offDelay
    printf("offDelay = %d\n",offDelay());
  #endif

// Attach interrupts...
attachInterrupt(digitalPinToInterrupt(remoteInt), remoteISR, CHANGE);
attachInterrupt(digitalPinToInterrupt(toolInt), toolISR, CHANGE);

// Setup complete, turn on the power led...
  digitalWrite(powerLed,HIGH);
}

// Loop...
void loop(){

  // Remote Start...
  if(remoteFlag==true && toolFlag==false && vacFlag==false){
    digitalWrite(remoteLed,true);                   // Turn on the Remote LED
    digitalWrite(vacLed,true);                      // Turn on the Vacuum LED
    digitalWrite(relayPin,true);                    // Turn on the relay
    vacFlag=true;
  }

  // Tool Start...
 if(remoteFlag==false && toolFlag==true && vacFlag==false){
    digitalWrite(toolLed,true);                     // Turn on the Remote
    flash(vacLedFlashsPerSecond,onToolDelay);       // flash the Vacuum LED
    digitalWrite(vacLed,true);                      // Turn on the Vacuum LED
    digitalWrite(relayPin,true);                    // Turn on the relay
    vacFlag=true;
  }

  // Stop...
  if(remoteFlag==false && toolFlag==false && vacFlag==true){
    flash(vacLedFlashsPerSecond,offDelay());        // flash the Vacuum LED
    digitalWrite(vacLed,false);                     // Make sure the Vacuum LED if off
    digitalWrite(relayPin,false);                   // Turn off the Vacuum relay
    vacFlag=false;
    digitalWrite(remoteLed,false);                  // Turn off the Remote LED
    digitalWrite(toolLed,false);                    // Turn off the Tool LED
  }
}
