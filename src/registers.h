#pragma once 

// All registers in the datasheet do not differ from real registers by 1,
// so all registers here are 1 less than


// Metered data

//-------Possibly under Energy----------\\

#define REAL_ENERGY_TOTAL_FLOAT 1000    // kWh

#define APPARENT_ENERGY_TOTAL_FLOAT 1002 // kVAh

//--------

#define REAL_POWER_TOTAL_FLOAT 1006 - 1     // kWh 

#define APPARENT_POWER_TOTAL_FLOAT 1008 - 1 // kVA

#define REACTIVE_POWER_TOTAL_FLOAT 1010 - 1 // kVAR

#define CURRENT_3P_AVERAGE_FLOAT 1018 - 1  // Amp

#define REAL_POWER_TOTAL_DEMAND_PRESENT_FLOAT 1022 - 1 // kW



//---------------CURRENT-----------------\\


#define CURRENT_A_FLOAT 1034 - 1            // Amp
#define CURRENT_B_FLOAT 1036 - 1           // Amp
#define CURRENT_C_FLOAT 1038 - 1            // Amp
#define CURRENT_N_FLOAT 1040 - 1             // Amp

// Displayed on the AMPS PER PHASE display

#define CURRENT_3P_AVERAGE_INT 4012 - 1    // Amp


#define CURRENT_A_INT 4020 - 1             // Amp
#define CURRENT_B_INT 4021 - 1              // Amp
#define CURRENT_C_INT 4022 - 1               // Amp
#define CURRENT_N_INT 4023 - 1              // Amp

#define CURRENT_A_DEMAND_PRESENT_INT 4024 - 1   // Amp
#define CURRENT_B_DEMAND_PRESENT_INT 4025 - 1  // Amp
#define CURRENT_C_DEMAND_PRESENT_INT 4026 - 1   // Amp


#define CURRENT_A_DEMAND_PEAK_INT 4027 - 1   // Amp
#define CURRENT_B_DEMAND_PEAK_INT 4028 - 1   // Amp
#define CURRENT_C_DEMAND_PEAK_INT 4029 - 1   // Amp


#define CURRENT_A_MIN_INT 4060 - 1
#define CURRENT_B_MIN_INT 4061 - 1
#define CURRENT_C_MIN_INT 4062 - 1

#define CURRENT_A_MAX_INT 4085 - 1
#define CURRENT_B_MAX_INT 4086 - 1
#define CURRENT_C_MAX_INT 4087 - 1
//---------------------------------------\\


//-----------------POWER------------------\\

#define REAL_POWER_TOTAL_INT 4006 - 1      // kW

//---------------------------------------\\

// Setup ans status

#define USAGE_HOURS_INT 4110 - 1            // TIMER
#define USAGE_MINUTES_INT 4111 - 1          // TIMER

#define USAGE_HOURS_FLOAT 1204 - 1          // TIMER
#define USAGE_MINUTES_FLOAT 1205 - 1        // TIMER

#define SERVICE_FRIQUENCY_INT 4125 - 1      // 50 or 60 Hz

#define DISPLAY_MODE_INT 4128 - 1         // 0 = IEC
                                        // 1 = IEEE
