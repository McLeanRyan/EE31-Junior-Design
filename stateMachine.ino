enum States {
  RED,
  GREEN,
  BLUE
};

// Set the initial (i.e., starting) state
States state = States::RED;

void nextState() {
  if (state == States::RED) state = States::GREEN;
  else if (state == States::GREEN) state = States::BLUE;
  else state = States::RED;
}

#define R_LED_PIN A0
#define G_LED_PIN A1
#define B_LED_PIN A2

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
}

void loop() {
  // The following actions should always happen,
  // irrespective of the state
  digitalWrite(R_LED_PIN, LOW);
  digitalWrite(G_LED_PIN, LOW);
  digitalWrite(B_LED_PIN, LOW);

  // State-aware actions
  switch(state) {
    case States::RED:
      digitalWrite(R_LED_PIN, HIGH);
      break;
    case States::GREEN:
      digitalWrite(G_LED_PIN, HIGH);
      break;
    case States::BLUE:
      digitalWrite(B_LED_PIN, HIGH);
      break;
  }

  // Simulate some delay and then switch to the next state
  delay(2500);
  nextState();
}
