#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

struct RelayData{
    std::string pin;
    std::string enumeration;
    int chainNumber; //This will need to be changed based on user input, Power = 0, Precision = 1
    int deviceNumber;
    std::string portNumber;
    std::string driveStrength;
    std::string normalState;
    int minOpenTime = 0; //This and all the other time values are hard coded in based on what numbers have worked for TI
    int typOpenTime = 60;
    int maxOpenTime = 200;
    int minCloseTime = 0;
    int typCloseTime = 100;
    int maxCloseTime = 500;
    std::string relayName;
};  //Holds all final relay data

struct NetlistData {
    std::vector<std::string> name;
    std::vector<std::string> identifier;
    std::vector<std::string> connection;
    std::vector<std::string> device;
};  //Holds all netlist input data

struct BillData{
  std::string item;
  std::string quantity;
  std::string manufacturer;
  std::string partNumber;
  std::vector<std::string> identifier;
  std::string description;
  std::string dni;
  std::string status;
  std::string maxTemp;
};  //Holds all BOM input data

struct IOData{
    std::string identifier;
    int position;
};  //Holds all IO expander data

struct ModelData{
  std::string relayModel;
  std::string driveStrength;
};  //Holds all information directly tied to the relay model
