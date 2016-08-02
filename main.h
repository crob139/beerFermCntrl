#include "fermChamber.h"

#define NUM_OF_FERM_CHAMBERS 3

#if NUM_OF_FERM_CHAMBERS > 9
    #define NUM_DIGITS_FERM_CHAMBERS 2
#elif NUM_OF_FERM_CHAMBERS > 99
    #define NUM_DIGITS_FERM_CHAMBERS 3
#elif NUM_OF_FERM_CHAMBERS > 999
    #define NUM_DIGITS_FERM_CHAMBERS 4
#else
    #define NUM_DIGITS_FERM_CHAMBERS 1
#endif

#define BEER "beer"
#define CHAMBER "chamber"
#define AMBIENT "ambient"

void init(bool &fahrenheit, fermChamber (&chambers)[NUM_OF_FERM_CHAMBERS]);
void getTempData(bool fahrenheit, fermChamber (&chambers)[NUM_OF_FERM_CHAMBERS]);
int strToInt(string &strToConvert, string type);
