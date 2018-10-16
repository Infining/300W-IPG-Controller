// TODO: validate command

#include <math.h>
#include <string>
#include <vector>

//----SETTINGS----
const bool debugMode = false; // set if you want to debug through a serial console

//----VARIABLES----
std::string inputCommand = ""; // a string to hold the command from the user
std::string outputCommand = ""; // a string to hold the command going to the laser
std::string responseFromLaser = ""; // recieved laser response
std::string responseToUser = ""; // processed laser response

//----TIMING VARIABLES----
unsigned long progStartTime;
unsigned long loopStartTime;
unsigned long emissionStartTime;
unsigned long diff1Time;
unsigned long diff2Time;

//----LASER CONTROL VARIABLES----
double delaySec = 0.0;
double riseTSec = 0.0;
double onDurSec = 0.0;
double fallTSec = 0.0;
double offDurSec = 0.0;
double minP = 0.0;
double maxP = 0.0;
double phaseShiftSec = 0.0;
double endTSec = 0.0;

//----LASER STATE----
bool laserOn = false;
bool laserEmitting = false;
bool laserConnected = false;

//----SETUP----
void setup() {

  progStartTime = micros();

  // initiate Serials
  Serial.begin(115200); // Serial console debugger
  Serial1.begin(57600); // Galileo PIN0->RX and PIN1->TX at laser baud

  if (debugMode) {
    // print startTime for debuging
    Serial.println("--Galileo-- Start Time: " + String(progStartTime) + " microseconds");
  }
}

//----MAIN LOOP----
void loop() {

  // set the current loop time (microseconds) at the begining of the main loop
  loopStartTime = micros();

  // command to laser
  inputCommand = readCommandFromUser();
  if (!inputCommand.empty()) {
    if (isCommandGalileo(inputCommand)) {
      processToGalileoCommand(inputCommand);
    } else {
      outputCommand = processToLaserCommand(inputCommand);
      sendCommandToLaser(outputCommand);
    }
  }

  // response from laser
  responseFromLaser = readResponseFromLaser();
  if (!responseFromLaser.empty()) {
    responseToUser = processResonseFromLaser(responseFromLaser);
    sendResponseToUser(responseToUser);
  }

  if (laserEmitting) {
    sendCurrentToLaser(); // LSRFN 5.0 1.0 2.0 3.0 4.0 10.0 100.0 0.0 -1.0
  }
}

//----UTILITY FUNCTIONS----

std::string serialRead(Stream *serialX, char endChar) {
  std::string serialString;
  char buff = '\0';
  
  for(int i = 0; serialX->available() > 0 && buff != endChar; i++) {
    for (int n = 0; buff != endChar; n++) {
        buff = serialX->read();
        serialString += buff;
        delayMicroseconds(1);
    }
  }

  return serialString;
}

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

std::string floatToString(float val) {
  char buff[10];
  dtostrf(val, numberOfDigits(val), 1, buff);
  return std::string(buff);
}

int numberOfDigits(float val) {
  int intVal = (int)val;
  int numDig = 0;

  if (intVal >= 0) {
    numDig += 1;
  }
  if (intVal >= 10) {
    numDig += 1;
  }
  if (intVal >= 100) {
    numDig += 1;
  }
  return numDig;
}

//----COMMAND UTILITY FUNCTIONS----

// returns the command type
// command format: "SDC 34.2"
std::string findCommandType(std::string command) {

  std::string commandType = "";
  int spaceIndex = -1;

  spaceIndex = command.find(" ");

  if (spaceIndex != -1) {
    commandType = command.substr(0, spaceIndex);
  } else {
    commandType = command.substr(0, command.length() - 1);
  }

  return commandType;
}

std::vector<std::string> findCommandArgs(std::string command) {
  std::vector<std::string> commandArgs(9);
  
  int spaceIndex = -1;
  int nextSpaceIndex = -1;

  spaceIndex = command.find(" ", spaceIndex + 1);
  nextSpaceIndex = command.find(" ", spaceIndex + 1);

  for (int i = 0; spaceIndex != -1; i++) {
    commandArgs[i] = command.substr(spaceIndex + 1, nextSpaceIndex - spaceIndex - 1);
    spaceIndex = nextSpaceIndex;
    nextSpaceIndex = command.find(" ", spaceIndex + 1);
  }

  return commandArgs;
}

//----GALILEO COMM FUNCTIONS----

void pingLaserConnection() {

  // send dummy command to laser
  Serial1.print("DUMMY\r");

}

std::string readCommandFromUser() {

  std::string command;

  if (debugMode) {
    diff1Time = micros();
  }
  
  command = serialRead(&Serial, '\r');

  // ensure a command has been received
  if (!command.empty()) {

    // debug new command back to user:
    if (debugMode) {
      diff2Time = micros();
      Serial.print("--Galileo-- Reading command from user took: " + (String)(diff2Time-diff1Time) + " us\n");
      Serial.print(("--Galileo-- Command received from user: " + command + "\n").c_str());
    }

  }

  return command;

}

bool isCommandGalileo(std::string command) {

  bool galileoCommand = false;

  std::string commandType = findCommandType(command);

  if (commandType == "LSRFN" || commandType == "PING" || commandType == "EMON" || commandType == "EMOFF") {
    galileoCommand = true;
  }
  
  return galileoCommand;
}

void processToGalileoCommand(std::string command) {

  std::string commandType = findCommandType(command);

  if (commandType == "LSRFN") {
    if (!laserEmitting) {
      std::vector<std::string> args(9);
      args = findCommandArgs(command);
  
      delaySec = atof(args[0].c_str());
      riseTSec = atof(args[1].c_str());
      onDurSec = atof(args[2].c_str());
      fallTSec = atof(args[3].c_str());
      offDurSec = atof(args[4].c_str());
      minP = atof(args[5].c_str());
      maxP = atof(args[6].c_str());
      phaseShiftSec = atof(args[7].c_str());
      endTSec = atof(args[8].c_str());
    }
    
  } else if (commandType == "PING") {
    outputCommand = processToLaserCommand(command);
    sendCommandToLaser(outputCommand);
    delayMicroseconds(10000);
    responseFromLaser = readResponseFromLaser();
    if (!responseFromLaser.empty()) {
      laserConnected = true;
      sendResponseToUser("ONLINE\r");
    } else {
      laserConnected = false;
      sendResponseToUser("OFFLINE\r");
    }
    
  } else if (commandType == "EMON") {
    laserEmitting = true;
    emissionStartTime = micros();
    outputCommand = processToLaserCommand(command);
    sendCommandToLaser(outputCommand);
    
  } else if (commandType == "EMOFF") {
    laserEmitting = false;
    outputCommand = processToLaserCommand(command);
    sendCommandToLaser(outputCommand);
    
  } else {
    if (debugMode) {
      Serial.print("--Galileo-- Galileo command not implemented: ");
      Serial.print(command.c_str());
      Serial.print('\n');
    }
  }
  
  if (debugMode) {
      Serial.print("--Galileo-- Processing Galileo command: ");
      Serial.print(command.c_str());
      Serial.print('\n');
  }
}

// processes the command from the user for forwarding to the laser
std::string processToLaserCommand(std::string command) {

  std::string processedCommand = "";

  // ensure a command has been received
  if (!command.empty()) {

    processedCommand = command;

    // TODO do more processing here
    // TODO process matlab (user) ping


    if (debugMode) {
      Serial.print("--Galileo-- Processing command: from -> ");
      Serial.print(command.c_str());
      Serial.print(" | to -> ");
      Serial.print(processedCommand.c_str());
      Serial.print('\n');
    }

  }

  return processedCommand;

}

void sendCommandToLaser(std::string command) {

  if (!command.empty()) {

    if (debugMode) {
      Serial.print("--Galileo-- Sending command to laser: ");
      Serial.print(command.c_str());
      Serial.print('\n');
    }

    if (debugMode) {
      diff1Time = micros();
    }

    Serial1.print(command.c_str());

    if (debugMode) {
      diff2Time = micros();
      Serial.print("--Galileo-- Sending command to laser took: " + (String)(diff2Time-diff1Time) + " us\n");
    }

  }

}

std::string readResponseFromLaser() {

  std::string response = "";

  if (debugMode) {
    diff1Time = micros();
  }

  response = serialRead(&Serial1, '\r');

  // ensure a response has been received
  if (response != "") {

    if (debugMode) {
      diff2Time = micros();
      Serial.print("--Galileo-- Reading response from laser took: " + (String)(diff2Time-diff1Time) + " us\n");
      Serial.print("--Galileo-- Response received from laser: ");
      Serial.print(response.c_str());
      Serial.print('\n');
    }

  }

  return response;

}

// processes the response from the laser to forward to the user
std::string processResonseFromLaser(std::string response) {

  std::string processedResponse = "";

  if (response != "") {

    // TODO do some processing here
    processedResponse = response;

    if (debugMode) {
      Serial.print("--Galileo-- Processing response: from -> ");
      Serial.print(response.c_str());
      Serial.print(" | to -> ");
      Serial.print(processedResponse.c_str());
      Serial.print('\n');
    }

  }

  return processedResponse;

}

void sendResponseToUser(std::string response) {

  if (response != "") {

    if (debugMode) {
      Serial.print("--Galileo-- Sending response to user: ");
      Serial.print(response.c_str());
      Serial.print('\n');
    }

    if (debugMode) {
      diff1Time = micros();
    }

    if (debugMode) {
      Serial.println(response.c_str());
    } else {
      Serial.println(response.c_str());
    }

    if (debugMode) {
      diff2Time = micros();
      Serial.print("--Galileo-- Sending response to user took: " + (String)(diff2Time-diff1Time) + " us\n");
    }

  }

}

unsigned long secondsToMicros(double seconds) {
  unsigned long microS = seconds * 1.0e6;
  return microS;
}

double laserFun(double delaySec, double riseTSec, double onDurSec, double fallTSec, double offDurSec, double minP, double maxP, double phaseShiftSec, unsigned long t, double endTSec) {
  
  double val = 0.0; // output value

  signed long tnew = t;

  double periodSec = riseTSec + onDurSec + fallTSec + offDurSec; // period of signal
  double r = fmod(phaseShiftSec / periodSec, 1.0); // remainder of shift accounting for period

  tnew += secondsToMicros(r*periodSec); // add shift to time
  tnew -= secondsToMicros(delaySec); // add delay to time

  if (t >= secondsToMicros(endTSec) && endTSec >= 0) {
    val = 0.0;
  } else {
    if (t < secondsToMicros(delaySec)) { // laser hasn't turned on yet
      val = 0.0;
    } else {

      tnew = tnew % secondsToMicros(periodSec); // account for the periodic nature of the function
      
      if (tnew < secondsToMicros(riseTSec)) {
        // power rising
        val = (((maxP-minP) / secondsToMicros(riseTSec)) * tnew) + minP;
      } else if (tnew >= secondsToMicros(riseTSec) && tnew < secondsToMicros(riseTSec + onDurSec)) {
        // max power
        val = maxP;
      } else if (tnew >= secondsToMicros(riseTSec + onDurSec) && tnew < secondsToMicros(riseTSec + onDurSec + fallTSec)) {
        // power falling
        val = (((minP - maxP) / secondsToMicros(fallTSec)) * (tnew - secondsToMicros(riseTSec + onDurSec))) + maxP;
      } else if (tnew >= secondsToMicros(riseTSec + onDurSec + fallTSec) && tnew < secondsToMicros(riseTSec + onDurSec + fallTSec + offDurSec)) {
        // minumum power
        val = minP;
      }
    }
  }

  return val;
}

void sendCurrentToLaser() {

  unsigned long currentTime;
  double current;
  std::string currentStr;

  currentTime = micros() - emissionStartTime;
  current = laserFun(delaySec,
                     riseTSec,
                     onDurSec,
                     fallTSec,
                     offDurSec,
                     minP,
                     maxP,
                     phaseShiftSec,
                     currentTime, // time in microseconds
                     endTSec
                     );

  currentStr = floatToString(current);

  if (debugMode) {
    Serial.print(("--Galileo-- Sending power to laser: " + currentStr).c_str());
    Serial.print('\n');
  }
                   
  if (debugMode) {
    diff1Time = micros();
  }
  
  Serial1.print(("SDC " + currentStr + '\r').c_str());
  
  if (debugMode) {
    diff2Time = micros();
    Serial.print("--Galileo-- Sending power to laser took: " + (String)(diff2Time-diff1Time) + " us\n");
  }
}
