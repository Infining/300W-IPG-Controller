// TODO separate transformation from processing
// TODO refactor naming conventions
// TODO check for command validity

#include "avdweb_VirtualDelay.h" // works with galileo
#include <math.h>

//----SETTINGS----
const bool mockMode = true; // set if program is running in mock mode
const bool debugMode = false; // set if you want to debug through a serial console
const bool pingLaser = false; // check if laser is connected

//----VARIABLES----
String inputCommand = ""; // a string to hold the command from the user
String outputCommand = ""; // a string to hold the command going to the laser
String responseFromLaser = ""; // recieved laser response
String responseToUser = ""; // processed laser response

boolean laserConnectedLast = false;
boolean laserConnectedNow = false;

//----DELAYS----
VirtualDelay laserPingDelay(micros);
unsigned long laserPingDelayInterval = 5000e3; // make sure intervals are in microseconds
VirtualDelay laserPongDelay(micros);
unsigned long laserPongDelayInterval = 5e3; // make sure intervals are in microseconds

//----PINS----
const int remoteStartPB = 4;

unsigned long startTime = -1;
unsigned long currentLoopTime = -1;

//----MOCK VARIABLES----
String mockInternalLaserCommand = ""; // a mock string that holds the command received by the laser
String mockLaserResponseToGalileo = ""; // a mock stirng to hold the response string internal to the laser

//----SETUP----
void setup() {

  // get start time (microseconds) from the time the Calileo was booted:
  startTime = micros();

  // initiate Serials
  Serial.begin(115200); // Serial console debugger
  Serial1.begin(57600); // Galileo PIN0->RX and PIN1->TX at laser baud

  // mock laser mode
  if (mockMode) {
    Serial2.begin(57600); // Simulated laser RS-232 serial
  }

  if (debugMode) {
    // print startTime for debuging
    Serial.println("--Galileo-- Start Time: " + String(startTime) + " microseconds");
  }

  // set direction of pins
  pinMode(remoteStartPB, OUTPUT);
  digitalWrite(remoteStartPB, LOW);

}

//----MAIN LOOP----
void loop() {

  // set the current loop time (microseconds) at the begining of the main loop
  currentLoopTime = micros();

  // command to laser
  inputCommand = readCommandFromUser();
  if (isCommandGalileo(inputCommand)) {
    // process command to the gailieo
    Serial.println("Command is Galileo type");
  } else {
    outputCommand = processToLaserCommand(inputCommand);
    sendCommandToLaser(outputCommand);
  }

  if (mockMode) {
    // mock laser response from command process
    mockInternalLaserCommand = mockReadCommandFromGalileo();
    mockLaserResponseToGalileo = mockConstructLaserResponse(mockInternalLaserCommand);
    mockSendResponseToGalileo(mockLaserResponseToGalileo);
  }

  // response from laser
  responseFromLaser = readResponseFromLaser();
  responseToUser = processResonseFromLaser(responseFromLaser);
  sendResponseToUser(responseToUser);

}

//----UTILITY FUNCTIONS----

String stringLiteralDisplayTransform(String str) {

  // TODO add more special characters

  str.replace("\n", "\\n");
  str.replace("\r", "\\r");

  return str;
}

float stringToFloat(String str) {

  char buffer[10];
  str.toCharArray(buffer, 10);

  return atof(buffer);
}

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

String floatToString(float val) {
  char buffer[10];
  dtostrf(val, numberOfDigits(val), 1, buffer);
  return buffer;
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

String scrubSerialMessage(String message) {

  // remove newline and carriage return characters:
  message.replace("\n", "");
  message.replace("\r", "");

  return message;
}

//----COMMAND UTILITY FUNCTIONS----

// returns the command type
// command format: "SDC 34.2"
String findCommandType(String command) {

  String commandType = "";
  int spaceIndex = -1;

  spaceIndex = command.indexOf(" ");
  commandType = command.substring(0, spaceIndex);

  return commandType;

}

String findCommandArg(String command) {
  String commandArg = "";
  int spaceIndex = -1;

  spaceIndex = command.indexOf(" ");

  if (spaceIndex != -1) {
    commandArg = command.substring(spaceIndex + 1);
  }

  return commandArg;
}

//----GALILEO COMM FUNCTIONS----

void pingLaserConnection() {

  // send dummy command to laser
  Serial1.print("DUMMY\r");

}

String readCommandFromUser() {

  String command = "";

  command = Serial.readStringUntil('\r');

  // ensure a command has been received
  if (command != "") {

    // debug new command back to user:
    if (debugMode) {
      Serial.print("--Galileo-- Command received from user: ");
      Serial.print(stringLiteralDisplayTransform(command));
      Serial.print('\n');
    }

  }

  return command;

}

bool isCommandGalileo(String command) {
  // TODO detect if command is to galileo or laser

  bool galileoCommand = false;

  String commandType = findCommandType(command);

  if (commandType == "LSRFN" || commandType == "PING") {
    galileoCommand = true;
  }
  
  return galileoCommand;
}

// processes the command from the user for forwarding to the laser
String processToLaserCommand(String command) {

  String processedCommand = "";

  // ensure a command has been received
  if (!(command == "")) {

    processedCommand = command;

    // remove newline and carriage return characters:
    scrubSerialMessage(processedCommand);

    // append carriage return to end of command, required by the laser
    processedCommand.concat('\r');

    // TODO do more processing here
    // TODO process matlab (user) ping


    if (debugMode) {
      Serial.print("--Galileo-- Processing command: from -> ");
      Serial.print(stringLiteralDisplayTransform(command));
      Serial.print(" | to -> ");
      Serial.print(stringLiteralDisplayTransform(processedCommand));
      Serial.print('\n');
    }

  }

  return processedCommand;

}

void sendCommandToLaser(String command) {

  if (command != "") {

    if (debugMode) {
      Serial.print("--Galileo-- Sending command to laser: ");
      Serial.print(stringLiteralDisplayTransform(command));
      Serial.print('\n');
    }

    Serial1.print(command);

  }

}

String readResponseFromLaser() {

  String response = "";

  response = Serial1.readStringUntil('\r');

  // ensure a response has been received
  if (response != "") {

    if (debugMode) {
      Serial.print("--Galileo-- Response received from laser: ");
      Serial.print(stringLiteralDisplayTransform(response));
      Serial.print('\n');
    }

  }

  return response;

}

// processes the response from the laser to forward to the user
String processResonseFromLaser(String response) {

  String processedResponse = "";

  if (response != "") {

    // TODO do some processing here
    processedResponse = response;

    processedResponse.concat('\r');

    if (debugMode) {
      Serial.print("--Galileo-- Processing response: from -> ");
      Serial.print(stringLiteralDisplayTransform(response));
      Serial.print(" | to -> ");
      Serial.print(stringLiteralDisplayTransform(processedResponse));
      Serial.print('\n');
    }

  }

  return processedResponse;

}

void sendResponseToUser(String response) {

  if (response != "") {

    if (debugMode) {
      Serial.print("--Galileo-- Sending response to user: ");
      Serial.print(stringLiteralDisplayTransform(response));
      Serial.print('\n');
    }

    if (debugMode) {
      Serial.println(response);
    } else {
      Serial.print(response);
    }

  }

}

//----MOCK LASER FUNCTIONS----

String mockReadCommandFromGalileo() {
  String command = "";

  command = Serial2.readStringUntil('\r');

  // ensure a command has been received
  if (command != "") {

    // debug new command back to user:
    if (debugMode) {
      Serial.print("--Mock Laser-- Command received from Galileo: ");
      Serial.print(stringLiteralDisplayTransform(command));
      Serial.print('\n');
    }

  }

  return command;
}

// mock function to simulate the construction of laser command responses
String mockConstructLaserResponse(String command) {

  String commandType = "";
  String commandArg = "";
  String processedCommand = "";
  float commandArgValue;

  String response = "";

  if (command != "") {

    processedCommand = command;

    // remove newline and carriage return characters:
    scrubSerialMessage(processedCommand);

    // separate command and argument
    commandType = findCommandType(processedCommand);
    commandArg = findCommandArg(processedCommand);
    commandArgValue = stringToFloat(commandArg);

    if (debugMode) {
      Serial.println("--Mock Laser-- Command Type: " + commandType);
      Serial.println("--Mock Laser-- Command Argument: " + commandArg);
    }

    // mocking of the response creation
    if (commandType == "SDC") {
      if (commandArgValue >= 10.0 && commandArgValue <= 100.0) {
        response = "SDC: " + floatToString(commandArgValue);
      } else {
        response = "ERR: Out of Range";
      }
    } else if (commandType == "RCS") {
      response = "RCS: " + floatToString(random(120, 1000) / 10.0); // generate a random float within range
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
      response = "EMON"; // TODO impliment keyswitch in remote
    } else if (commandType == "EMOFF") {
      response = "EMOFF"; // TODO impliment keyswitch in remote
    } else if (commandType == "EMOD") {
      response = "EMOD"; // TODO impliment emission is on
    } else if (commandType == "DMOD") {
      response = "DMOD"; // TODO impliment emission is on
    } else if (commandType == "EEC") {
      response = "EEC"; // TODO implement emission is on
    } else if (commandType == "DEC") {
      response = "DEC"; // TODO implement emission is on
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
      response = "BCMD";
    }

    response += '\r';

    if (debugMode) {
      Serial.print("--Mock Laser-- Response constructed from command: command -> ");
      Serial.print(stringLiteralDisplayTransform(command));
      Serial.print(" | response -> ");
      Serial.println(stringLiteralDisplayTransform(response));
    }

  }

  return response;

}

void mockSendResponseToGalileo(String response) {

  if (response != "") {

    if (debugMode) {
      Serial.print("--Mock Laser-- Sending response to Galileo: ");
      Serial.print(stringLiteralDisplayTransform(response));
      Serial.print('\n');
    }

    Serial2.print(response);
  }

}
