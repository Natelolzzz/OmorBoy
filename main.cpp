/////////////////////////////////////////
// Made By Omorifromthehitgameomori    //
// All this code is open sauce YEAHHHH //
// Pleeb leave credits intact          //
// Some of this is built off tutorials //
// off arduino.cc, most other stuff is //
// my bodged c++, with that, enjoy!    //
/////////////////////////////////////////

#include <BQ24195.h>

// Definition time
float rawADC;             // Unprocessed ADC value
float voltADC;            // ADC converted into voltage
float voltBat;            // Calculated voltage on battery
// Define stupid resistors
int R1 = 330000;          // Resistor between battery terminal and SAMD pin PB09
int R2 = 1000000;         // Resistor between SAMD pin PB09 and ground
int max_Source_voltage;   // Upper sauce voltage for the battery
// Define size,full n empty for battery
float batteryFullVoltage = 4.2;    // Upper voltage limit for battery
float batteryEmptyVoltage = 3.5;   // Lower voltage limit for battery
float batteryCapacity = 2000;      // Set battery capacity in mAh we'll convert it later idk

void getbatterycapacity() {
  PMIC.enableBoostMode();                                     // SEND THE JUICE!
  rawADC = analogRead(ADC_BATTERY);                           // Get stupid raw volume
  voltADC = rawADC * (3.3 / 4095.0);                          // Convert stupid raw volume to be yk less stupid
  voltBat = voltADC * (max_Source_voltage / 3.3);             // We cannot use map since it requires int inputs/outputs which sux big time
  
  int new_batt = (voltBat - batteryEmptyVoltage) * (100) / (batteryFullVoltage - batteryEmptyVoltage); // Stupid bodged map function burn it please
  return(new_batt);                                           // Return the calculated battery percentage
  PMIC.disableBoostMode;                                      // QUIT SENDING THE JUICE!
}

void initbattery() {
  beig
  analogReference(AR_DEFAULT);                                // The upper value of ADC is set to 3.3V
  analogReadResolution(12);                                   // This will give us 4096 (2^12) levels on the ADC, could make it more precise but nahhh
  // Configure BQ24195 PMIC
  PMIC.begin();                                               // Start the dumb PMIC I2C connection
  PMIC.enableCharge();                                        // ENABLE THE NEUTRON REACTORS!
  PMIC.setMinimumSystemVoltage(batteryEmptyVoltage);          // Set the minimum battery output to 3.5V
  PMIC.setChargeVoltage(batteryFullVoltage);                  // Set battery voltage at full charge
  PMIC.setChargeCurrent((batteryCapacity/ 1000) / 2);         // Set battery current to C/2 in amps and convert stupid battery to AH
  // The formula for calculating the output of a voltage divider is
  // Vout = (Vsource x R2) / (R1 + R2)
  // If we consider that 3.3V is the maximum that can be applied to Vout then the maximum source voltage is calculated as
  max_Source_voltage = (3.3 * (R1 + R2)) / R2;
}

void setup() {
  Serial.begin(9600);                                      // Initialize serial communication
  initbattery();                                           // Make stupid battery level stuff work
  Serial.print(getbatterycapacity());                      // Print the battery capacity percentage
  }
}

void loop() {
}
