#include <Wire.h>
#include <max6675.h>
#include <LiquidCrystal.h>


//these are the variables that track the button toggle state
int Press = 0;      
int Heat = 0; 
int modeSet = 0; 

int TargetTemp = 0;

int thermoDO2 = 4;
int thermoCS2 = 3;
int thermoCLK2 = 2;

double temp2;

double pressurepulse = 0;
double lastpressurepulse = -30000;


MAX6675 thermocouple2(thermoCLK2, thermoCS2, thermoDO2);

LiquidCrystal lcd(0);

// make a cute degree symbol
uint8_t degree[8]  = {140,146,146,140,128,128,128,128};

int Init = 5;
int Inter = 6;
int Final = 7;

int HeatRelay = 13;
int InitMode = 12;
int InterMode = 11;
int FinalMode = 10;
int PressureRelay = 9;




void setup() {
  Serial.begin(9600);
  
  pinMode(Init, INPUT);
  pinMode(Inter, INPUT);
  pinMode(Final, INPUT);
  
  digitalWrite(Init, HIGH);
  digitalWrite(Inter, HIGH);
  digitalWrite(Final, HIGH);
  
  
  pinMode(8, OUTPUT);
  pinMode(PressureRelay, OUTPUT);
  pinMode(FinalMode, OUTPUT);
  pinMode(InterMode, OUTPUT);
  pinMode(InitMode, OUTPUT);
  pinMode(HeatRelay, OUTPUT);  
  
  
  
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
  lcd.begin(16, 2);
  lcd.createChar(0, degree);

  // wait for MAX chip to stabilize
  delay(500);
  lcd.clear();
 
}

void loop() {
  
  if(digitalRead(Init) == LOW){
    modeSet = 1;
   }
  
  if(digitalRead(Inter) == LOW){
    modeSet = 2;
   }
  
  if(digitalRead(Final) == LOW){
    modeSet = 3;
   }
  
  if(digitalRead(Init) == LOW && digitalRead(Final) == LOW){
    modeSet = 0;
   }
  
  
  
  
  modeDisp(modeSet);
  
  
  
  getTemp();
 
 if(temp2 >= TargetTemp && digitalRead(HeatRelay) == HIGH){
   //set mode to cooling
   modeSet = 4; 
     
 }
 
 if(modeSet == 4 && temp2 < TargetTemp){
   //when below 40C and in the right condition, turn off
   modeSet = 0;
   
 }
 
 
 
  delay(500);
  
}
  
  

  
  
void getTemp() {

  temp2 = thermocouple2.readCelsius();
  
  lcd.setCursor(4,1);
  
  lcd.print("Meas");
     if(temp2 < 100) {
        lcd.print(" ");
        }
    
  lcd.print(temp2);
  lcd.write(byte(0));
  lcd.print("C ");
}

void modeDisp(int mode){
  
   if(mode == 0){
    lcd.setCursor(0,0);
    lcd.print("OFF             ");
   // lcd.write(byte(0));
   // lcd.print("C ");
    
    digitalWrite(HeatRelay, LOW);
    digitalWrite(PressureRelay, LOW);
    digitalWrite(InitMode, LOW);
    digitalWrite(InterMode, LOW);
    digitalWrite(FinalMode, LOW);
    
    TargetTemp = 0;
  }
  
  if(mode == 1){
    lcd.setCursor(0,0);
    lcd.print("INIT  StPt 115");
    lcd.write(byte(0));
    lcd.print("C ");
    
    digitalWrite(HeatRelay, HIGH);
    //digitalWrite(PressureRelay, HIGH);
    PressureOn();
    digitalWrite(InitMode, HIGH);
    digitalWrite(InterMode, LOW);
    digitalWrite(FinalMode, LOW);
    
    TargetTemp = 115;
    
  }
  
   if(mode == 2){
    lcd.setCursor(0,0);
    lcd.print("INTER StPt  103");
    lcd.write(byte(0));
    lcd.print("C ");
    
    digitalWrite(HeatRelay, HIGH);
    //digitalWrite(PressureRelay, HIGH);
    PressureOn();
    digitalWrite(InitMode, LOW);
    digitalWrite(InterMode, HIGH);
    digitalWrite(FinalMode, LOW);
    
    TargetTemp = 103;
  }
  
   if(mode == 3){
    lcd.setCursor(0,0);
    lcd.print("FINAL StPt 108");
    lcd.write(byte(0));
    lcd.print("C ");
    
    digitalWrite(HeatRelay, HIGH);
    //digitalWrite(PressureRelay, HIGH);
    PressureOn();
    digitalWrite(InitMode, LOW);
    digitalWrite(InterMode, LOW);
    digitalWrite(FinalMode, HIGH);
    
    TargetTemp = 108;
  }
  
   if(mode == 4){
    lcd.setCursor(0,0);
    lcd.print("Cooling         ");
    //lcd.write(byte(0));
    //lcd.print("C ");
    
    
    digitalWrite(HeatRelay, LOW);
    //digitalWrite(PressureRelay, HIGH);
    PressureOn();
    digitalWrite(InitMode, LOW);
    digitalWrite(InterMode, LOW);
    digitalWrite(FinalMode, LOW);
    
    TargetTemp = 40;
  }
  
  
  if(mode < 0 || mode > 4) {
    mode = 0;
    }
  
}

void PressureOn() {
 
  pressurepulse = millis();
  if((pressurepulse - lastpressurepulse) > 30000){
  digitalWrite(PressureRelay, HIGH);
  delay(500);
  digitalWrite(PressureRelay, LOW);
  lastpressurepulse = pressurepulse;
  }
  
}

  
