/* GarageDoorController:  open/close the garage door and report whether the garage door is open or closed.
    This program uses an HC_SR04 ultrasonic range sensor to determine if a garage door
    is open or closed.  It uses the Wireless I/O board to connect up the sensor, and
    uses the relay to trigger a wireless garage door opener by putting
    the relay NO/COM contacts in parallal with the manual button.  If the door opener is powered
    by a Lithium battery (approx 3 volts), then the Wireless I/O board "motor power"
    can be used to power the opener so that no battery is needed.
    
    version 1.1; 7/11/20; By: Bob Glicksman
    
*/

// Photon I/O pin definitions; compatible with the Wireless I/O Board
const int TRIGGER_PIN = D1; // ultrasonic sensor trigger
const int ECHO_PIN = D4;    // ultrasonic sensor echo
const int ACTIVATE = D0;    // activate the relay to toggle the garage door state
const int LED_PIN = D7;

// Globals
String distMeasurement; // the distance measurement
bool doorIsOpen;        // true if door is open, false if door is closed.

// constants
const int BUTTON_TIME = 2000;    // time to trip the relay to "press" the garage opener button
const float DECISION_DISTANCE = 24.0;   // door is closed if distance > 24 inches.


void setup() {
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(ACTIVATE, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    
    Particle.variable("door_distance", distMeasurement);    // cloud variable containing the door distance in inches
    Particle.variable("Door_Is_Open", doorIsOpen);          // cloud variable containing the open/closed decision
    Particle.function("toggle_garage_door", toggleDoor);    // cloud function to activate the door opener
}

void loop() {
    static bool flash = false;
    double distance = 0.0;
    
    // take a distance reading and record result int he cloud variable
    distance= measureDistance(TRIGGER_PIN, ECHO_PIN);
    distMeasurement = String(distance, 1);
    
    // is door open or colosed:
    if( (float)distance > DECISION_DISTANCE) {
        doorIsOpen = false;
    } else {
        doorIsOpen = true;
    }
    
    delay(1000);    //wait one second before taking another measurement
    
    // toggle the LED to show that the program is running
    if(flash == false) {
        digitalWrite(LED_PIN, LOW);
        flash = true;
    } else {
        digitalWrite(LED_PIN, HIGH);
        flash = false;
    }
}

// cloud function to activate the relay to "press" the garage opener button
int toggleDoor(String nothingNeeded) {
    digitalWrite(ACTIVATE, HIGH);   // "press" the button
    delay(BUTTON_TIME);
    digitalWrite(ACTIVATE, LOW);    // "release" the button
}


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

