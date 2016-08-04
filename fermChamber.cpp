using namespace std;

#include "fermChamber.h"

// Public Method Definitions

// Constructor
fermChamber::fermChamber(void)
{
    // Initialise the beer name to a known value
    beerName = "You Gotta Set a Beer Name";

    // Set the sensor paths empty. They will remain this way if there is no sensor. Allows to determine if sensor doesn't exist.
    beerSensorPath = "";
    fridgeSensorPath = "";
    ambientSensorPath = "";
    
    // Set the sensors to false and change as sensors read in from .ini
    hasBeerSensor = false;
    hasFridgeSensor = false;
    hasAmbientSensor = false;
    
    // The temperature values are initialised to 0.
    currentBeerTemp = 0;
    currentFridgeTemp = 0;
    currentAmbientTemp = 0;
    previousBeerTemp = 0;
    previousFridgeTemp = 0;
    previousAmbientTemp = 0;
    setBeerTemp = 0;
    setFridgeTemp = 0;

    // Chamber door values are initalised to false.
    hasDoorSwitch = false;
    doorOpen = false;
}

string fermChamber::getBeerName(void)
{
    return beerName;
}
void fermChamber::setBeerName(string newName)
{
    beerName = newName;
}

string fermChamber::getChamberBeerSensorPath(void)
{
    return beerSensorPath;
}
void fermChamber::setChamberSensorPath(string path, int id)
{
    // For id: 0 = beerSensor, 1 = fridgeSensor, 2 = ambientSensor. Allows for additional sensors to be added in the future.
    switch (id)
    {
    case 0:
        beerSensorPath = path;
        break;
    case 1:
        fridgeSensorPath = path;
        break;
    case 2:
        ambientSensorPath = path;
        break;
    default:
        // Should never end up here. Can add error handling if desired.
        break;
    }
}

string fermChamber::getChamberFridgeSensorPath(void)
{
    return fridgeSensorPath;
}
string fermChamber::getChamberAmbientSensorPath(void)
{
    return ambientSensorPath;
}

float fermChamber::getCurrentBeerTemp(void)
{
    return currentBeerTemp;
}
float fermChamber::getCurrentFridgeTemp(void)
{
    return currentFridgeTemp;
}
float fermChamber::getCurrentAmbientTemp(void)
{
    return currentAmbientTemp;
}
float fermChamber::getPreviousBeerTemp(void)
{
    return previousBeerTemp;
}
float fermChamber::getPreviousFridgeTemp(void)
{
    return previousFridgeTemp;
}
float fermChamber::getPreviousAmbientTemp(void)
{
    return previousAmbientTemp;
}

void fermChamber::changeSetBeerTemp(float newTemp)
{
    setBeerTemp = newTemp;
}
void fermChamber::changeSetFridgeTemp(float newTemp)
{
    setFridgeTemp = newTemp;
}
float fermChamber::getSetBeerTemp(void)
{
    return setBeerTemp;
}
float fermChamber::getSetFridgeTemp(void)
{
    return setFridgeTemp;
}

void fermChamber::setCurrentBeerTemp(float newTemp)
{
    setNewPreviousBeerTemp();
    currentBeerTemp = newTemp;
}
void fermChamber::setCurrentFridgeTemp(float newTemp)
{
    setNewPreviousFridgeTemp();
    currentFridgeTemp = newTemp;
}
void fermChamber::setCurrentAmbientTemp(float newTemp)
{
    setNewPreviousAmbientTemp();
    currentAmbientTemp = newTemp;
}
void fermChamber::setNewPreviousBeerTemp(void)
{
    previousBeerTemp = currentBeerTemp;
}
void fermChamber::setNewPreviousFridgeTemp(void)
{
    previousFridgeTemp = currentFridgeTemp;
}
void fermChamber::setNewPreviousAmbientTemp(void)
{
    previousAmbientTemp = currentAmbientTemp;
}

void fermChamber::setHasBeerSensor(bool setValue)
{
    hasBeerSensor = setValue;
}
void fermChamber::setHasFridgeSensor(bool setValue)
{
    hasFridgeSensor = setValue;
}
void fermChamber::setHasAmbientSensor(bool setValue)
{
    hasAmbientSensor = setValue;
}
bool fermChamber::getHasBeerSensor(void)
{
    return hasBeerSensor;
}
bool fermChamber::getHasFridgeSensor(void)
{
    return hasFridgeSensor;
}
bool fermChamber::getHasAmbientSensor(void)
{
    return hasAmbientSensor;
}
