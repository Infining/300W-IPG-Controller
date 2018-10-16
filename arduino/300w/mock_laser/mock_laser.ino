#include <math.h>
#include <string>

//----SETTINGS----
const bool debugMode = false; // set if you want to debug through a serial console
const bool debugVerbose = false; // set if you want verbose information to be shown

//----VARIABLES----
std::string mockInternalLaserCommand = ""; // a mock string that holds the command received by the laser
std::string mockLaserResponseToGalileo = ""; // a mock stirng to hold the response string internal to the laser

//----TIMING VARIABLES----
unsigned long startTime;
unsigned long currentLoopTime;
unsigned long diff1Time;
unsigned long diff2Time;

//----SIMULATED LASER VARIALBES----
int laserPin = 6;
double laserCurrent = 0.0; //percentage
bool laserKeyswitchON = true; // physical laser keyswitch state: true => ON position, false => REM position
bool laserEmission = false; // laser emission state: true => on, false => off
bool laserModulation = false; // laser modulation state: true => on, false => off
bool laserExternalControl = false; // laser external control state: true => on, false => off

//----SETUP----
void setup() {

  // get start time (microseconds) from the time the "laser" was booted:
  startTime = micros();

  // simulated laser output LED pin
  pinMode(laserPin, OUTPUT);

  // initiate Serials
  Serial.begin(115200); // Serial console debugger
  Serial1.begin(57600); // Galileo PIN0->RX and PIN1->TX at laser baud

  if (debugMode && debugVerbose) {
    // print startTime for debuging
    Serial.println("--Laser-- Start Time: " + String(startTime) + " microseconds");
  }
}

//----MAIN LOOP----
void loop() {

  // set the current loop time (microseconds) at the begining of the main loop
  currentLoopTime = micros();

  // mock laser response from command process
  mockInternalLaserCommand = mockReadCommandFromGalileo();
 
  mockLaserResponseToGalileo = mockConstructLaserResponse(mockInternalLaserCommand);
  mockSendResponseToGalileo(mockLaserResponseToGalileo);

  if (laserEmission) {
    analogWrite(laserPin, (int)((laserCurrent / (double)100.0) * (int)255));
  } else {
    analogWrite(laserPin, 0);
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
  int spaceIndex;

  command.erase(command.begin() + command.find('\r'));

  spaceIndex = command.find(" ");

  commandType = command.substr(0, spaceIndex);

  return commandType;

}

std::string findCommandArg(std::string command) {
  std::string commandArg = "";
  int spaceIndex = -1;

  spaceIndex = command.find(" ");

  if (spaceIndex != -1) {
    commandArg = command.substr(spaceIndex + 1);
  }

  return commandArg;
}

//----MOCK LASER FUNCTIONS----

std::string mockReadCommandFromGalileo() {
  std::string command = "";

  if (debugMode) {
    command = serialRead(&Serial, '\r');
  } else {
    command = serialRead(&Serial1, '\r');
  }

  // ensure a command has been received
  if (!command.empty()) {

    // debug new command back to user:
    if (debugMode && debugVerbose) {
      Serial.print("--Mock Laser-- Command received from Galileo: ");
      Serial.print(command.c_str());
      Serial.print('\n');
    }

  }

  return command;
}

// mock function to simulate the construction of laser command responses
std::string mockConstructLaserResponse(std::string command) {

  std::string commandType = "";
  std::string commandArg = "";
  std::string processedCommand = "";
  double commandArgValue;

  std::string response = "";

  if (!command.empty()) {

    processedCommand = command;

    // separate command and argument
    commandType = findCommandType(processedCommand);
    commandArg = findCommandArg(processedCommand);
    commandArgValue = atof(commandArg.c_str());

    if (debugMode && debugVerbose) {
      Serial.println(("--Mock Laser-- Command Type: " + commandType).c_str());
      Serial.println(("--Mock Laser-- Command Argument: " + commandArg).c_str());
    }

    // mocking of the response creation
    if (commandType == "SDC") {
      if ((commandArgValue >= 10.0 && commandArgValue <= 100.0) || commandArgValue == 0.0) {
        laserCurrent = commandArgValue;
        response = "SDC: " + floatToString(laserCurrent);
      } else {
        response = "ERR: Out of Range";
      }
    } else if (commandType == "RCS") {
      response = "RCS: " + floatToString(laserCurrent);
    } else if (commandType == "RNC") {
      response = "RNC: 12.0"; // minimum current setpoint at 12%
    } else if (commandType == "RDC") {
      response = "RDC: 4.5";
    } else if (commandType == "ROP") {
      response = "ROP: 96.1"; // TODO impliment off and low states
    } else if (commandType == "RFV") {
      response = "RFV: 1.0.103";
    } else if (commandType == "RCT") {
      response = "RCT: 34.5";
    } else if (commandType == "STA") {
      response = "STA: 4100"; // TODO impliment mock laser state
    } else if (commandType == "EMON") {
      if (laserKeyswitchON) {
        laserEmission = true;
        response = "EMON";
      } else {
        response = "ERR: Keyswitch in remote";
      }
    } else if (commandType == "EMOFF") {
      if (laserKeyswitchON) {
        laserEmission = false;
        response = "EMOFF";
      } else {
        response = "ERR: Keyswitch in remote";
      }
    } else if (commandType == "EMOD") {
      if (laserEmission) {
        response = "ERR: Emission is on";
      } else {
        laserModulation = true;
        response = "EMOD";
      }
    } else if (commandType == "DMOD") {
      if (laserEmission) {
        response = "ERR: Emission is on";
      } else {
        laserModulation = false;
        response = "DMOD";
      }
    } else if (commandType == "EEC") {
      if (laserEmission) {
        response = "ERR: Emission is on";
      } else {
        laserExternalControl = true;
        response = "EEC";
      }
    } else if (commandType == "DEC") {
      if (laserEmission) {
        response = "ERR: Emission is on";
      } else {
        laserExternalControl = false;
        response = "DEC";
      }
    } else if (commandType == "RERR") {
      response = "RERR";
    } else if (commandType == "ABN") {
      response = "ABN";
    } else if (commandType == "ABF") {
      response = "ABF";
    } else if (commandType == "EEABC") {
      response = "EEABC";
    } else if (commandType == "DEABC") {
      response = "DEABC";
    } else if (commandType == "SFWS") {
      if (commandArgValue > 5.0) {
        response = "ERR: Out of Range";
      } else {
        response = "SFWS: " + floatToString(commandArgValue); // must be a multiple of 0.2 and at a maximum of 5.0
      }
    } else if (commandType == "RFWS") {
      response = "RFWS: 3.4";
    } else if (commandType == "RMEC") {
      response = "RMEC: 0";
    } else {
      response = "BCMD " + processedCommand;
    }

    response += '\r';

    if (debugMode && debugVerbose) {
      Serial.print("--Mock Laser-- Response constructed from command: command -> ");
      Serial.print(command.c_str());
      Serial.print(" | response -> ");
      Serial.println(response.c_str());
    }

  }

  return response;

}

void mockSendResponseToGalileo(std::string response) {

  if (response != "") {

    if (debugMode && debugVerbose) {
      Serial.print("--Mock Laser-- Sending response to Galileo: ");
      Serial.print(response.c_str());
      Serial.print('\n');
    }

    if (debugMode) {
      Serial.print(response.c_str());
    } else {
      Serial1.print(response.c_str());
    }
    
  }

}
