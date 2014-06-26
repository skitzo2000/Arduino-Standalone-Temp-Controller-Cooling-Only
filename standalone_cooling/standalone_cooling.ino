#include <OneWire.h>
#include <DallasTemperature.h>

// DS18b20 Temp Sensor Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4
//#define TEMPERATURE_PRECISION 12
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//Set pin for Fan control through tip120
int TIP120pin = 11;
//Set Temp max to turn on Fan in Celcius
int hightemp = 18;
//SEt Temperature return variable
int temp = 0;
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

void setup(void)
{
  // start serial port Note I'm using a bluetoothe arduino so you serial port number may be different most common is 9600
  Serial.begin(115200);
  Serial.println("Fan Control with Dallas Temp Sensor Demo");
  pinMode(TIP120pin, OUTPUT); // Set pin for output to control TIP120 Base pin
  // Start up the library
  sensors.begin();
  delay(2000);
}

void call_temp(){
   // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  temp = sensors.getTempCByIndex(0);
  //Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(temp);  
}
//function to compare temperature and return the difference between the high temp setting and current temp
int check_temp(){
  int val = 0;
  val = hightemp - temp;
  return val;
}


void loop(void)
{ 
  call_temp();
  //Serial.println(check_temp());
  if (check_temp() < 0) {  //if the current temp is higher than the current temp turn on the fan
      analogWrite(TIP120pin, 255);
  } else {
     analogWrite(TIP120pin, 0);
  }
  delay(1000);
}
