/* GarageDoorController:  open/close the garage door and report whether the garage door is open or closed.
    This program uses an HC_SR04 ultrasonic range sensor to determine if a garage door
    is open or closed.  It uses the Wireless I/O board to connect up the sensor, and
    uses the relay to trigger a wireless garage door opener by putting
    the relay NO/COM contacts in parallal with the manual button.  If the door opener is powered
    by a Lithium battery (approx 3 volts), then the Wireless I/O board "motor power"
    can be used to power the opener so that no battery is needed.
    
    version 2.0; 7/29/20; By: Bob Glicksman
    
*/

//  STARTUP(WiFi.selectAntenna(ANT_INTERNAL));  // uncomment to use the Photon's internel patch antenna
//  STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));  // uncomment to use an external WiFi antenna on the Photon

// Photon I/O pin definitions; compatible with the Wireless I/O Board
const int TRIGGER_PIN = D1; // ultrasonic sensor trigger
const int ECHO_PIN = D4;    // ultrasonic sensor echo
const int ACTIVATE = D0;    // activate the relay to toggle the garage door state
const int LED_PIN = D7;

// Globals
String distMeasurement; // the distance measurement

// constants
const int BUTTON_TIME = 2000;    // time to trip the relay to "press" the garage opener button
const float DECISION_DISTANCE = 24.0;   // door is closed if distance > 24 inches.


void setup() {
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(ACTIVATE, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    digitalWrite(ACTIVATE, LOW);    // make sure that garage door activation is off
    
    Particle.variable("door_distance", distMeasurement);    // cloud variable containing the door distance in inches
    Particle.function("toggle_garage_door", toggleDoor);    // cloud function to activate the door opener
    Particle.function("take_measurement", usMeasure);       // Ccloud function to take an ultrasonic measurement
}

void loop() {
    // nothing happens in loop but toggle the D7 LED to show that the program is running
    static bool flashLED = false;  // D7 LED is on or off
    static unsigned long currentTime = millis();
    
//  flash the D7 LED on and off every one second to show that the program is running

    if( (millis() - currentTime) > 1000UL) {     // fire off every 1 second
        if(flashLED == false) {
            digitalWrite(LED_PIN, HIGH);
            flashLED = true;
        } else {
            digitalWrite(LED_PIN, LOW);
            flashLED = false;
        }
        
        currentTime = millis();     // update the timer
    }
    
}   // end of loop()

// cloud function to activate the relay to "press" the garage opener button
int toggleDoor(String nothingNeeded) {
    digitalWrite(ACTIVATE, HIGH);   // "press" the button
    delay(BUTTON_TIME);
    digitalWrite(ACTIVATE, LOW);    // "release" the button
    
}   // end of toggleDoor()


// cloud function to measure distance and return an integer decision:
//  0 = door is closed
//  1 = door is open
//  -1 = error in measurement
int usMeasure(String nothingNeeded) {
    double distance = 0.0;
    
    distance= measureDistance(TRIGGER_PIN, ECHO_PIN);
    distMeasurement = String(distance, 1);  // write the measurement to the cloud variable, as a 1 decimal digit string
    
     // is door open or colosed:
    if(distance <= 0.0)  {      // error condition
        return -1;
    }
    
    // measurement is not an error, so determine the door status
    if( (float)distance > DECISION_DISTANCE) {
        return 0;
    } else {
        return 1;
    }   
}   // end of usMeasure()

// function to measure distance in inches using the ultrasonic rangefinder
double measureDistance(int trigPin, int echoPin) {

    unsigned long duration; // variable to hold the distance measurement in microseconds
    
    // Clear the trigger pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
    // Set the trigger pin HIGH for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // process the echo
    duration = pulseIn(echoPin, HIGH);
    
    
  // Calculate the distance
  return duration/74.0/2.0;  // conversion of microseconds to inches  
  
} // end of measureDistance()

