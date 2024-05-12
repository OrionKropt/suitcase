#pragma once 

// All registers in the datasheet do not differ from real registers by 1,
// so all registers here are 1 less than


// Metered data

//-------Possibly under Energy----------\\

#define REAL_ENERGY_TOTAL_LONG 4000 - 1 // kWh

#define REAL_ENERGY_TOTAL_FLOAT 1000    // kWh

#define APPARENT_ENERGY_TOTAL_FLOAT 1002 // kVAh  / Scale

#define APPARENT_ENERGY_TOTAL_LONG 4002 - 1 // kVAh  / Scale

#define REACTIVE_ENERGY_TOTAL_LONG 4004 - 1 // kVARh / Scale
//--------



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

#define REAL_POWER_TOTAL_FLOAT 1006 - 1         // kWh 
#define APPARENT_POWER_TOTAL_FLOAT 1008 - 1     // kVA 
#define REACTIVE_POWER_TOTAL_FLOAT 1010 - 1     // kVAR 


#define REAL_POWER_TOTAL_INT 4006 - 1        // kW / Scale

#define APPARENT_POWER_TOTAL_INT 4007 - 1     // kVA / Scale

#define REACTIVE_POWER_TOTAL_INT 4008 - 1     // kVAR / Scale

#define POWER_FACTOR_TOTAL_INT 4009 - 1 

#define REAL_POWER_A_INT 4036 - 1             // kW
#define REAL_POWER_B_INT 4037 - 1             // kW
#define REAL_POWER_C_INT 4038 - 1             // kW

#define REAL_POWER_TOTAL_MIN_INT 4055 - 1     // kW
#define REAL_POWER_TOTAL_MAX_INT 4080 - 1     // kW

#define REAL_POWER_TOTAL_MIN_FLOAT 1104 - 1  // kW 

#define APPARENT_POWER_A_INT 4039 - 1         // kVA
#define APPARENT_POWER_B_INT 4040 - 1         // kVA
#define APPARENT_POWER_C_INT 4041 - 1         // kVA

#define APPARENT_POWER_TOTAL_MIN_INT 4056 - 1 // kVA
#define APPARENT_POWER_TOTAL_MAX_INT 4081 - 1 // kVA

#define REACTIVE_POWER_A_INT 4042 - 1         // kVAR 
#define REACTIVE_POWER_B_INT 4043 - 1         // kVAR 
#define REACTIVE_POWER_C_INT 4044 - 1         // kVAR 

#define REACTIVE_POWER_TOTAL_MIN_INT 4057 - 1 // kVAR
#define REACTIVE_POWER_TOTAL_MIN_INT 4082 - 1 // kVAR 

//---------------------------------------\\

// Setup ans status

#define SCALE_FACTOR_I_INT 4105 - 1         // Power of 10
#define SCALE_FACTOR_V_INT 4106 - 1         // Power of 10
#define SCALE_FACTOR_W_INT 4107 - 1         // Power of 10
#define SCALE_FACTOR_E_INT 4108 - 1         // Power of 10

#define USAGE_HOURS_INT 4110 - 1            // TIMER
#define USAGE_MINUTES_INT 4111 - 1          // TIMER

#define USAGE_HOURS_FLOAT 1204 - 1          // TIMER
#define USAGE_MINUTES_FLOAT 1205 - 1        // TIMER

#define SERVICE_FRIQUENCY_INT 4125 - 1      // 50 or 60 Hz

#define DISPLAY_MODE_INT 4128 - 1         // 0 = IEC
                                          // 1 = IEEE
