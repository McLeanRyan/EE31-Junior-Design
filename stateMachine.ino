enum States {
  STOP = 0,
  FORWARD,
  BACKWARD,
  PivotClockwise,
  PivotCounterClockwise,
  TurnRight,
  TurnLeft
};

volatile bool buttonPressed = false;  // set in ISR
States state = STOP;

void nextState() {
  state = (state + 1) % 7;
}

// ISR
void handleButtonInterrupt() {
  buttonPressed = true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(A0), handleButtonInterrupt, FALLING);
}

void loop() {
  // If the button was pressed, update the state
  if (buttonPressed) {
    buttonPressed = false; // clear flag
    nextState();
    Serial.print("State changed to: ");
    Serial.println(state);
  }

  // Repeatedly blink to indicate current state 
  int blinkCount = state + 1;

  for (int i = 0; i < blinkCount; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }

  delay(1000); // pause before blinking again
}
