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
    
    while (true)
    {
        for (int i = 0; i < NUM_OF_FERM_CHAMBERS; i++)
        {
            // Get beer temp and print it
            if (chambers[i].getHasBeerSensor())
            {
                getBeerTempData(fahrenheit, chambers[i]);
                cout << "Chamber " << i << " Beer Temp: " << chambers[i].getCurrentBeerTemp() << endl;
            }

            // Get chamber temp and print it
            if (chambers[i].getHasFridgeSensor())
            {
                getFridgeTempData(fahrenheit, chambers[i]);
                cout << "Chamber " << i << " Chamber Temp: " << chambers[i].getCurrentFridgeTemp() << endl;
            }

            // Get ambient temp and print it
            if (chambers[i].getHasAmbientSensor())
            {
                getAmbientTempData(fahrenheit, chambers[i]);
                cout << "Chamber " << i << " Ambient Temp: " << chambers[i].getCurrentAmbientTemp() << endl;
            }
        }
    }
    
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
                            chambers[i].setHasBeerSensor(true); // Set the chamber variable for checking if a beer sensor is available
                            break;
                        }
                    }
                    continue;
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
                            chambers[i].setHasFridgeSensor(true); // Set the chamber variable for checking if a chamber sensor is available
                            break;
                        }
                    }
                    continue;
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
                            chambers[i].setHasAmbientSensor(true); // Set the chamber variable for checking if an ambient sensor is available
                            break;
                        }
                    }
                    continue;
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
                    continue;
                }
            }
        }
        configFile.close();
    }
    else
    {
        cout << "ERROR: Failed to open main.ini file!" << endl;
    }
    
    for (int i = 0; i < NUM_OF_FERM_CHAMBERS; i++)
    {
        // Check
        if ((!chambers[i].getHasBeerSensor()) && (!chambers[i].getHasFridgeSensor()))
        {
            cout << "ERROR: Chamber " << i << " doesn't have a beer or chamber temp sensor specified." << endl;
            exit(EXIT_FAILURE);
        }
        
        cout << "CHAMBER " << i << endl;
        // Beer sensor serial
        if (chambers[i].getHasBeerSensor())
        {
            cout << "    Beer Sensor: " << chambers[i].getChamberBeerSensorPath() << endl;
        }
        else
        {
            cout << "    Beer Sensor: None" << endl;
        }
        // Chamber sensor serial
        if (chambers[i].getHasFridgeSensor())
        {
            cout << "    Chamber Sensor: " << chambers[i].getChamberFridgeSensorPath() << endl;
        }
        else
        {
            cout << "    Chamber Sensor: None" << endl;
        }
        // Ambient sensor serial
        if (chambers[i].getHasAmbientSensor())
        {
            cout << "    Ambient Sensor: " << chambers[i].getChamberAmbientSensorPath() << endl;
        }
        else
        {
            cout << "    Ambient Sensor: None" << endl;
        }
        
        cout << endl; // For formating
    }
}

void getBeerTempData(bool fahrenheit, fermChamber &chamberToUpdate)
{
    ifstream configFile;
    string line;
    size_t found;

    // Get Beer Temperature Data
    configFile.open(chamberToUpdate.getChamberBeerSensorPath().c_str());
    if (configFile.is_open())
    {
        while (getline(configFile, line))
        {
            found = line.find("t=");
            if (found != string::npos)
            {
                if (fahrenheit)
                {
                    chamberToUpdate.setCurrentBeerTemp((((((float)(atoi((line.erase(0, (found+2))).c_str()))))*9)+160000)/5000);
                }
                else
                {
                    chamberToUpdate.setCurrentBeerTemp(((float)(atoi((line.erase(0, (found+2))).c_str())))/1000);
                }
                break;
            }
        }
        configFile.close();
    }
    else
    {
        cout << "Failed to open beer temperature sensor data file!" << endl;
    }
}

void getFridgeTempData(bool fahrenheit, fermChamber &chamberToUpdate)
{
    ifstream configFile;
    string line;
    size_t found;

    // Get Fridge Temperature Data
    configFile.open(chamberToUpdate.getChamberFridgeSensorPath().c_str());
    if (configFile.is_open())
    {
        while (getline(configFile, line))
        {
            found = line.find("t=");
            if (found != string::npos)
            {
                if (fahrenheit)
                {
                    chamberToUpdate.setCurrentFridgeTemp((((((float)(atoi((line.erase(0, (found+2))).c_str()))))*9)+160000)/5000);
                }
                else
                {
                    chamberToUpdate.setCurrentFridgeTemp(((float)(atoi((line.erase(0, (found+2))).c_str())))/1000);
                }
                break;
            }
        }
        configFile.close();
    }
    else
    {
        cout << "Failed to open chamber temperature sensor data file!" << endl;
    }
}

void getAmbientTempData(bool fahrenheit, fermChamber &chamberToUpdate)
{
    ifstream configFile;
    string line;
    size_t found;

    // Get Ambient Temperature Data
    configFile.open(chamberToUpdate.getChamberAmbientSensorPath().c_str());
    if (configFile.is_open())
    {
        while (getline(configFile, line))
        {
            found = line.find("t=");
            if (found != string::npos)
            {
                if (fahrenheit)
                {
                    chamberToUpdate.setCurrentAmbientTemp((((((float)(atoi((line.erase(0, (found+2))).c_str()))))*9)+160000)/5000);
                }
                else
                {
                    chamberToUpdate.setCurrentAmbientTemp(((float)(atoi((line.erase(0, (found+2))).c_str())))/1000);
                }
                break;
            }
        }
        configFile.close();
    }
    else
    {
        cout << "Failed to open chamber temperature sensor data file!" << endl;
    }
}

int strToInt(string &strToConvert, string type)
{
    int convertedInt = 0, j = 0, retVal = 0;
    int tempNum[NUM_DIGITS_FERM_CHAMBERS];

    for (int i = 0; i < strToConvert.length(); ++i)
    {
        // Check to see if we have more than NUM_DIGITS_FERM_CHAMBERS after the key word.
        if (j > NUM_DIGITS_FERM_CHAMBERS)
        {
            cout << "Too many chambers specified vs NUM_DIGITS_FERM_CHAMBERS. Error in line of .ini here: " << strToConvert << endl;
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
            for (int k = 0; k < NUM_DIGITS_FERM_CHAMBERS; k++)
            {
                retVal += tempNum[k] * (10 ^ (NUM_DIGITS_FERM_CHAMBERS - k - 1));
            }
            
            // Checks
            if (retVal > NUM_OF_FERM_CHAMBERS)
            {
                cout << "ERROR: Sensor chambers specified in .ini file exceed NUM_OF_FERM_CHAMBERS!" << endl;
                exit(EXIT_FAILURE);
            }
            else if (retVal < 0)
            {
                cout << "ERROR: Incorrect .ini file format!" << endl;
                exit(EXIT_FAILURE);
            }
            
            return retVal;
        }
    }
}
