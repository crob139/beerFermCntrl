#include "fermChamber.h"

#define NUM_OF_FERM_CHAMBERS 1

// This is used to determine how many digits are looked for after sensor type.
// i.e. if the total number of chambers is 11 it will look for 2 digits after sensor type in .ini (e.g. beer11)
#if NUM_OF_FERM_CHAMBERS > 999
    #define NUM_DIGITS_FERM_CHAMBERS 4
#elif NUM_OF_FERM_CHAMBERS > 99
    #define NUM_DIGITS_FERM_CHAMBERS 3
#elif NUM_OF_FERM_CHAMBERS > 9
    #define NUM_DIGITS_FERM_CHAMBERS 2
#else
    #define NUM_DIGITS_FERM_CHAMBERS 1
#endif

#define BEER "beer"
#define CHAMBER "chamber"
#define AMBIENT "ambient"

void init(bool &fahrenheit, fermChamber (&chambers)[NUM_OF_FERM_CHAMBERS]);
void getBeerTempData(bool fahrenheit, fermChamber &chamberToUpdate);
void getFridgeTempData(bool fahrenheit, fermChamber &chamberToUpdate);
void getAmbientTempData(bool fahrenheit, fermChamber &chamberToUpdate);
int strToInt(string &strToConvert, string type);
