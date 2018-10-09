#include <math.h>
#include <string>

//----SETTINGS----
const bool debugMode = true; // set if you want to debug through a serial console

//----VARIABLES----
std::string inputCommand = ""; // a string to hold the command from the user
std::string outputCommand = ""; // a string to hold the command going to the laser
std::string responseFromLaser = ""; // recieved laser response
std::string responseToUser = ""; // processed laser response

//----TIMING VARIABLES----
unsigned long startTime;
unsigned long currentLoopTime;
unsigned long diff1Time;
unsigned long diff2Time;

//----SETUP----
void setup() {

  // get start time (microseconds) from the time the Calileo was booted:
  startTime = micros();

  // initiate Serials
  Serial.begin(115200); // Serial console debugger
  Serial1.begin(57600); // Galileo PIN0->RX and PIN1->TX at laser baud

  if (debugMode) {
    // print startTime for debuging
    Serial.println("--Galileo-- Start Time: " + String(startTime) + " microseconds");
  }
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

  // response from laser
  responseFromLaser = readResponseFromLaser();
  responseToUser = processResonseFromLaser(responseFromLaser);
  sendResponseToUser(responseToUser);

}

//----UTILITY FUNCTIONS----

std::string serialRead(Stream *serialX, char endChar) {
  std::string serialString;
  char buff = '\0';
  
  for(int i = 0; serialX->available() > 0 && buff != endChar; i++) {
    for (int n = 0; buff != endChar; n++) {
        buff = serialX->read();
        serialString += buff;
    }
  }

  return serialString;
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

//----COMMAND UTILITY FUNCTIONS----

// returns the command type
// command format: "SDC 34.2"
std::string findCommandType(std::string command) {

  std::string commandType = "";
  int spaceIndex = -1;

  spaceIndex = command.find(" ");
  commandType = command.substr(0, spaceIndex);

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
  // TODO detect if command is to galileo or laser

  bool galileoCommand = false;

  std::string commandType = findCommandType(command);

  if (commandType == "LSRFN" || commandType == "PING") {
    galileoCommand = true;
  }
  
  return galileoCommand;
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
      Serial.print(response.c_str());
    }

    if (debugMode) {
      diff2Time = micros();
      Serial.print("--Galileo-- Sending response to user took: " + (String)(diff2Time-diff1Time) + " us\n");
    }

  }

}
