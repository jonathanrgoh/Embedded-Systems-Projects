

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output

const int button1 = 9;
const int button2 = 8;

char temp=0;

void setup() {
  pinMode(SW_pin, INPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  digitalWrite(SW_pin, LOW);
  Serial.begin(115200);
}

void loop() {
  int xVal=analogRead(X_pin);
  int yVal=analogRead(Y_pin);  
  char data;
  int b1Val=digitalRead(button1);
  char datab1;
  int b2Val=digitalRead(button2);
  char datab2;

  if(xVal<=50)
  {
    data = 'l';
    Serial.write(data);
  }
  else if(xVal>=170)
  {
    data = 'r';
    Serial.write(data);
  }
  else if(yVal<=50)
  {
    data = 'u';
    Serial.write(data);
  }
  else if(yVal>=170)
  {
    data = 'd';
    Serial.write(data);
  }
  else
  {
    data = '-';
    Serial.write(data);
  }

  if(b1Val==1)
  {
    datab1='-';
  }
  else
  {
    datab1='p';
    Serial.write(datab1);
  }

  if(b2Val==1)
  {
    datab2='-'; 
  }
  else
  {
    datab2='q';
    Serial.write(datab2);
  }

  
  Serial.print("BUTTON 1:  ");
  Serial.print(b1Val);
    Serial.print("\n");
  Serial.print("BUTTON 2:  ");
  Serial.print(b2Val);
    Serial.print("\n");
  Serial.print("JOYSTICK SWITCH:  ");
  Serial.print(digitalRead(SW_pin));
    Serial.print("\n");
  Serial.print("X AXIS: ");
  Serial.print(xVal);
    Serial.print("\n");
  Serial.print("Y AXIS: ");
  Serial.println(yVal);
    Serial.print("\n");
  Serial.print("DATA JOYSTICK:  ");
  Serial.print(data);
    Serial.print("\n");
  Serial.print("DATA B1:  ");
  Serial.print(datab1);
    Serial.print("\n");
  Serial.print("DATA B2:  ");
  Serial.print(datab2);
    Serial.print("\n");
    Serial.print("\n\n=================================\n");

  delay(100);
}
