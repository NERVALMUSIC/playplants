//Touch this
//

//Defines
#define CONTROL true	// <-- Comentar para emisor
//#define DEBUG //Uncomment for debug

//Sensor Stuff
#ifndef CONTROL
  #define HEAD 8			  // <-- Hardcode value between 1 and 9
  #define TOUCH 12
  #define RELEASE 4
  mpr121_proxmode_type  prox_mode = PROX_DISABLED; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)
#else
  #define HEAD 10  // <-- Midi channel for control keypad
  #define CCNOTE 25
  #define GOCHANNEL 5            //<-- Channel used for MODE change
  #define ELEC_GO 10             //<-- Electrode used for MODE increase
  #define ELEC_GO_BACK 11        //<-- Electrode used for MODE decrease
  /*ARRAY de memorias:
   0 = No cambia nada
   1 = Cambia MainStage
   2 = Cambia Resolume
   3 = Cambia todo
   */
   const uint8_t MEMORIAS[20] = {3, 1, 2, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
  /*ARRAYS para MODOS Y NOTAS
  NOTE son las arrays para cada planta y modo
  Los MODES son:
  0 = Send notes basic.
  1 = Send notes with control change while touching.
  2 = Send proximity value of first note of its array.
  3 = COUNT UP from first note of its array, ecah touch next note. Fixed intensity
  4 = Send random values from array on touching
  5 = Do nothing
  */

  
  const uint8_t NOTES1[ 9 ][ 12 ] = { {60, 1, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 1 <-- Dialogo 1
                                      {60, 2, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2 <-- Dialogo 2
                                      {36, 40, 43, 47, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {60, 64, 67, 71, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15}, // Canal 5 < -- Control Electrodos 10 y 11
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES1[9] = {3, 3, 1, 1, 1, 1, 1, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES2[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES2[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES3[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES3[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES4[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES4[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES5[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES5[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES6[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES6[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES7[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES7[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES8[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES8[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES9[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES9[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES10[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES10[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES11[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- ControlControl
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES11[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES12[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES12[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES13[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES13[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES14[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES14[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES15[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES15[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES16[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES16[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES17[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES17[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES18[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES18[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES19[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES19[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  const uint8_t NOTES20[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES20[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 
#endif

const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t KEY_PRESSURE = 0xA0;      //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types
const uint8_t PROGRAM_CHANGE = 0xC0;    //<-- Used Midi message types
const uint8_t CHANNEL_PRESSURE = 0xD0;  //<-- Used Midi message types
const uint8_t PITCH_BEND = 0xE0;        //<-- Used Midi message types

//Do NOT touch this
//

//Sensor stuf
#define I2CADDR 0x5A
#define WINDOW 25
#define SENSORS 13 // number of sensors + proximity
#define MINPROX -25
#define MAXPROX 6

//Radio stuff
//const uint64_t PIPES[4] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL};
#define PINCE 9
#define PINCS 10
//#define BUFF 4
