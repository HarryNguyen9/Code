#include<Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

#define VCC  12
#define OUT  4
#define S2   7
#define S3   8
#define S0   6
#define S1   5

#define  Filters_R  0
#define  Filters_G  1
#define  Filters_B  2

long count = 0;
int counter = 0;
Servo feedWheel;
Servo feed;
int G_flag = 1;
int RGB = 0;
float scaleFactor[3];
float result[3];
int rcounter= 0;
int bcounter= 0;
int gcounter= 0;
int tcounter= 0;

// White Balance
void setup()
{
  lcd.begin(16,2);  
  lcd.setCursor(2,0); 
  lcd.print("Mean Machine Team");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("SORTING CANDY MACHINE");
  delay(8000);  
  pinMode(OUT, INPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  feedWheel.attach(3);
  feed.attach(2);
  

  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH);


  Serial.begin(9600);
  Serial.println("Starting...");
  
  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++)
  {
    Select_Filters(i);
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
    scaleFactor[i] = 255.0/count;

    Serial.print("count: ");
    Serial.println(count);
    Serial.print("counter: ");
    Serial.println(counter);
    Serial.print("scaleFactor: ");
    Serial.println(scaleFactor[i],9);    
    counter=0;
    count=0;
  }
  
  digitalWrite(VCC, LOW);

}

void loop()
{
  feedWheel.write(150);
  delay(2000);
  feedWheel.write(83);
  delay(1500);
  Serial.println("loop");
   

  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++)
  {
    RGB=i;
    Select_Filters(i);
    count=0;
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    result[i]=scaleFactor[i]*count;
    Serial.print("value: ");
    Serial.println(result[i]);
    Serial.print("counter ");
    Serial.println(counter);
    counter=0;
    count=0;
 
  }
  if (result[0]>result[1] && result[0]>result[2]){
    rcounter++;
    feed.write(0);
    delay(1000);
    feedWheel.write(15);
    delay(750);
    Serial.println("Red");
    }
     else if (result[2]>result[1] && result[2]>result[0]){
    bcounter++;
    feed.write(30);
    delay(1000);
    feedWheel.write(15);
    delay(750);
    Serial.println("blue");
    }
     else if (result[1]>result[0] && result[1]>result[2]){
     gcounter++;
    feed.write(90);
    delay(1000);
    feedWheel.write(15);
    delay(750);
    } 

    tcounter=rcounter+gcounter+bcounter;
   lcd.print("Total ");
   delay(100);
   lcd.print(tcounter);
   delay(100);
   lcd.clear();
  
  digitalWrite(VCC, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);  
  
  delay(500);
  digitalWrite(S1, HIGH);
     
}

void Select_Filters(int RGB)
{
  switch(RGB)
  {
    case Filters_R:          //Red
    digitalWrite(S2, LOW);    
    digitalWrite(S3, LOW);
    Serial.println("-----select Red color");
    break;
    
    case Filters_G:          //Green
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, HIGH);
    Serial.println("-----select Green color");
    break;
    
    case Filters_B:          //Blue
    digitalWrite(S2, LOW);    
    digitalWrite(S3, HIGH);
    Serial.println("-----select Blue color");
    break;
    
    default:                  //Clear(no filter)
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, LOW);
    Serial.println("-----no filter");
    break;
  }
}
