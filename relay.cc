#include "Relay.h"

std::string Relay::convertPin(std::vector<NetlistData> input, int index) {
    //Declare and initialize necessary variables
    std::string pin;
    int pinNumber;
    std::string name = input[index].name[0];
    char lastChar = name.back();

    //Finds the pin number based on the last character of the name
    if (isdigit(lastChar)) {
        //Converts the pin number to an integer
        pinNumber = lastChar - '0';

        //Checks if the pin number is a double digit number
        lastChar = name[name.size() - 2];
        if (isdigit(lastChar)) {
            //Adjusts the pin number to the correct value
            pinNumber += (lastChar - '0') * 10;
        }

        //Converts the pin number to a string
        pin = "DUT_PIN_" + std::to_string(pinNumber);
    }
    else {
        //If the end of the name is not a number, it is set to the deafult pin
        pin = "DUT_PIN_1";
    }

    //Returns the pin number
    return pin;
}

std::string Relay::convertEnum(std::vector<NetlistData> input, int index) {
    //Declare and initialize necessary variables
    std::string enumeration;
    std::string name = input[index].name[0];
    char lastChar = name.back();
    int tracker = 0;
    std::vector<int> nameBreaks;

    //Finds if the last character of the name is a number
    if (isdigit(lastChar)) {
        //Finds all instances of the underscore in the name
        while ((tracker = name.find("_", tracker + 1)) != std::string::npos) {
            nameBreaks.push_back(tracker);
        }
        //Replaces the middle information with CIB
        enumeration = name.substr(0, nameBreaks[0] + 1) + "CIB" + name.substr(nameBreaks.back(), name.back() - nameBreaks.back() + 1);
    }
    else {
        //If the end of the name is not a number, the final word is replaced with CIB unless it is a gnd
        if (name.find("SYSGND") != std::string::npos) {
            enumeration = name.substr(0, name.find("SYSGND")) + "SYSGND";
        }
        else {
            //Finds all instances of the underscore in the name
            while ((tracker = name.find("_", tracker + 1)) != std::string::npos) {
                nameBreaks.push_back(tracker);
            }
            //Replaces the final word with CIB
            enumeration = name.substr(0, nameBreaks.back() + 1) + "CIB";
        }

    }

    //Returns the enumeration
    return enumeration;
}

int Relay::convertChain(std::string boardType) {
    //Declare and initialize necessary variables
    int chainNumber = 0;

    //Determines the chain number based on the board type
    if (boardType == "Power")
        chainNumber = 0;
    else if (boardType == "Precision")
        chainNumber = 1;

    //Returns the chain number
    return chainNumber;
}

int Relay::convertDevice(std::vector<NetlistData> input, int index, std::string boardType, std::vector<IOData> expanders) {
    //Declare and initialize necessary variables
    int deviceNumber = 0;

    //Determines which line of the netlist data has the device number
    for (int i = 0; i < input[index].device.size(); i++) {
        //Find the index of the connection to the IO expander
        if (input[index].device[i] == "MAX6957ATL_TQFN-40-MAX6957ATL+") {
            //Loops through each IO expander
            for (int j = 0; j < expanders.size(); j++) {
                //Checks which IO expander is associated with this relay
                if (expanders[j].identifier == input[index].identifier[i]) {
                    deviceNumber = expanders[j].position;
                    break;
                }
            }
            break;
        }
    }

    //Adjusts the device number based on the board type
    if (boardType == "Power")
        deviceNumber += 21;
    else if (boardType == "Precision") {
        deviceNumber += 18;
    }

    //Returns the device number
    return deviceNumber;
}

std::string Relay::convertPort(std::vector<NetlistData> input, int index) {
    //Declare and initialize necessary variables
    std::string portIdentifier;
    int connectionNumber = 0;
    std::string portNumber;

    //Determines which line of the netlist data has the port connection
    for (int i = 0; i < input[index].device.size(); i++) {
        //Find the index of the connection to the IO expander
        if (input[index].device[i] == "MAX6957ATL_TQFN-40-MAX6957ATL+") {
            //Assigns the port identifier based on connection to the IO expander
            portIdentifier = input[index].connection[i];
            break;
        }
    }

    //Converts the port identifier to the corresponding connection number
    connectionNumber = stoi(portIdentifier);

    //Determines the port number based on the connection number and returns it
    switch (connectionNumber) {
    case 1:
        portNumber = "P8";
        break;
    case 2:
        portNumber = "P12";
        break;
    case 3:
        portNumber = "P9";
        break;
    case 4:
        portNumber = "P13";
        break;
    case 5:
        portNumber = "P10";
        break;
    case 6:
        portNumber = "P14";
        break;
    case 7:
        portNumber = "P11";
        break;
    case 8:
        portNumber = "P15";
        break;
    case 9:
        portNumber = "P16";
        break;
    case 10:
        portNumber = "P17";
        break;
    case 12:
        portNumber = "P18";
        break;
    case 13:
        portNumber = "P19";
        break;
    case 14:
        portNumber = "P20";
        break;
    case 15:
        portNumber = "P21";
        break;
    case 16:
        portNumber = "P22";
        break;
    case 17:
        portNumber = "P23";
        break;
    case 18:
        portNumber = "P24";
        break;
    case 19:
        portNumber = "P25";
        break;
    case 21:
        portNumber = "P26";
        break;
    case 22:
        portNumber = "P27";
        break;
    case 23:
        portNumber = "P28";
        break;
    case 24:
        portNumber = "P7";
        break;
    case 25:
        portNumber = "P29";
        break;
    case 26:
        portNumber = "P6";
        break;
    case 27:
        portNumber = "P30";
        break;
    case 28:
        portNumber = "P5";
        break;
    case 29:
        portNumber = "P31";
        break;
    case 30:
        portNumber = "P4";
        break;
    }
    return portNumber;
}

std::string Relay::convertDriveStrength(std::vector<NetlistData> input, int index) {
    //Declare and initialize necessary variable, relays are set to 10.5 mA by default
    std::string driveStrength = "CONFIG_LED_10p5_mA";

    //Checks each identifier for the relay for non default cases
    for (int i = 0; i < input[index].identifier.size(); i++) {
        //Checks if the identifier specifies the relay as normally closed
        if (input[index].identifier[i].find("_NC_") != std::string::npos) {
            driveStrength = "CONFIG_OUT";
            break;
        }
        //Checks if the name specifies the relay as an enable
        else if (input[index].name[i].find("_EN") != std::string::npos) {
            driveStrength = "CONFIG_OUT";
            break;
        }
        //Checks if the relay type will change the drive strength from the default
        else if (input[index].device[i].find("120_1_A_RELAY-4P-120-1-A-5/1") != std::string::npos) {
            driveStrength = "CONFIG_LED_21p0_mA";
            break;
        }
    }

    return driveStrength;
}

std::string Relay::convertNormalState(std::vector<NetlistData> input, int index) {
    //Declare and initialize necessary variables, relays are normally open by default
    int tracker;
    std::string normalState = "RELAY_NO";

    //Checks if any of the identifiers specify the relay is normally closed
    for (int i = 0; i < input[index].identifier.size(); i++) {
        //Checks if the identifier specifies the relay as normally closed
        if (input[index].identifier[i].find("_NC_") != std::string::npos) {
            normalState = "RELAY_NC";
            break;
        }
    }

    //Returns the normal state of the relay
    return normalState;
}

void Relay::relayConvert(std::vector<NetlistData> input, std::vector<RelayData>& relay, int relayAmount, std::string boardType, std::vector<IOData> expanders) {
    //Declare and initialize necessary variables
    RelayData temp;
    temp = RelayData();

    //Fills in all relay data
    for (int i = 0; i < relayAmount; i++) {
        //Finds the interpreter based relay data and temporarily stores it
        temp.pin = convertPin(input, i);
        std::cout << "1.." << std::endl;
        temp.enumeration = convertEnum(input, i);
        std::cout << "2.." << std::endl;
        temp.chainNumber = convertChain(boardType);
        std::cout << "3.." << std::endl;
        temp.deviceNumber = convertDevice(input, i, boardType, expanders);
        std::cout << "4.." << std::endl;
        temp.portNumber = convertPort(input, i);
        std::cout << "5.." << std::endl;
        temp.driveStrength = convertDriveStrength(input, i);
        std::cout << "6.." << std::endl;
        temp.normalState = convertNormalState(input, i);
        std::cout << "7.." << std::endl;
        temp.relayName = input[i].name[0];

        //Fills in a line of relay data
        relay.push_back(temp);
    }


    //Returns to main
    return;
}
