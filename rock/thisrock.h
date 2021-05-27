//Touch this
//

//Defines
#define CONTROL true	// <-- Comentar para emisor
//#define DEBUG //Uncomment for debug

//Sensor Stuff
#ifndef CONTROL
  #define HEAD 2			  // <-- Hardcode value between 1 and 9 
  #define TOUCH 12
  #define RELEASE 4
  mpr121_proxmode_type  prox_mode = PROX_DISABLED; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)
#else
  #define HEAD 10  // <-- Midi channel for control keypad
  #define CCNOTE 25
  #define GOCHANNEL 1            //<-- Channel used for MODE change
  #define ELEC_GO 0             //<-- Electrode used for MODE increase
  #define ELEC_GO_BACK 1        //<-- Electrode used for MODE decrease
  /*ARRAY de memorias:
   0 = No cambia nada
   1 = Cambia MainStage
   2 = Cambia Resolume
   3 = Cambia todo
   */
   const uint8_t MEMORIAS[20] = {2, 3, 3, 3, 2, 2, 3, 3, 2, 3, 3, 3, 2, 3, 3, 3, 2, 2, 2, 2}; 
  /*ARRAYS para MODOS Y NOTAS
  NOTE son las arrays para cada planta y modo
  Los MODES son:
  0 = Send notes basic.
  1 = Send notes with control change while touching.
  2 = Send proximity value of first note of its array.
  3 = COUNT UP from first note of its array, ecah touch next note. Fixed intensity
  4 = Send random values from array on touching
  5 = Do nothing

  SOLO PARA RAMBLETA

 
  1 = CONTROL - 2 (snsor 0 Go  - sensor 1 back)
  2 = ALOE - (sensores 0,1,4,5,6,7,8,9)
  3 = ARBOL (sensores 0,1,2,3,4,5,6,7 )
  4 = MONSTERA GRANDE (0,1,2,3,4 )
  5 = MONSTERA PEQUE (0,1,2,3,4,5 )
  6 = AFRA (0,1,2,3 )
  7 = CALA + Alocacia (0,1,2,3,4,5)
  
  */

 //1 - Entrada público  
  const uint8_t NOTES1[ 9 ][ 12 ] = { {60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Canal 1< -- Control
                                      {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 2 - no usar sensor 3,4
                                     {72, 76, 69, 65, 62, 57, 84, 89, 0, 0, 0}, // Canal 3..está
                                    {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 4..está  
                                      {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 5 .. está
                                      {60, 1, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 6
                                     {60, 2, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 7
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Canal 8
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};// Canal 9
  const uint8_t MODES1[9] = {0, 0, 0, 0, 0, 3, 3, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random

//2 - José 
  const uint8_t NOTES2[ 9 ][ 12 ] =  { {60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Canal 1< -- Control
                                      {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 2 - no usar sensor 3,4
                                     {72, 76, 69, 65, 62, 57, 84, 89, 0, 0, 0}, // Canal 3..está
                                      {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 4..está  
                                      {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 5 .. está
                                      {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 6
                                     {72, 76, 69, 65, 62, 57, 84, 89, 93, 0, 0, 0}, // Canal 7
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Canal 8
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};// Canal 9
  const uint8_t MODES2[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};       //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  
 //3 - Presentación plantas 
  const uint8_t NOTES3[ 9 ][ 12 ] = { {60, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},            // Canal 1 <-- Dialogo 1
                                      {60, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},            // Canal 2 <-- Dialogo 2
                                      {60, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                       {60, 50, 51, 57, 58, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                       {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
 const uint8_t MODES3[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random

 //4 - Natural
 const uint8_t NOTES4[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {51, 50, 57, 58, 60 , 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES4[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  
  //5 - Video Vivero + Nubes
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
 
  //6 - Aspiradora
  const uint8_t NOTES6[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {53, 53, 53, 53, 53, 53, 53, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {53, 69, 70, 72, 73, 75, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES6[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 
  //7 - Conversación plantas
  const uint8_t NOTES7[ 9 ][ 12 ]  = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1< -- Control
                                      {53, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Canal 2 - no usar sensor 3,4
                                      {53, 41, 65, 58, 46, 70, 0, 0, 0, 0, 0, 0}, // Canal 3..está
                                      {59, 38,  59, 54, 64, 0, 0, 0, 0, 0, 0, 0}, // Canal 4..está  
                                      {53, 69, 70, 72, 73, 75, 47, 48, 36, 38, 13, 15}, // Canal 5 .. está
                                      {60, 1, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 2, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES7[9] = {0, 0, 0, 0, 0, 3, 3, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 
  //8 - TECHNO
  const uint8_t NOTES8[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1< -- Control
                                      {53, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Canal 2 - no usar sensor 3,4
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 3..está
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 4..está  
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 5 .. está
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES8[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 
 //9 - TXT VIOLENCIA 
  const uint8_t NOTES9[ 9 ][ 12 ] = {{60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
                                      {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
                                      {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
                                      {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES9[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 
  //10 - VIDEO EXPEDICIÓN 
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
 
 //11 - LUZ + CORTAR UN ARBOL
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
 
  //12 - ESCUELA + FUERA MICRO
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
 
  //13 - SOMOS AGUA
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
  
  //14 - LUZ + SOMOS AGUA
  const uint8_t NOTES14[ 9 ][ 12 ]= { {60, 64, 67, 71, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 1 < -- Control< -- Mainstage 13 y 14 Resolume 15 y 16 ///SOMOS AGUA
                                      {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71},// Canal 2 
                                      {60, 61, 62, 63, 64, 72, 76, 79, 83, 84, 72, 84}, // Canal 3..está
                                      {55, 59, 60, 57, 59, 45, 47, 48, 36, 38, 40, 41}, // Canal 4 .. está
                                       {35, 35, 35, 41, 43, 45, 47, 48, 36, 38, 13, 15}, // Canal 5 ..está percusión
                                      {60, 61, 62, 63, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6..está
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES14[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 //15 - BBC VIDEO + TELEGRAFO DRAMA
const uint8_t NOTES15[ 9 ][ 12 ]  = { {60, 64, 67, 71, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 1 < -- Control< -- Mainstage 13 y 14 Resolume 15 y 16///SOMOS AGUA
                                      {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71},// Canal 2 
                                      {60, 61, 62, 63, 64, 72, 76, 79, 83, 84, 72, 84}, // Canal 3..está
                                      {55, 59, 60, 57, 59, 45, 47, 48, 36, 38, 40, 41}, // Canal 4 .. está
                                       {35, 35, 35, 41, 43, 45, 47, 48, 36, 38, 13, 15}, // Canal 5 ..está percusión
                                      {60, 61, 62, 63, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6..está
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES15[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 //16 - RIO BUENO
  const uint8_t NOTES16[ 9 ][ 12 ] = { {60, 64, 67, 71, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 1 < -- Control< -- Mainstage 13 y 14 Resolume 15 y 16///SOMOS AGUA
                                  {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71},// Canal 2 
                                      {60, 61, 62, 63, 64, 72, 76, 79, 83, 84, 72, 84}, // Canal 3..está
                                      {55, 59, 60, 57, 59, 45, 47, 48, 36, 38, 40, 41}, // Canal 4 .. está
                                       {35, 35, 35, 41, 43, 45, 47, 48, 36, 38, 13, 15}, // Canal 5 ..está percusión
                                      {60, 61, 62, 63, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6..está
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
  const uint8_t MODES16[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  
  //17 - LUZ FINAL
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
  
  //18- APLAUSOS 
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
  
  //Aplausos
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
  
  //Salida
//  const uint8_t NOTES20[ 9 ][ 12 ] = { {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 1
  //                                    {48, 52, 55, 59, 60, 48, 52, 55, 59, 60, 48, 60}, // Canal 2
    //                                  {72, 76, 79, 83, 84, 72, 76, 79, 83, 84, 72, 84}, // Canal 3
      //                                {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 40, 41}, // Canal 4
        //                              {36, 38, 40, 41, 43, 45, 47, 48, 36, 38, 13, 15},     // Canal 5 < -- Control
          //                            {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 6
            //                          {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 7
              //                        {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}, // Canal 8
                //                      {60, 64, 67, 71, 72, 60, 64, 67, 71, 72, 60, 72}};// Canal 9
 // const uint8_t MODES20[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
 
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
#define SATVAL -100

//Radio stuff
//const uint64_t PIPES[4] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL};
#define PINCE 9
#define PINCS 10
//#define BUFF 4
