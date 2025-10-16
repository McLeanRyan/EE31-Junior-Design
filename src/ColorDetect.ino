int ambient = 0;
int delay_time = 50;

int BLACK = 0;
int BLUE = 1;
int YELLOW = 2;
int RED = 3;

int current_blue = 0;
int current_yellow = 0;
int current_red = 0;
int current_class = 0;

int training[4][4] = {
    {-35, -50, 0, BLACK},
    {-170, -180, 0, BLUE},
    {-110, -100, -50, YELLOW},
    {-70, -60, -20, RED}
  };

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);

  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  ambient = analogRead(A5);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);
  // Serial.print("RED: ");
  // Serial.println(analogRead(A5)-ambient);
  current_red = analogRead(A5)-ambient;
  delay(delay_time);

  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  // Serial.print("BLUE: ");
  // Serial.println(analogRead(A5)-ambient);
  current_blue = analogRead(A5)-ambient;
  delay(delay_time);

  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  // Serial.print("YELLOW: ");
  // Serial.println(analogRead(A5)-ambient);
  current_yellow = analogRead(A5)-ambient;
  delay(delay_time);

  digitalWrite(11, LOW);
  // Serial.print("AMBIENT: ");
  // Serial.println(analogRead(A5)-ambient);
  // delay(delay_time);
  current_class = calculateClass(current_blue, current_yellow, current_red);
  Serial.println(current_class);
}

int calculateClass(int blue1, int yellow1, int red1) {
  int distance = 1000000;
  int temp_distance = 0;
  int blue2 = 0;
  int yellow2 = 0;
  int red2 = 0;
  int classifier = 0;

  for (int i = 0; i < 4; i++) {
    blue2 = training[i][0];
    yellow2 = training[i][1];
    red2 = training[i][2];
    temp_distance = pow((pow((blue1-blue2),2))+pow((yellow1-yellow2),2)+pow((red1-red2),2),0.5);
    // Serial.println(i);
    // Serial.println(temp_distance);
    
    if (temp_distance < distance) {
      distance = temp_distance;
      classifier = training[i][3];
    } 
  }
  return classifier;
}