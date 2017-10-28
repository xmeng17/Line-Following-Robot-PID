#define kp 30
#define kd 30

#define sp 80


#define BLACK 512

#define FORWARD 0
#define BACKWARD 1

#define RM 0
#define LM 1

#define MI A0
#define LI A1
#define RI A2

// Pin Assignments //
//Default pins:
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

////Alternate pins:
//#define DIRA 8 // Direction control for motor A
//#define PWMA 9 // PWM control (speed) for motor A
//#define DIRB 7 // Direction control for motor B
//#define PWMB 10 // PWM control (speed) for motor B

// driveArdumoto drives 'motor' in 'dir' direction at 'spd' speed
void driveArdumoto(byte motor, byte dir, byte spd)
{
  if (motor == RM)
  {
    digitalWrite(DIRA, dir);
    analogWrite(PWMA, spd);
  }
  else if (motor == LM)
  {
    digitalWrite(DIRB, dir);
    analogWrite(PWMB, spd);
  }  
}

void drive(int v=128, int dx_prev=0, int dx=0){
  int dv = kp * dx + kd * (dx-dx_prev);
  int lv = v - dv;
  int rv = v + dv;
  if (lv>0) driveArdumoto(LM,FORWARD,lv);
  else driveArdumoto(LM,BACKWARD,-lv*0.5);
  if (rv>0) driveArdumoto(RM,FORWARD,rv);
  else driveArdumoto(RM,BACKWARD,-rv*0.5);
}

//is black
byte k(int reading){
  if (reading>BLACK) return 1;
  else return 0;
}

int get_x(int prev){
    int mi = analogRead(MI);
    int li = analogRead(LI);
    int ri = analogRead(RI);
    if ((!k(li)&&!k(mi)&&!k(ri))||(k(li)&&k(mi)&&k(ri))){
      if (prev<0) return -10;
      else if (prev>0) return 10;
      else return 0;
    }else if (k(ri)&&!k(mi)&&!k(li)) return -2;
    else if (k(ri)&&k(mi)&&!k(li)) return -1;
    else if (k(mi)&&!k(li)&&!k(ri)) return 0;
    else if (k(li)&&k(mi)&&!k(ri)) return 1;
    else if (k(li)&&!k(mi)&&!k(ri)) return 2;
    else return 0;
}

void setup()
{
  Serial.begin(9600);
  pinMode(MI,INPUT);
  pinMode(LI,INPUT);
  pinMode(RI,INPUT);
  // All pins should be setup as outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}
int x = 0;
void loop()
{
  //Serial.print(analogRead(LI));
  //delay(1000);
    int newx = get_x(x);
    drive(sp, x, newx);
    x = newx;
}

