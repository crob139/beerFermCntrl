#include <string>

class fermChamber
{
    // Variable Declarations
private:
    // This is the name of the beer that the user can specify. Will be shown on web interface.
    string beerName;

    // Hold's path to the file containing the sensor's temp data. If NULL, sensor doesn't exist.
    string beerSensorPath, fridgeSensorPath, ambientSensorPath;

    // Temperature Data
    float currentBeerTemp, currentFridgeTemp, currentAmbientTemp;      // Current/most recent temperature data reading values.
    float previousBeerTemp, previousFridgeTemp, previousAmbientTemp;   // Previous temperature data reading values.
    float setBeerTemp, setFridgeTemp;                                  // Set values. Ambient can't be set so has no variable.

    /*** !!!FOR FUTURE!!! ***/
    // Chamber door switch variables.
    bool hasDoorSwitch; // True = Chamber has door switch. False = Chamber doesn't have door switch.
    bool doorOpen;      // True = Chamber door is open. False = Chamber door is closed.

    // Method Declarations
public:
    fermChamber(void);

    string getBeerName(void);
    void setBeerName(string newName);

    string getChamberBeerSensorPath(void);
    string getChamberFridgeSensorPath(void);
    string getChamberAmbientSensorPath(void);
    void setChamberSensorPath(string path, int id);

    float getCurrentBeerTemp(void);
    float getCurrentFridgeTemp(void);
    float getCurrentAmbientTemp(void);
    float getPreviousBeerTemp(void);
    float getPreviousFridgeTemp(void);
    float getPreviousAmbientTemp(void);
    void setCurrentBeerTemp(float newTemp);
    void setCurrentFridgeTemp(float newTemp);
    void setCurrentAmbientTemp(float newTemp);

    void changeSetBeerTemp(float newTemp);
    void changeSetFridgeTemp(float newTemp);
    float getSetBeerTemp(void);
    float getSetFridgeTemp(void);

private:
    void setNewPreviousBeerTemp(void);
    void setNewPreviousFridgeTemp(void);
    void setNewPreviousAmbientTemp(void);
};
