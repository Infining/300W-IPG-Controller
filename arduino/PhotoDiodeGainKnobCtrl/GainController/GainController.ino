#include <AccelStepper.h>
#include <EEPROM.h> //lets you save to non-volatile memory
#include <VoltageReader.h>

#define HalfStep 8
int LED = 13;

 
VoltageReader V(0,5,1);//Analog pins assigned to measure voltage across resistor from backup battery.  
String DataIn = ""; //a string to hold incoming data
boolean StriComplete = false;//true if user entered data string is done being read onto the Arduino
float Num; //the number value user inputs after mode specifier
float Potential;
String CharStr;//String characters user inputs
//range of values -2,147,483,648 to 2,147,483,647
//long TrackPos; // tracks the net changes in position during the current power cycle
#define eeAddressPos 0 //location on the non-volatile drive to store data
#define eeAddressPins sizeof(long)
long TrackPos;
long HomeOffset;
//Default settings:
char Mode = 'S'; // "S" for half step default mode
char SubMode; // sub mode, no default set
struct Pins {
  int Pin1N1 = 2;// Pin1Nx are the pins on the stepper motor board, 2-5 are the pins on the Arduino
  int Pin1N2 = 3; int Pin1N3 = 4; int Pin1N4 = 5;    
            };
Pins P;//P is an instance of the structure Pins
int Accel = 300;//steps/s^2
int MaxSpeed = 5000; // steps/s set max so user cant go over 
int Speed = 4000; //steps/s default speed of stepper motor

AccelStepper stepper1(HalfStep, P.Pin1N1, P.Pin1N3, P.Pin1N2, P.Pin1N4); // declaring stepper as an instance of
//class AccelSteper

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps  
  DataIn.reserve(200);// reserve 200 bytes for DataIn:
  EEPROM.get(eeAddressPos, TrackPos);
  HomeOffset = TrackPos;
  //More default Settings:  
  EEPROM.get(eeAddressPins,P);//if saved Pins are located in EEPROM use them, otherwise use defaults
  if (sizeof(P)<8){ 
    Pins P;
  }
  pinMode(LED, OUTPUT);
  AssignPins();
   
  Serial.println("Ready");//prints to serial port when Arduino is ready to receive commands
}

void loop()
{
  ReadSerialLine();//reads in the serial data
  
//Below performs all actions the user intiated once the user serial data is completely read in:
  if (StriComplete == true)
  { // Sepparates the Mode specifiers (letters) and the number values from the user serial input
     ParseInput(); // parses user input read in with ReadSerialLine into strings and long ints

//Takes the picked Mode (user string input) and determines what actions should be taken from the switch cases below
    //Then uses the numbers the user entered in for the switch-case selected (based on Mode)
    switch (toUpperCase(CharStr[0]))
    {      
// Set-Pin Mode:  Pin assignment for Arduino to stepper motor coils"
      case 'P': //this allows for user to change the default pin settings in case they are changed or
        //two motors are being controlled. Arduino pins (in order of user input) correspond
        //to: 1N1, 1N2, 1N3, and 1N4 respectively on the stepper mmotor board.
        {
          if (CharStr == "P?")
          {//when asked what the pin setting is
              Serial.print(P.Pin1N1); Serial.print(P.Pin1N2);
              Serial.print(P.Pin1N3); Serial.println(P.Pin1N4);            
          }
          else
          {
            // Pair Motor pin (left variables) to an arduino pin (right variables)
            if (DataIn.length() == 5)
            { // Needs 1 specifier 'P' and 4 numbers to assign pins
              P.Pin1N1 = int(DataIn[1]) - '0'; P.Pin1N2 = int(DataIn[2]) - '0';
              P.Pin1N3 = int(DataIn[3]) - '0'; P.Pin1N4 = int(DataIn[4]) - '0';
              // Redefining the steps:
              AssignPins(); 
              EEPROM.put(eeAddressPins, P);
              Serial.println("Done");
            }
            else
            {
              Serial.println("Error:Incorrect number of inputs for pin assignment."
                             "Must be P followed by 4 numbers" );
            }
          }
        }
        break;
        
// Move Mode: handles step motion and degree motion for both relative and absolute moves
      case 'M':
      {        
        if (CharStr.length() == 3)
        {
          int R = 4096;//half steps per 1 rotation
          
          if (CharStr == "MSA") // Absolute number of steps
          {//M-move, S-step, A-absolute motion (based off of home location)
             stepper1.moveTo(long(Num)-HomeOffset);
//             TrackPos += Num-HomeOffset;
             TrackPos = Num;
          }
          else if (CharStr == "MSR")//Relative number of steps
          {//M-move, S-step, R-relative motion (based off of current location)
             stepper1.move(long(Num));
             //Serial.println(Num,4);
             TrackPos += Num; 
          }
          else if (CharStr == "MDA")
          {//M-move, D-degrees, A-absolute motion (based off of home location)
            float r = Num*R/360;
            long Numi = long(round(r));
            stepper1.moveTo(Numi-HomeOffset);
//            TrackPos += Numi-HomeOffset;
            TrackPos = Numi;
//            Serial.println(r);
//            Serial.println(Numi);            
          }
           else if (CharStr == "MDR")
          {//M-move, D-degrees, R-relative motion (based off of current location)
            float r = Num*R/360;
            long Numi = long(round(r));
            stepper1.move(Numi);
            TrackPos += Numi;
//            Serial.println(r);
//            Serial.println(Numi);
          }         
        }
      }
      break;    
             
// Home Mode (Absolute motion): Returns stepper motor to home position zero, or set new home position
      case 'H':
        {
          if (CharStr.length() == 1)
          { // Go to home position
            stepper1.move(-TrackPos);
            TrackPos = 0;
          }
          else if (CharStr == "HS") // If there is another element in CharStr check to see if it is 'S'
          { // Set new home position to current position
            stepper1.setCurrentPosition(stepper1.currentPosition());
            TrackPos = 0;   
            HomeOffset = 0;
            EEPROM.put(eeAddressPos, TrackPos);
//            delay(30);
           // Soft_Reset();
          }
        }
        break;
        case 'S'://saves current motor position for next use
        {
          EEPROM.put(eeAddressPos, TrackPos);
          Serial.println("Done"); 
        }
        break;

        case 'L': //user asks arduino where the motor is
        {
          Serial.println(TrackPos);
        }
        break;

        case 'V'://user wants to do something with the velocity of the motor
        {
           if (CharStr == "V?")//user wants to know what the velocity is set to
           {
            Serial.println(stepper1.speed(),0);
           }
           else if (CharStr == "V")//user wants to set speed
           {
             stepper1.setSpeed(Num);
           }
           else if (CharStr == "VM?")//user wants to know what max speed is
           {
            Serial.println(stepper1.maxSpeed(),0);
           }
           else if (CharStr == "VM")// sets max speed
           {
             stepper1.setMaxSpeed(Num);
           }          
        }
        break; 

        case 'A'://user wants to do something with acceleration
        {
          if (CharStr =="A?")//user wants to know what the current acceleration is
          {
            Serial.println(Accel);
          }
          if (CharStr =="A")//user wants to set the acceleration
          {
            Accel = Num;
            stepper1.setAcceleration(Accel);
          }                    
        }
        break;
    }    
    
//Reset for next user input:
    StriComplete = false; //makes it so it doesn't infinitely repeat user commands
    DataIn = ""; // resets the string for next line read
    CharStr = "";
  }  
  
  if (stepper1.distanceToGo() == 0)
  {//set pins to zero
    digitalWrite(P.Pin1N1, LOW); digitalWrite(P.Pin1N2, LOW);
    digitalWrite(P.Pin1N3, LOW); digitalWrite(P.Pin1N4, LOW);
  }
  else
  {     
   int test = 0;
   //int origPosition = stepper1.currentPosition()+HomeOffset; 
  
   int dist = stepper1.distanceToGo();
   int oldDist =  dist;
    while(dist!=0)
    { //makes motor move uninterruptable-good for smoothest/fastest movements, bad if you want to do something
      //while its running
      
      test = 1;
      Potential = V.Volts();
     
     
//      if (abs(P)>.045 & abs(P) < 0.15)
//      {//if ture then power supply to arduino has been interrupted
//        //Wait to see if it comes back 
//      }
      
      stepper1.run();//moves the stepper motor to the position (if any) it was told to go to  
      dist = stepper1.distanceToGo();   
      if (dist == oldDist-20)
      {
       
//        origPosition+=oldDist-dist;
//        Serial.println(origPosition); 
        Serial.println(stepper1.currentPosition()+HomeOffset);
        
        oldDist = dist;
//        //Serial.println(stepper1.currentPosition()+HomeOffset);
      }      
     }
    if (test == 1)//then motor just moved
    {
      Serial.println(stepper1.currentPosition()+HomeOffset);
      Serial.println("Done");         
    }
    
    Potential = V.Volts();
    
  }
//   if (~Serial)
//     {
//        digitalWrite(LED, LOW);
//     }
//     else
//     {
//       digitalWrite(LED, HIGH);
//     }
}




// Supporting Functions:

// Parses the user input into strings and numbers 
void ParseInput()
{
  //Serial.println("Called");
  //Serial.println(DataIn);
  int tempVar; String NumStr; 
  String Decimals; boolean isDec = false;
  for (int kk = 0; kk < DataIn.length(); kk++)
  {
    tempVar = int(DataIn[kk]) - '0';
    if ((tempVar >= 0) && (tempVar <= 9) && isDec == false)        
    {//DataIn[kk] is a number (does not save to the right of the decimal thats handled below)
      NumStr += DataIn[kk];
    }
    else if (tempVar == -3 ) // to handle negative signs, respectively
    { NumStr += DataIn[kk];}
    else if ((tempVar == -2) || (isDec == true))//to handle decimal points '.'
    { 
      isDec = true;
      NumStr += DataIn[kk];
    }
    else if (tempVar == -16)// to remove any spaces
    { // do nothing
    // Serial.println("Space Removed");
    }
    else
    { CharStr += DataIn[kk];}
  }    
//Convert string input to all caps to remove case sensitivity 
  if (CharStr.length() > 0)
  {
    for (int jj = 0; jj < CharStr.length(); jj++)
    { CharStr[jj] = toUpperCase(CharStr[jj]); }
  }    
// Convert user entered number values into long ints from string values or floats if decimal
  if (NumStr.length() > 0)
  {
    if (isDec == true)
    { Num = NumStr.toFloat();
    }
    else
    { Num = NumStr.toInt();//converts from string to int (in this case long int)     
      return 0;
    }
  }
}

void AssignPins()
{
  AccelStepper stepper2(HalfStep, P.Pin1N1, P.Pin1N3, P.Pin1N2, P.Pin1N4);
  stepper1 = stepper2;
  stepper1.setAcceleration(Accel); //steps/s^2
  stepper1.setMaxSpeed(MaxSpeed);//steps/s user can not set the speed higher than this
  stepper1.setSpeed(Speed);//steps/s default speed of stepper motor
}

void ReadSerialLine()
{
  if (Serial.available() > 0) //only run code if there is data in the serial buffer
  {
    char inChar = (char)Serial.read();
    if (inChar == '\r' || inChar == '\n') // end of Serial input return values
    { StriComplete = true;}
    else
    { DataIn += inChar;}  //appends the new character in inChar to the end of the String DataIn    
  }
}

void Soft_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  asm volatile(" jmp 0");
}

 
