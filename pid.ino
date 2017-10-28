//parameter for pid
#define kp 30
#define kd 30
//speed
#define sp 80
//sensor value threshold
#define BLACK 512
//motor direction
#define FORWARD 0
#define BACKWARD 1
//motor id
#define RM 0
#define LM 1
//pin
#define MI A0  //middle sensor
#define LI A1  //left sensor
#define RI A2  //right sensor
#define DIRA 2 // Direction control for motor A
#define PWMA 3  // PWM control (speed) for motor A
#define DIRB 4 // Direction control for motor B
#define PWMB 11 // PWM control (speed) for motor B

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

//drives robot using pid
void drive(int v=128, int dx_prev=0, int dx=0){
  //parameters:
  //  v: base speed
  //  dx_prev: location of previous loop
  //  dx: location of current loop
  int dv = kp * dx + kd * (dx-dx_prev);//defines spining speed
  int lv = v - dv;//left motor speed
  int rv = v + dv;//right motor speed
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

//gets location of the robot
int get_x(int prev){
//negative location means on the left of the lane, positive on the right
//the more negative, the more on the left, more positive more on the right
    int mi = analogRead(MI);//middle sensor reading
    int li = analogRead(LI);//left sensor reading
    int ri = analogRead(RI);//right sensor reading
    if ((!k(li)&&!k(mi)&&!k(ri))||(k(li)&&k(mi)&&k(ri))){
    // if all sensor are white or all sensor are black
      if (prev<0) return -10;//if previous detection is on the left, then it's on the very left 
      else if (prev>0) return 10;//on the right
      else return 0;//in the middle
    }else if (k(ri)&&!k(mi)&&!k(li)) return -2;//if only the right sensor is black, then it's on the far left
    else if (k(ri)&&k(mi)&&!k(li)) return -1;//if the right and middle sensor is black, then it's on the left
    else if (k(mi)&&!k(li)&&!k(ri)) return 0;//if the middle sensor is black, then it's in the middle
    else if (k(li)&&k(mi)&&!k(ri)) return 1;//if the left sensor and middle sensor is black, then it's on the right
    else if (k(li)&&!k(mi)&&!k(ri)) return 2;//if only the left sensor is black, then it's on teh far right
    else return 0;//othe situations, pretend it's in the middle
}

//setup arduino
void setup()
{
  //All sensor pins are inputs:
  pinMode(MI,INPUT);
  pinMode(LI,INPUT);
  pinMode(RI,INPUT);
  
  // All motor pins are outputs:
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Initialize motor pins as low:
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DIRA, LOW);
  digitalWrite(DIRB, LOW);
}

int x = 0;//current position
void loop()
{
    int newx = get_x(x);//get new location using old location
    drive(sp, x, newx);//use old and new location to drive
    x = newx;//set new location as current location
}

