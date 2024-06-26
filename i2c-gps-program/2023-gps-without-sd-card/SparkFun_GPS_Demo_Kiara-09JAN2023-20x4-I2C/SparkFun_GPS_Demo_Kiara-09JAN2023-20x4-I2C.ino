/*
  6-8-10
  Aaron Weiss
  SparkFun Electronics
  
  Example GPS Parser based off of arduiniana.org TinyGPS examples.
  
  Parses NMEA sentences from an EM406 running at 4800bps into readable 
  values for latitude, longitude, elevation, date, time, course, and 
  speed. 
  
  For the SparkFun GPS Shield. Make sure the switch is set to DLINE.
  
  Once you get your longitude and latitude you can paste your 
  coordinates from the terminal window into Google Maps. Here is the 
  link for SparkFun's location.  
  http://maps.google.com/maps?q=40.06477,+-105.20997
  
  Uses the NewSoftSerial library for serial communication with your GPS, 
  so connect your GPS TX and RX pin to any digital pin on the Arduino, 
  just be sure to define which pins you are using on the Arduino to 
  communicate with the GPS module. 
  
  REVISIONS:
  1-17-11 
    changed values to RXPIN = 2 and TXPIN = to correspond with
    hardware v14+. Hardware v13 used RXPIN = 3 and TXPIN = 2.
  
*/

// In order for this sketch to work, you will need to download
// TinyGPS library from arduiniana.org and put them
// into the hardware->libraries folder in your ardiuno directory.
#include <Wire.h>
#include <SerLCD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
//added 39-40 for mega
#include <defaults.h>  //changed from defaultsMega.h
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>

//for 20x4 lcd display


SerLCD lcd;
//-SerLCD OpenLCD;
//lcd.begin(Wire);

// Define which pins you will use on the Arduino to communicate with your
// GPS. In this case, the GPS module's TX pin will connect to the
// Arduino's RXPIN which is pin 3.
#define RXPIN 4
#define TXPIN 5
//Set this value equal to the baud rate of your GPS
#define GPSBAUD 4800

// Create an instance of the TinyGPS object
TinyGPS gps;
// Initialize the NewSoftSerial library to the pins you defined above
SoftwareSerial uart_gps(RXPIN, TXPIN);
//-SoftwareSerial OpenLCD(6,7); // pin 7 = TX, pin 6 = RX (unused)

// This is where you declare prototypes for the functions that will be
// using the TinyGPS library.
void getgps(TinyGPS &gps);

// In the setup function, you need to initialize two serial ports; the
// standard hardware serial port (Serial()) to communicate with your
// terminal program an another serial port (NewSoftSerial()) for your
// GPS.
void setup() {
  // This is the serial rate for your terminal program. It must be this
  // fast because we need to print everything before a new sentence
  // comes in. If you slow it down, the messages might not be valid and
  // you will likely get checksum errors.
  Serial.begin(115200);

  Wire.begin();
  lcd.begin(Wire);

  Wire.setClock(400000);  //Optional - set I2C SCL to High Speed Mode of 400kHz

  lcd.clear();
  lcd.cursor();  //Turn on the underline cursor
                 //lcd.print("Watch the cursor!");

  //- OpenLCD.begin(9600); // set up serial port for 9600 baud  BobR
  delay(500);  // wait for display to boot up   BobR

  //Sets baud rate of your GPS
  uart_gps.begin(GPSBAUD);

  Serial.println("");
  Serial.println("GPS Shield QuickStart Example Sketch v12");
  Serial.println("       ...waiting for lock...           ");
  Serial.println("");
  //-OpenLCD.write(254); // move cursor to beginning of first line
  //-OpenLCD.write(128);

  // lcd.init();
  //Wire.begin();
  //lcd.begin(serial);
  // lcd.setCursor(0,0);
  //lcd.print("Timey: ");
}

// This is the main loop of the code. All it does is check for data on
// the RX pin of the ardiuno, makes sure the data is valid NMEA sentences,
// then jumps to the getgps() function.
void loop() {
  while (uart_gps.available())  // While there is data on the RX pin...
  {
    int c = uart_gps.read();  // load the data into a variable...
    if (gps.encode(c))        // if there is a new valid sentence...
    {
      Serial.println("       ...HERE IT COMES...           ");  //BobR
      getgps(gps);                                              // then grab the data.
    }
  }
}

// The getgps function will get and print the values we want.
void getgps(TinyGPS &gps) {
  // To get all of the data into varialbes that you can use in your code,
  // all you need to do is define variables and query the object for the
  // data. To see the complete list of functions see keywords.txt file in
  // the TinyGPS and NewSoftSerial libs.

  // Define the variables that will be used
  float latitude, longitude;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  Serial.print("Lat/Long: ");
  Serial.print(latitude, 5);
  Serial.print(", ");
  Serial.println(longitude, 5);

  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths);
  // Print data and time
  Serial.print("Date: ");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(day, DEC);
  Serial.print("/");
  Serial.print(year);
  int echour = hour - 5;
  Serial.print("  Time: ");
  Serial.print(echour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(second, DEC);
  Serial.print(".");
  Serial.println(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr

  // BobR Write TIME to Display
  //- OpenLCD.write("                "); // clear display
  //-  OpenLCD.write("                ");

  //OpenLCD.write(254); // move cursor to beginning of first line
  //OpenLCD.write(192);
  //lcd.setCursor(0,0);

  //- OpenLCD.write("Time: "); OpenLCD.print(hour, DEC); OpenLCD.print(":");
  //- OpenLCD.print(minute, DEC); OpenLCD.print(":");
  //- OpenLCD.print(second, DEC); OpenLCD.print(":"); OpenLCD.print(hundredths, DEC);

  // lcd.init();
  lcd.setCursor(4, 0);
  lcd.print("hello all");


  lcd.setCursor(0, 2);
  lcd.print("Time: ");
  lcd.print(echour, DEC);
  lcd.print(":");
  lcd.print(minute, DEC);
  lcd.print(":");
  lcd.print(second, DEC);
  lcd.print(":");
  lcd.print(hundredths, DEC);
  lcd.print("  ");


  //display speed
  lcd.setCursor(0, 3);
  lcd.print("Speed: ");
  lcd.print(gps.f_speed_kmph());
  // Here you can print the altitude and course values directly since
  // there is only one value for the function
  Serial.print("Altitude (meters): ");
  Serial.println(gps.f_altitude());
  // Same goes for course
  Serial.print("Course (degrees): ");
  Serial.println(gps.f_course());
  // And same goes for speed
  Serial.print("Speed(kmph): ");
  Serial.println(gps.f_speed_kmph());
  Serial.println();

  // Here you can print statistics on the sentences (not used here).
  // unsigned long chars;
  // unsigned short sentences, failed_checksum;
  // gps.stats(&chars, &sentences, &failed_checksum);
  //Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
  //Serial.println(); Serial.println();
}
