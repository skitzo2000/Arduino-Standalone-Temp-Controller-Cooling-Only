#include <OneWire.h>
#include <DallasTemperature.h>

// DS18b20 Temp Sensor Data wire is plugged into port 4 on the Arduino
#define ONE_WIRE_BUS 4
#define TEMPERATURE_PRECISION 11
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//Set pin for Fan control through tip120
int TIP120pin = 11;
//Set Temp max to turn on Fan in Celcius
float hightemp = 17.2;
//SEt Temperature return variable
float temp = 0;

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

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
  numberOfDevices = sensors.getDeviceCount();
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
	{
		Serial.print("Found device ");
		Serial.print(i, DEC);
		Serial.print(" with address: ");
		printAddress(tempDeviceAddress);
		Serial.println();

		Serial.print("Setting resolution to ");
		Serial.println(TEMPERATURE_PRECISION, DEC);

		// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
		sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

		 Serial.print("Resolution actually set to: ");
		Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
		Serial.println();
	}else{
		Serial.print("Found ghost device at ");
		Serial.print(i, DEC);
		Serial.print(" but could not detect address. Check power and cabling");
	}
  }

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
float check_temp(){
  float val = 0;
  val = hightemp - temp;
  return val;
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
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
