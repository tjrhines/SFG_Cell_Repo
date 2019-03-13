#include <SPI.h>

/* List of constants corresponding to the different 
pins that will be used in controlling the valves 
and potentiometer.*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const int valve_pin_array[] = {4, 5, 6, 7, 8, 9};
// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
const int pumpPowerPin = 11; 
//const int LEDPin = 13;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*Initial parameters for pump and relevant variables
r_num is our indicator of the resistor number in digital
potentiometer. We inintialize to the middle of the resistor stack*/
int r_num = 128;

const byte incr = B00000100; const byte decr = B00001000;


SPISettings settings_main(10000000, MSBFIRST, SPI_MODE0); 
SPISettings settings_read(250000, MSBFIRST, SPI_MODE0);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`
void setup() {
  // initialize pyserial-arduino communication at 115200 
  // bits per second: 
Serial.begin(115200);

// initializes different pins and outputs. 
for (int i = 0; i < 6; i++) {
  pinMode(valve_pin_array[i], OUTPUT); 
}
// set the slaveSelectPin (chip select) as an output: 
pinMode(slaveSelectPin, OUTPUT); 
digitalWrite(pumpPowerPin, LOW);



// initialize SPI:
SPI.begin(); 
SPI.beginTransaction(settings_main);
}


  
void open_valve(int val_num) {
  //Opens Valve Number (__) 
  digitalWrite(valve_pin_array[val_num], HIGH);
}

void close_valve(int val_num) {
  //Closes Valve Number (__)
  digitalWrite(valve_pin_array[val_num], LOW);
}

void digitalPotWrite(int value) {
  //Selects Resistor Number (__)
  // take the SS pin low to select the chip: 
  digitalWrite(slaveSelectPin, LOW);
  // send in address (wiper) via SPI: 
  SPI.transfer(0);
  // send in value (resistor number) via SPI: 
  SPI.transfer(value);
  // take the SS pin high to de-select the chip: 
  digitalWrite(slaveSelectPin, HIGH); 
  r_num = value;
}



/* A loop that runs continuously. It runs when data is sent 
   to the arduino. Depending on the string sent to the arduino, 
   the arduino responds accordingly.*/

void loop() {



while (Serial.available() > 0) { // Checks that arduino is receiving input and proceeds to read input from python.
String state = Serial.readStringUntil('\n');
if (state.startsWith("ECHO") )
{ // This is the case where the string written on the "command" function from python begins with"ECHO". It removes the first 5 characters to
String reply = state.substring(5); // return the original message.
Serial.print(reply); 
}
  


//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state.startsWith("VALVE"))
{ 
  if (state[6] == '1'){
    open_valve(0);
  }
  else if (state[6] == '0'){
    close_valve(0);
  }
  if (state[7] == '1'){
    open_valve(1);
  }
  else if (state[7] == '0'){
    close_valve(1);
  }
  if (state[8] == '1'){
    open_valve(2);
  }
  else if (state[8] == '0'){
    close_valve(2);
  }  
  if (state[9] == '1'){
    open_valve(3);
  }
  else if (state[9] == '0'){
    close_valve(3);
  }
  if (state[10] == '1'){
    open_valve(4);
  }
  else if (state[10] == '0'){
    close_valve(4);
  }
  if (state[11] == '1'){
    open_valve(5);
  }
  else if (state[11] == '0'){
    close_valve(5);
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~
else if (state.startsWith("SPEED")){
  int sum = 0;
  if (state[6] == '1'){
        sum = sum + 128;
    }
    if (state[7] == '1'){
        sum = sum + 64;
    }
    if (state[8] == '1'){
        sum = sum + 32;
    }
    if (state[9] == '1'){
        sum = sum + 16;
    }
    if (state[10] == '1'){
        sum = sum + 8;
    }
    if (state[11] == '1'){
        sum = sum + 4;
    }
    if (state[12] == '1'){
        sum = sum + 2;
    }
    if (state[13] == '1'){
        sum = sum + 1;
    }
  digitalPotWrite(sum);
  Serial.println("Speed changed.");
}


//~~~~~~~~~~~~~~~~~~~~~~~
else if (state == "PUMP OFF") 
{ // Stops pump motor.
  //digitalWrite(pumpPowerPin,LOW);
  digitalPotWrite(0);
  Serial.println("Pump is now off."); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state == "PUMP LOW") 
{ // Stops pump motor.
  //digitalWrite(pumpPowerPin,MED);
  digitalPotWrite(128);
  Serial.println("Pump is now on low."); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state == "PUMP HIGH") 
{ // Runs pump.
  //digitalWrite(pumpPowerPin,HIGH);
  digitalPotWrite(255);
  Serial.println("Pump is now on high."); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state.substring(0,5) == "R NUM") //i.e. if state == "R NUM 128", then checks state without the number
{ // Selects desired resistor number in digital potentiometer.
  r_num = state.substring(6).toInt(); //Converts the number part of state to an integer 
  digitalPotWrite(r_num);
  Serial.println("Now on resistor number " + String(r_num) + ".");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state == "R TOP")
{ // Goes to biggest resistor in digital potentiometer.
  digitalPotWrite(256);
  Serial.println("Now on resistor number " + String(r_num) + "."); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state == "R UP")
{ // Selects resistor in digital potentiometer one higher than current one.
  if (r_num < 256) { //Avoids problems from trying to increment when at the top resistor 
    digitalPotWrite(r_num + 1);
  }
  Serial.println("Now on resistor number " + String(r_num) + "."); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else if (state == "R DOWN")
{ // Selects resistor in digital potentiometer one lower than current one.
  if (r_num > 0) { //Avoids problems from trying to decrement when at the bottom resistor 
    digitalPotWrite(r_num - 1);
  }
  Serial.println("Now on resistor number " + String(r_num) + "."); 
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~
else {
String Error = "ERROR: INVALID COMMAND '" + state + "' ";
Serial.print(Error); 
}
} 

}
