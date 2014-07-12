
#include <GPS.h>
#include <SoftwareSerial.h>

GPS gps(0); // initialize gps object, 0 argument means no serial manager...

/*Not   all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).*/
SoftwareSerial sserial(8, 9, false);

String inputString;
char incomingByte;
uint8_t index = 0;

void setup() 
{
	gps.init(); // this will initialize all variables to avoid nulls ...

	sserial.begin(9600);//init software serial with 9600 brate for your GPS device, change accordingly
	Serial.println(9600);//init serial to send messages to console
}

void loop() {
	
	while (sserial.available())
	{
		char incomingByte = (char)sserial.read();
		inputString += incomingByte;
		if (incomingByte == '\n') {
			// call update method to parse the String you just collected from the software serial
			// this is a MUST include method
			gps.update(inputString);
		}
	} 
		
	if (gps.dataIsReady())//call dataIsReady() to verify all fields have been setup 
	{
		switch (index)
		{
			case 0:
				Serial.print("Sentence parsed: ");
				//getNmeaSentenceName() - this will show the last parsed sentence
				Serial.println(gps.getNmeaSentenceName());
				break;

			case 1:
				Serial.print("UTC time: ");
				//getTimeUTC() - this shows current UTC time
				Serial.println(gps.getTimeUTC());
				Serial.print("UTC date: ");
				//getDateUTC - this shows the UTC date, the true arguments means is in US format mm.dd.yyyy 
				//if false format will be dd.mm.yyyy 
				Serial.println(gps.getDateUTC(true));
				break;

			case 2:
				Serial.print("Decimal Latitude: ");
				//getLatitude() - returns float decimal latitude 
				Serial.println(gps.getLatitude());
				Serial.print("Decimal Longitude: ");
				//getLongitude() - returns float decimal longitude
				Serial.println(gps.getLongitude());
				break;
	
			case 3:
				Serial.print("Latitude Format1: ");
				// gets latitude in format ##� ##.####' N 
				Serial.println(gps.getLatitudeS1());
				Serial.print("Longitude Format1: ");
				// gets longitude in format ###� ##.####' W 
				Serial.println(gps.getLongitudeS1());
				break;

			case 4:
				Serial.print("Latitude Format2: ");
				// gets latitude in format ##� ##' ###'' N 
				Serial.println(gps.getLatitudeS2());
				Serial.print("Longitude Format2: ");
				// gets latitude in format ###� ##' ###'' W 
				Serial.println(gps.getLongitudeS2());
				break;

			case 5:
				Serial.print("Current Speed: ");
				// get speed in kilometres per hour, the international standard
				Serial.println((String)gps.getSpeed() + "km/h");
				Serial.print("Current Speed: ");
				// get speed in knots per hour
				Serial.println((String)gps.getSpeedKph() + "knt/h");
				Serial.print("Current Speed: ");
				// get speed in miles per hour
				Serial.println((String)gps.getSpeedMph() + "mph");
				break;


			case 6:
				Serial.print("Current Altitude: ");
				/* the function below will ask if altitude can be trusted valid, if so it will print to console,
				// otherwise it will show no geoID which means even if you are getting a value that 
				// you cannot trust that altitude ;)*/
				gps.suspectAltitude() ? Serial.println(gps.getAltitude()) : Serial.println("no geoID");
				Serial.print("Satellites being tracked: ");
				// shows the amount of satellites being tracked 
				Serial.println(gps.getsatellitesTracked());
				Serial.print("Magnetic Deviation: ");
				// show the last part of the GPRMC sentence, the magnetic variation, it was useful to some of us
				// to arrange coordinates... you may use it too 
				Serial.println(gps.getMagneticVariation());
				break;

			case 7:
				Serial.print("Local time: ");
				//getTimeLocal() - this experimental function uses longitude to attempt a local time, it hardly ever works...
				Serial.println(gps.getTimeLocal());
				Serial.print("Local date: ");
				/* getDateUTC - this experimental function shows the UTC date, the true arguments means is in US format mm.dd.yyyy 
				// if false format will be dd.mm.yyyy, if you are planning to navigate outside cote d'ivoire or somewhere in England
				// to NY is probably best if you don't trust this while travelling throughout the GMT :) */
				Serial.println(gps.getDateLocal(true));
				break;
			
			default:
				if (index >= 8)
					index = 0;
			break;
		}
		index++;
	}


}
