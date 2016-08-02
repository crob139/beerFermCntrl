#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

#include "main.h"

int main(void)
{
    // Main Variable Declarations
    bool fahrenheit = true; // Determines whether celsius or fahrenheit is used. True = Use fahrenheit. False = Use celsius.
    bool mode = true; // Determines what temperature to control. True = beer temperature. False = fridge temperature.

    fermChamber chambers[NUM_OF_FERM_CHAMBERS]; // Declare array of fermentation chambers.

    init(fahrenheit, chambers); // Do all initialisation. Read .ini file and setup fermentation chambers.
    
    getTempData(fahrenheit, chambers);

    cout << "Beer Temp: " << chambers[0].getCurrentBeerTemp() << endl;
    cout << "Fridge Temp: " << chambers[0].getCurrentFridgeTemp() << endl;
    
    return 0;
}

void init(bool &fahrenheit, fermChamber (&chambers)[NUM_OF_FERM_CHAMBERS])
{
    ifstream configFile;
    string line, tempCompare;
    size_t found;
    bool skip = false;
    string temperatureValuePath = "/sys/bus/w1/devices/";

    configFile.open("main.ini");
    if (configFile.is_open())
    {
        // Read in config settings from .ini file
        while (getline(configFile, line))
        {
            skip = false;
            for (int i = 0; i < line.length(); ++i)
            {
                if (line[i] == '#')
                {
                    skip = true;
                    break;
                }
                else if (line[i] != ' ')
                {
                    break;
                }
            }

            if (!skip)
            {
                // Convert the entire line to lower case
                for (string::size_type i = 0; i < line.length(); ++i)
                {
                    line[i] = tolower(line[i]);
                }

                // Get the beer temperature sensor serial number
                found = line.find(BEER);
                if (found != string::npos)
                {
                    for (int i = 0; i < NUM_OF_FERM_CHAMBERS; ++i)
                    {
                        if (strToInt(line, BEER) == i)
                        {
                            // n'th chamber's beer sensor
                            found = line.find("28");
                            chambers[i].setChamberSensorPath(temperatureValuePath + line.substr(found) + "/w1_slave", 0); // Beer Sensor Path
                            break;
                        }
                    }
                }

                // Get the chamber temperature sensor serial number
                found = line.find(CHAMBER);
                if (found != string::npos)
                {
                    for (int i = 0; i < NUM_OF_FERM_CHAMBERS; ++i)
                    {
                        if (strToInt(line, CHAMBER) == i)
                        {
                            // n'th chamber's chamber sensor
                            found = line.find("28");
                            chambers[i].setChamberSensorPath(temperatureValuePath + line.substr(found) + "/w1_slave", 1); // Chamber Sensor Path
                            break;
                        }
                    }
                }

                // Get the chamber temperature sensor serial number
                found = line.find(AMBIENT);
                if (found != string::npos)
                {
                    for (int i = 0; i < NUM_OF_FERM_CHAMBERS; ++i)
                    {
                        if (strToInt(line, AMBIENT) == i)
                        {
                            // n'th chamber's ambient sensor
                            found = line.find("28");
                            chambers[i].setChamberSensorPath(temperatureValuePath + line.substr(found) + "/w1_slave", 2); // Ambient Sensor Path
                            break;
                        }
                    }
                }

                // Determine if we are using celsius or fahrenheit
                found = line.find("fahrenheit");
                if (found != string::npos)
                {
                    found = line.find("=");
                    line = line.erase(0, found);
                    for (int i = 0; i < line.length(); ++i)
                    {
                        if (line[i] == 'n')
                        {
                            fahrenheit = false;
                            break;
                        }
                        else if (line[i] == 'y')
                        {
                            fahrenheit = true;
                            break;
                        }
                    }
                }
            }
        }
        configFile.close();
    }
    else
    {
        cout << "ERROR: Failed to open main.ini file!" << endl;
    }
}

void getTempData(bool fahrenheit, fermChamber (&chambers)[NUM_OF_FERM_CHAMBERS])
{
    ifstream configFile;
    string line;
    size_t found;

    // Get Beer Temperature Data
    configFile.open(chambers[0].getChamberBeerSensorPath().c_str());
    if (configFile.is_open())
    {
        while (getline(configFile, line))
        {
            found = line.find("t=");
            if (found != string::npos)
            {
                if (fahrenheit)
                {
                    chambers[0].setCurrentBeerTemp((((((float)(atoi((line.erase(0, (found+2))).c_str()))))*9)+160000)/5000);
                }
                else
                {
                    chambers[0].setCurrentBeerTemp(((float)(atoi((line.erase(0, (found+2))).c_str())))/1000);
                }
            }
        }
        configFile.close();
    }
    else
    {
        cout << "Failed to open beer temperature sensor data file!" << endl;
    }

    // Get Fridge Temperature Data
    configFile.open(chambers[0].getChamberFridgeSensorPath().c_str());
    if (configFile.is_open())
    {
        while (getline(configFile, line))
        {
            found = line.find("t=");
            if (found != string::npos)
            {
                if (fahrenheit)
                {
                    chambers[0].setCurrentFridgeTemp((((((float)(atoi((line.erase(0, (found+2))).c_str()))))*9)+160000)/5000);
                }
                else
                {
                    chambers[0].setCurrentFridgeTemp(((float)(atoi((line.erase(0, (found+2))).c_str())))/1000);
                }
            }
        }
        configFile.close();
    }
    else
    {
        cout << "Failed to open chamber temperature sensor data file!" << endl;
    }

    // Need to add ambient temperature data. Also need to allow for there being no sensor for each option.
    // Also need to throw error if no beer0 specified
}

int strToInt(string &strToConvert, string type)
{
    int convertedInt = 0, j = 0;
    int tempNum[NUM_DIGITS_FERM_CHAMBERS];

    for (int i = 0; i < strToConvert.length(); ++i)
    {
        // Check to see if we have 3 numbers after the key word. This puts a limit on chambers to 999.
        if (j == 3)
        {
            cout << "Too many chambers specified. Error in line of .ini here: " << strToConvert << endl;
            exit(EXIT_FAILURE);
        }

        // Check to see if the element in the string is a number.
        switch (strToConvert[type.length() + i])
        {
        case '0':
            tempNum[j] = 0;
            j++;
            continue;
        case '1':
            tempNum[j] = 1;
            j++;
            continue;
        case '2':
            tempNum[j] = 2;
            j++;
            continue;
        case '3':
            tempNum[j] = 3;
            j++;
            continue;
        case '4':
            tempNum[j] = 4;
            j++;
            continue;
        case '5':
            tempNum[j] = 5;
            j++;
            continue;
        case '6':
            tempNum[j] = 6;
            j++;
            continue;
        case '7':
            tempNum[j] = 7;
            j++;
            continue;
        case '8':
            tempNum[j] = 8;
            j++;
            continue;
        case '9':
            tempNum[j] = 9;
            j++;
            continue;
        default:
            if (j == 1)
            {
                return tempNum[0];
            }
            else if (j == 2)
            {
                return tempNum[0]*10 + tempNum[1];
            }
            else if (j == 3)
            {
                return tempNum[0]*100 + tempNum[1]*10 + tempNum[2];
            }
            else
            {
                cout << "ERROR: Converting char to int." << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
}
