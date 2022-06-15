#include <Servo.h>
Servo servo2;

#define WCout 9 //toilet trig pin
#define WCin 10 //toilet echo pin
#define Tout 7 //water tank trig pin
#define Tin 8 //water tank echo pin
#define Sout 5 //sink trig pin
#define Sin 6 //sink echo pin
#define TapIn 12 //αισθητήρας για πλύσιμο χεριών echo
#define TapOut 11//αισθητήρας για πλύσιμο χεριών trig
#define servo_pin 22 //
#define button_pin 24
#define tapPin 26 // Για ενεργοποίηση της βρύσης για πλύσιμο χεριών
#define apoxeteyshPin 46 //Το pin που ενεργοποιεί την "κρυφή" αντλία προς το δίκτυο

int red_light_pin= 45;
int green_light_pin = 44;
int angle = 40; //αρχική γωνία σερβοκινητήρα καζανακιού

int button_pushed = 0;
long currentMillis = 0;       
long interval = 2000;        
long startMillis=0;
int buttonFirstPressed=0;
int TapDist;
void setup() {
  // put your setup code here, to run once:
 
Serial.begin(9600);
servo2.attach(servo_pin);
pinMode(button_pin,INPUT_PULLUP);
Serial.println("button");
servo2.write(angle);

  pinMode(2,OUTPUT);  
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(WCout,OUTPUT);
  pinMode(WCin,INPUT);
  pinMode(Tout,OUTPUT);
  pinMode(Tin,INPUT);
  pinMode(Sout,OUTPUT);
  pinMode(Sin,INPUT);
  
  pinMode(TapOut,OUTPUT);
  pinMode(TapIn,INPUT);
  pinMode(tapPin,OUTPUT);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(apoxeteyshPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//int limit=analogRead(A0);
//Serial.println(limit);
int epipedoNerouApoxeteyshs=analogRead(A0);
if(epipedoNerouApoxeteyshs>300) //αν στο δοχείο αποχέτευσης το νερό φτάσει ψηλά το επιστρέφουμε στο δίκτυο
{
digitalWrite(46,HIGH); //Ανοίγουμε αντλία που το στέλνει στο δίκτυο
delay(800);
}
else
digitalWrite(46,LOW); //κλείνουμε αντλία που το στέλνει στο δίκτυο

if(button_pushed || buttonFirstPressed==1 ){
 analogWrite(green_light_pin, 180);
 analogWrite(red_light_pin, 255);
 delay(2000);
 analogWrite(green_light_pin, 0);
 analogWrite(red_light_pin, 0);
 }

digitalWrite(Sout,LOW);
delayMicroseconds(2);
digitalWrite(Sout,HIGH);
delayMicroseconds(10);
digitalWrite(Sout,LOW);
int duration1=pulseIn(Sin,HIGH);
int sinkDist=duration1*0.034/2;

digitalWrite(Tout,LOW);
delayMicroseconds(2);
digitalWrite(Tout,HIGH);
delayMicroseconds(10);
digitalWrite(Tout,LOW);
int duration2=pulseIn(Tin,HIGH);
int tankDist=duration2*0.034/2;

digitalWrite(WCout,LOW);
delayMicroseconds(2);
digitalWrite(WCout,HIGH);
delayMicroseconds(10);
digitalWrite(WCout,LOW);
int duration3=pulseIn(WCin,HIGH);
int WCDist=duration3*0.034/2;

Serial.println("sink: ");
Serial.println(sinkDist);
Serial.println("tank: ");
Serial.println(tankDist);
Serial.println("WC: ");
Serial.println(WCDist);
Serial.println("tap: ");
Serial.println(TapDist);





if (sinkDist<=5)
{
  if (tankDist<8) 
  {     //tank full
    
    digitalWrite(2,LOW); //σβήνει αντλία
  }
  else if (tankDist>=10) 
  {    //tank empty
    
    digitalWrite(2,HIGH); //αντλία λειτουργεί
  }
}
else if (sinkDist>6) //δοχείο νεροχύτη άδειο, δεν έχει 
{
  digitalWrite(2,LOW); //αντλία σβήνει
  
}

if (WCDist>=13)

{
  analogWrite(green_light_pin,0 );
analogWrite(red_light_pin,255 );
  if (tankDist<16) 
  {     //tank full
    
    digitalWrite(3,HIGH); //ανοίγει βάνα από ντεπόζιτο
     Serial.println("prasino on");
    digitalWrite(4,LOW); //κλείνει η βάνα από δίκτυο
  }
  else if (tankDist>=16) 
  {    //tank empty
    
    digitalWrite(3,LOW); //κλείνει βάνα από ντεπόζιτο
    Serial.println("prasino off");
    digitalWrite(4,HIGH); //ανοίγει η βάνα από δίκτυο
    
  }
}
else if (WCDist<13) //καζανάκι γεμάτο 
{
  analogWrite(green_light_pin,255);
  analogWrite(red_light_pin,0 );
  digitalWrite(3,LOW); //κλείνει βάνα από ντεπόζιτο
  digitalWrite(4,LOW); //κλείνει βάνα από δίκτυο
  
}

digitalWrite(TapOut,LOW);
delayMicroseconds(2);
digitalWrite(TapOut,HIGH);
delayMicroseconds(10);
digitalWrite(TapOut,LOW);
int duration4=pulseIn(TapIn,HIGH);
TapDist=duration4*0.034/2;
Serial.print("βρύση");
Serial.println(TapDist);
 if (TapDist<=10)
{
  
    digitalWrite(tapPin,HIGH); 
  }
 else 
  {    
    
    digitalWrite(tapPin,LOW); 
  }


if(digitalRead(button_pin) == LOW){
  button_pushed = 1;
  buttonFirstPressed=1;
}
if(button_pushed && buttonFirstPressed==1 ){
  
  servo2.write(angle-45);
  buttonFirstPressed=0;
    //delay(3000);
      startMillis = millis();
 
   Serial.println("button pressed");
  
    //servo2.write(angle);
    //button_pushed=0;
      
 }
 
 if(button_pushed==1)
 {   
  Serial.println("test time");
 currentMillis=millis();
 Serial.print("current:");
 Serial.println(currentMillis);
 Serial.print("start:");
 Serial.println(startMillis);
 
  if(currentMillis - startMillis > interval) {
    
    
      button_pushed =0;
     servo2.write(angle+45);
     Serial.println("finito");
  }
 }



Serial.print("Moved to: ");
Serial.println(angle);
Serial.print(" degree");
//delay(500);

}
