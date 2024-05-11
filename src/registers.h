#pragma once 

// All registers in the datasheet do not differ from real registers by 1,
// so all registers here are 1 less than


// Metered data

#define FREQUENCY_FLOAT 1020 - 1                           // Hz 45.0 - 65.0 current frequency
#define FREQUENCY_MIN_FLOAT 1112 - 1                       // Hz 45.0-85.0 
#define FREQUENCY_MAX_FLOAT 1162 - 1                       // Hz 45.0-85.0 

#define FREQUENCY_INT 4013 - 1                             // Hz Scale: 0.0001, 4500-6500 

#define FREQUENCY_MIN_INT 4059 - 1                         // Hz Scale: 0.01, 4500-8500
#define FREQUENCY_MAX_INT 4084 - 1                         // Hz Scale: 0.01, 4500-8500

//----------Possibly under Energy----------\\

#define REAL_ENERGY_TOTAL_LONG 4000 - 1                    // kWh / Scale

#define APPARENT_ENERGY_TOTAL_LONG 4002 - 1                // kVAh  / Scale

#define REACTIVE_ENERGY_TOTAL_LONG 4004 - 1                // kVARh / Scale

#define REAL_ENERGY_TOTAL_FLOAT 1000 - 1                   // kWh

#define APPARENT_ENERGY_TOTAL_FLOAT 1002 - 1               // kVAh  

#define REACTIVE_ENERGY_TOTAL_FLOAT 1004 - 1               // kVARh  


//----------Possibly under Energy----------\\



//-----------------VOLTAG------------------\\

#define VOLTAGE_LL_3P_AVERAGE_FLOAT 1014 - 1               // Volt 
#define VOLTAGE_LN_3P_AVERAGE_FLOAT 1016 - 1               // Volt 

#define VOLTAGE_AB_FLOAT 1054 - 1                          // Volt
#define VOLTAGE_BC_FLOAT 1056 - 1                          // Volt
#define VOLTAGE_CA_FLOAT 1058 - 1                          // Volt

#define VOLTAGE_AN_FLOAT 1060 - 1                          // Volt
#define VOLTAGE_BN_FLOAT 1062 - 1                          // Volt
#define VOLTAGE_CN_FLOAT 1064 - 1                          // Volt

#define VOLTAGE_AN_THD_FLOAT 1092 - 1                      // 0.0-1000.0 %
#define VOLTAGE_BN_THD_FLOAT 1094 - 1                      // 0.0-1000.0 %
#define VOLTAGE_CN_THD_FLOAT 1096 - 1                      // 0.0-1000.0 %
#define VOLTAGE_AB_THD_FLOAT 1098 - 1                      // 0.0-1000.0 %
#define VOLTAGE_BC_THD_FLOAT 1100 - 1                      // 0.0-1000.0 %
#define VOLTAGE_CA_THD_FLOAT 1102 - 1                      // 0.0-1000.0 %
                                                           
#define VOLTAGE_AN_MIN_FLOAT 1122 - 1                      // Volt 
#define VOLTAGE_BN_MIN_FLOAT 1124 - 1                      // Volt 
#define VOLTAGE_CN_MIN_FLOAT 1126 - 1                      // Volt 
#define VOLTAGE_AB_MIN_FLOAT 1128 - 1                      // Volt 
#define VOLTAGE_BC_MIN_FLOAT 1130 - 1                      // Volt  
#define VOLTAGE_CA_MIN_FLOAT 1132 - 1                      // Volt 

#define VOLTAGE_AN_THD_MIN_FLOAT 1142 - 1                  // % 0.0-1000.0 
#define VOLTAGE_BN_THD_MIN_FLOAT 1144 - 1                  // % 0.0-1000.0 
#define VOLTAGE_CN_THD_MIN_FLOAT 1146 - 1                  // % 0.0-1000.0 
#define VOLTAGE_AB_THD_MIN_FLOAT 1148 - 1                  // % 0.0-1000.0 
#define VOLTAGE_BC_THD_MIN_FLOAT 1150 - 1                  // % 0.0-1000.0 
#define VOLTAGE_CA_THD_MIN_FLOAT 1152 - 1                  // % 0.0-1000.0 

#define VOLTAGE_AN_MAX_FLOAT 1172 - 1                      // Volt 
#define VOLTAGE_BN_MAX_FLOAT 1174 - 1                      // Volt  
#define VOLTAGE_CN_MAX_FLOAT 1176 - 1                      // Volt  

#define VOLTAGE_AB_MAX_FLOAT 1178 - 1                      // Volt  
#define VOLTAGE_BC_MAX_FLOAT 1180 - 1                      // Volt  
#define VOLTAGE_CA_MAX_FLOAT 1182 - 1                      // Volt  

#define VOLTAGE_AN_THD_MAX_FLOAT 1192 - 1                  // % 0.0-1000.0 
#define VOLTAGE_BN_THD_MAX_FLOAT 1194 - 1                  // % 0.0-1000.0
#define VOLTAGE_CN_THD_MAX_FLOAT 1196 - 1                  // % 0.0-1000.0

#define VOLTAGE_AB_THD_MAX_FLOAT 1198 - 1                  // % 0.0-1000.0
#define VOLTAGE_BC_THD_MAX_FLOAT 1200 - 1                  // % 0.0-1000.0
#define VOLTAGE_CA_THD_MAX_FLOAT 1202 - 1                  // % 0.0-1000.0


#define VOLTAGE_LL_3P_AVERAGE_INT 4010 - 1                 // - Volt / Scale

#define VOLTAGE_LN_3P_AVERAGE_INT 4011 - 1                 // - Volt / Scale

#define VOLTAGE_AB_INT 4030 - 1                            // - Volt / Scale
#define VOLTAGE_BC_INT 4031 - 1                            // - Volt / Scale
#define VOLTAGE_CA_INT 4032 - 1                            // - Volt / Scale
                                                           
#define VOLTAGE_AN_INT 4033 - 1                            // - Volt / Scale
#define VOLTAGE_BN_INT 4034 - 1                            // - Volt / Scale
#define VOLTAGE_CN_INT 4035 - 1                            // - Volt / Scale

#define VOLTAGE_AN_THD_INT 4049 - 1                        // Scale 0.1, 0-10000 %
#define VOLTAGE_BN_THD_INT 4050 - 1                        // Scale 0.1, 0-10000 %
#define VOLTAGE_CN_THD_INT 4051 - 1                        // Scale 0.1, 0-10000 %
                                                           
#define VOLTAGE_AB_THD_INT 4052 - 1                        // Scale 0.1, 0-10000 %
#define VOLTAGE_BC_THD_INT 4053 - 1                        // Scale 0.1, 0-10000 %
#define VOLTAGE_CA_THD_INT 4054 - 1                        // Scale 0.1, 0-10000 %
                                                           
#define VOLTAGE_AN_MIN_INT 4064 - 1                        // Volt
#define VOLTAGE_BN_MIN_INT 4065 - 1                        // Volt
#define VOLTAGE_CN_MIN_INT 4066 - 1                        // Volt
                                                           
#define VOLTAGE_AB_MIN_INT 4067 - 1                        // Volt
#define VOLTAGE_BC_MIN_INT 4068 - 1                        // Volt
#define VOLTAGE_CA_MIN_INT 4069 - 1                        // Volt

#define VOLTAGE_A_THD_MIN_INT 4070 - 1                     // Scale: 0.1, 0-10000 %
#define VOLTAGE_B_THD_MIN_INT 4071 - 1                     // Scale: 0.1, 0-10000 %
#define VOLTAGE_C_THD_MIN_INT 4072 - 1                     // Scale: 0.1, 0-10000 %
                                                           
#define VOLTAGE_AN_THD_MIN_INT 4074 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_BN_THD_MIN_INT 4075 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_CN_THD_MIN_INT 4076 - 1                    // Scale: 0.1, 0-10000 %

#define VOLTAGE_AB_THD_MIN_INT 4077 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_BC_THD_MIN_INT 4078 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_CA_THD_MIN_INT 4079 - 1                    // Scale: 0.1, 0-10000 %

#define VOLTAGE_AN_MAX_INT 4089 - 1                        // Volt
#define VOLTAGE_BN_MAX_INT 4090 - 1                        // Volt
#define VOLTAGE_CN_MAX_INT 4091 - 1                        // Volt
                                                           
#define VOLTAGE_AB_MAX_INT 4092 - 1                        // Volt
#define VOLTAGE_BC_MAX_INT 4093 - 1                        // Volt
#define VOLTAGE_CA_MAX_INT 4094 - 1                        // Volt

#define CURRENT_A_THD_MAX_INT 4095 - 1                     // Scale: 0.1, 0-10000 %
#define CURRENT_B_THD_MAX_INT 4096 - 1                     // Scale: 0.1, 0-10000 %
#define CURRENT_C_THD_MAX_INT 4097 - 1                     // Scale: 0.1, 0-10000 %
                                                           
#define VOLTAGE_AN_THD_MAX_INT 4099 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_BN_THD_MAX_INT 4100 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_CN_THD_MAX_INT 4101 - 1                    // Scale: 0.1, 0-10000 %
                                                           
#define VOLTAGE_AB_THD_MAX_INT 4102 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_BC_THD_MAX_INT 4103 - 1                    // Scale: 0.1, 0-10000 %
#define VOLTAGE_CA_THD_MAX_INT 4104 - 1                    // Scale: 0.1, 0-10000 %


//-----------------CURRENT-----------------\\

#define CURRENT_3P_AVERAGE_FLOAT 1018 - 1                  // Amp


#define CURRENT_A_FLOAT 1034 - 1                           // Amp
#define CURRENT_B_FLOAT 1036 - 1                           // Amp
#define CURRENT_C_FLOAT 1038 - 1                           // Amp
#define CURRENT_N_FLOAT 1040 - 1                           // Amp

#define CURRENT_A_DEMAND_PRESENT_FLOAT 1042 - 1            // Amp 
#define CURRENT_B_DEMAND_PRESENT_FLOAT 1044 - 1            // Amp 
#define CURRENT_C_DEMAND_PRESENT_FLOAT 1046 - 1            // Amp 

#define CURRENT_A_DEMAND_PEAK_FLOAT 1048 - 1               // Amp 
#define CURRENT_B_DEMAND_PEAK_FLOAT 1050 - 1               // Amp 
#define CURRENT_C_DEMAND_PEAK_FLOAT 1052 - 1               // Amp

#define CURRENT_A_THD_FLOAT 1084 - 1                       // 0.0-1000.0 %
#define CURRENT_B_THD_FLOAT 1086 - 1                       // 0.0-1000.0 %
#define CURRENT_C_THD_FLOAT 1088 - 1                       // 0.0-1000.0 %
                                                           
#define CURRENT_A_MIN_FLOAT 1114 - 1                       // Amp
#define CURRENT_B_MIN_FLOAT 1116 - 1                       // Amp
#define CURRENT_C_MIN_FLOAT 1118 - 1                       // Amp

#define CURRENT_A_THD_MINIMUM_FLOAT 1134 - 1               //  0.0-1000.0 %
#define CURRENT_B_THD_MINIMUM_FLOAT 1136 - 1               //  0.0-1000.0 %
#define CURRENT_C_THD_MINIMUM_FLOAT 1138 - 1               //  0.0-1000.0 %

#define CURRENT_A_MAX_FLOAT 1164- 1                        // Amp 
#define CURRENT_B_MAX_FLOAT 1166 - 1                       // Amp
#define CURRENT_C_MAX_FLOAT 1168 - 1                       // Amp

#define CURRENT_A_THD_MAX_FLOAT 1184 - 1                   // % 0.0-1000.0 
#define CURRENT_B_THD_MAX_FLOAT 1186 - 1                   // % 0.0-1000.0 
#define CURRENT_C_THD_MAX_FLOAT 1188 - 1                   // % 0.0-1000.0 


// Displayed on the AMPS PER PHASE display

#define CURRENT_3P_AVERAGE_INT 4012 - 1                    // Amp


#define CURRENT_A_INT 4020 - 1                             // Amp
#define CURRENT_B_INT 4021 - 1                             // Amp
#define CURRENT_C_INT 4022 - 1                             // Amp
#define CURRENT_N_INT 4023 - 1                             // Amp

#define CURRENT_A_DEMAND_PRESENT_INT 4024 - 1              // Amp
#define CURRENT_B_DEMAND_PRESENT_INT 4025 - 1              // Amp
#define CURRENT_C_DEMAND_PRESENT_INT 4026 - 1              // Amp


#define CURRENT_A_DEMAND_PEAK_INT 4027 - 1                 // Amp
#define CURRENT_B_DEMAND_PEAK_INT 4028 - 1                 // Amp
#define CURRENT_C_DEMAND_PEAK_INT 4029 - 1                 // Amp


#define CURRENT_A_MIN_INT 4060 - 1                         // Amp
#define CURRENT_B_MIN_INT 4061 - 1                         // Amp
#define CURRENT_C_MIN_INT 4062 - 1                         // Amp
                                                           
#define CURRENT_A_MAX_INT 4085 - 1                         // Amp
#define CURRENT_B_MAX_INT 4086 - 1                         // Amp
#define CURRENT_C_MAX_INT 4087 - 1                         // Amp

//-----------------CURRENT-----------------\\


//------------------POWER------------------\\

#define REAL_POWER_TOTAL_FLOAT 1006 - 1                    // kWh 
#define APPARENT_POWER_TOTAL_FLOAT 1008 - 1                // kVA 
#define REACTIVE_POWER_TOTAL_FLOAT 1010 - 1                // kVAR 

#define POWER_FACTOR_TOTAL 1012 - 1                        // 0.0 - 1.0

#define REAL_POWER_TOTAL_DEMAND_PRESENT_FLOAT 1022 - 1     // kW 

#define APPARENT_POWER_TOTAL_DEMAND_PRESENT_FLOAT 1024 - 1 // kVA

#define REACTIVE_POWER_TOTAL_DEMAND_PRESENT_FLOAT 1026 - 1 // kVAR 

#define REAL_POWER_TOTAL_DEMAND_PEAK_FLOAT 1028 - 1        // kW

#define APPARENT_POWER_TOTAL_DEMAND_PEAK_FLOAT 1030 - 1    // kVA

#define REACTIVE_POWER_TOTAL_DEMAND_PEAK_FLOAT 1032 - 1    // kVAR



#define REAL_POWER_A_FLOAT 1066 - 1                        // kW
#define REAL_POWER_B_FLOAT 1068 - 1                        // kW
#define REAL_POWER_C_FLOAT 1070 - 1                        // kW

#define APPARENT_POWER_A_FLOAT 1072 - 1                    // kVA
#define APPARENT_POWER_B_FLOAT 1074 - 1                    // kVA
#define APPARENT_POWER_C_FLOAT 1076 - 1                    // kVA
                                                           
#define REACTIVE_POWER_A_FLOAT 1078 - 1                    // kVAR
#define REACTIVE_POWER_B_FLOAT 1080 - 1                    // kVAR
#define REACTIVE_POWER_C_FLOAT 1082 - 1                    // kVAR

#define REAL_POWER_TOTAL_MIN_FLOAT 1104 - 1                // kW 
#define APPARENT_POWER_TOTAL_MIN_FLOAT 1106 - 1            // kVA
#define REACTIVE_POWER_TOTAL_MIN_FLOAT 1108 - 1            // kVAR
#define POWER_FACTOR_TOTAL_MIN_FLOAT 1110 - 1              // 0.0-1.0

#define REAL_POWER_TOTAL_MAX_FLOAT 1154 - 1                // kW 
#define APPARENT_POWER_TOTAL_MAX_FLOAT 1156 - 1            // kVA
#define REACTIVE_POWER_TOTAL_MAX_FLOAT 1158 - 1            // kVAR
#define POWER_FACTOR_TOTAL_MAX_FLOAT 1160 - 1              // 0.0-1.0 

#define REAL_POWER_TOTAL_INT 4006 - 1                      // kW / Scale
#define APPARENT_POWER_TOTAL_INT 4007 - 1                  // kVA / Scale
#define REACTIVE_POWER_TOTAL_INT 4008 - 1                  // kVAR / Scale

#define POWER_FACTOR_TOTAL_INT 4009 - 1                    // Scale: 0.0001, 0-1

#define REAL_POWER_TOTAL_DEMAND_PRESENT_INT 4014 - 1       // kW / Scale
#define APPARENT_POWER_TOTAL_DEMAND_PRESENT_INT 4015 - 1   // kVA / Scale
#define REACTIVE_POWER_TOTAL_DEMAND_PRESENT_INT 4016 - 1   // kVAR / Scale

#define REAL_POWER_TOTAL_DEMAND_PEAK_INT 4017 - 1          // kW / Scale
#define APPARENT_POWER_TOTAL_DEMAND_PEAK_INT 4018 - 1      // kVA / Scale
#define REACTIVE_POWER_TOTAL_DEMAND_PEAK_INT 4019 - 1      // kVAR / Scale

#define REAL_POWER_A_INT 4036 - 1                          // kW / Scale
#define REAL_POWER_B_INT 4037 - 1                          // kW / Scale
#define REAL_POWER_C_INT 4038 - 1                          // kW / Scale

#define APPARENT_POWER_A_INT 4039 - 1                      // kVA / Scale
#define APPARENT_POWER_B_INT 4040 - 1                      // kVA / Scale
#define APPARENT_POWER_C_INT 4041 - 1                      // kVA / Scale
                                                           
#define REACTIVE_POWER_A_INT 4042 - 1                      // kVAR / Scale
#define REACTIVE_POWER_B_INT 4043 - 1                      // kVAR / Scale
#define REACTIVE_POWER_C_INT 4044 - 1                      // kVAR / Scale

#define CURRENT_A_THD_INT 4045 - 1                         // Scale 0.1, 0-10000 %
#define CURRENT_B_THD_INT 4046 - 1                         // Scale 0.1,  0-10000 %
#define CURRENT_C_THD_INT 4047 - 1                         // Scale 0.1 0-10000 %

#define REAL_POWER_TOTAL_MIN_INT 4055 - 1                  // kW
#define APPARENT_POWER_TOTAL_MIN_INT 4056 - 1              // kVA
#define REACTIVE_POWER_TOTAL_MIN_INT 4057 - 1              // kVAR
                                                           
#define POWER_FACTOR_TOTAL_MIN_INT 4058 - 1                // Scale 0.0001,  0-1000.0
                                                           
#define REAL_POWER_TOTAL_MAX_INT 4080 - 1                  // kW
#define APPARENT_POWER_TOTAL_MAX_INT 4081 - 1              // kVA
#define REACTIVE_POWER_TOTAL_MAX_INT 4082 - 1              // kVAR 

#define POWER_FACTOR_TOTAL_MAX_INT 4083 - 1                // Scale: 0.0001, 0-1000.0 

//------------------POWER------------------\\


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

#define SCALE_FACTOR_I_INT 4105 - 1                        // Power of 10
#define SCALE_FACTOR_V_INT 4106 - 1                        // Power of 10
#define SCALE_FACTOR_W_INT 4107 - 1                        // Power of 10
#define SCALE_FACTOR_E_INT 4108 - 1                        // Power of 10
                                                           
#define USAGE_HOURS_INT 4110 - 1                           // TIMER
#define USAGE_MINUTES_INT 4111 - 1                         // TIMER
                                                           
#define SERVICE_FRIQUENCY_INT 4125 - 1                     // 50 or 60 Hz
#define DISPLAY_MODE_INT 4128 - 1                          // 0 = IEC
                                                           // 1 = IEEE

#define USAGE_HOURS_FLOAT 1204 - 1                         // TIMER
#define USAGE_MINUTES_FLOAT 1205 - 1                       // TIMER

#define THEMAL_DEMAND_INTERVAL_INT 4117 - 1                // Minutes 1-60, Current Demand Only
#define POWER_DEMAND_INTERVAL_INT 4118 - 1                 // Minutes 1-60, Duration in minutes

#define CT_RATIO_PRIMARY_INT 4120 - 1 
#define CT_RATIO_SECONDARY_INT 4121 - 1
#define PW_RATIO_PRIMARY_INT 4122 - 1
#define PW_RATIO_SCALE_INT 4123 - 1                        // 0, 1, 10, 100 (0 = No PT)
#define PT_RATIO_SECONDARY_INT 4124 - 1                    // 100, 110, 115, 120

#define SERVICE_FREQUENCY_INT 4125 - 1                     // Hz 50 or 60