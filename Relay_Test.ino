/*
* This is the Arduino code for a 5V Relay
 * to control turn ON or OFF AC or DC load
 */

void setup() {
  pinMode(13, OUTPUT);// connected to S terminal of Relay

}

void loop() {

  digitalWrite(13,HIGH);// turn relay ON
  delay(3000);// keep it ON for 3 seconds

  digitalWrite(13, LOW);// turn relay OFF
 delay(5000);// keep it OFF for 5 seconds

}
 
