// Define the pins we're going to call pinMode on
int BUTTON = D0;
int SAFEGUARD = D1;
int LED = D7;
bool armed = false;
bool deploying = false;

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(SAFEGUARD, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Initial Safeguard to allow the spark to be flashed if something is wrong.
  if(digitalRead(SAFEGUARD) == LOW) {
    Spark.connect();
    Spark.process();
    delay(1000);
  } else if(!armed && digitalRead(BUTTON) == LOW) { // If no safeguard and just woke up (armed = false) and the button is disengaged.
    // Process any outstanding events or updates
    Spark.connect();
    Spark.process();
    // Enable the LED to indicate the device is armed
    digitalWrite(LED, HIGH);
    // Set the armed variable
    armed = true;
    // Publish an event notifying the device is armed
    Spark.publish("armed!", NULL, 60, PRIVATE);
  } else if(armed && digitalRead(BUTTON) == HIGH) { // If the button is armed and the button is engaged, run the deploy!
    Spark.publish("opsworks-deploy", "{\"action\":\"deploy\"}", 60, PRIVATE);
    digitalWrite(LED, LOW);
    armed = false; // Disarm the button
    delay(3000);
  } else if(!armed) { // If the button is not armed and the button is engaged, put it to sleep until it's disengaged.
      Spark.sleep(BUTTON, FALLING);
  } else {
   delay(100);
  }
}
