//Touch this
//

//Defines
 #define CONTROL true	// <-- Comentar para emisor
//#define DEBUG //Uncomment for debug

//Sensor Stuff
#ifndef CONTROL
  #define HEAD 4	  // <-- Hardcode value between 1 and 9 
  #define TOUCH 20
  #define RELEASE 15
  mpr121_proxmode_type  prox_mode = PROX_DISABLED; //PROX_DISABLED PROX_0_1 PROX_0_3 PROX_0_11 (no proximidad, 2 sensores, 4 sensores, 12 sensores)
#else
  #define HEAD 10                 // <-- Midi channel for control keypad
  #define CCNOTE 25
  #define SUSTAIN 64
  #define GOCHANNEL 1            //<-- Channel used for MEMRORY change
  #define ELEC_GO 0             //<-- Electrode used for MEMORY increase
  #define ELEC_GO_BACK 1        //<-- Electrode used for MEMORY decrease
  #define SUSTAIN_CHANGE 2        //<-- Electrode used for SUSTAIN change
  /*ARRAY de memorias:
   0 = No cambia nada
   1 = Cambia MainStageq
   2 = Cambia Resolume
   3 = Cambia todo
   */
   const uint8_t MEMORIAS[20] = {3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2}; 
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

  /*INICIO TELEGRAFO SEVILLA */
  const uint8_t NOTES1[ 6 ][ 6 ] = {  {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {55, 74, 72, 46, 46, 55}, // Canal 2 
                                      {79, 62, 43, 58, 58, 43}, // Canal 3
                                        {55, 74, 43, 46 , 46, 55},// Canal 4
                                      {67, 62, 48, 70, 70, 67}, // Canal 5
                                      {55, 50, 72, 58, 58, 55}};// Canal 6
  const uint8_t MODES1[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  
  /*PIEDRA SEVILLA */
  const uint8_t NOTES2[ 6 ][ 6 ] = { {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                       {55, 74, 72, 51, 46, 55}, // Canal 2 
                                      {43, 62, 60, 63, 58, 43}, // Canal 3
                                      {55, 74, 60, 51 , 46, 55},// Canal 4
                                      {67, 62, 48, 75, 70, 67}, // Canal 5
                                      {55, 50, 72, 63, 58, 55}};// Canal 6
  const uint8_t MODES2[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*RIO BUENO  SEVILLA */
  const uint8_t NOTES3[ 6 ][ 6 ] = {  {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {55, 74, 72, 46, 46, 55}, // Canal 2 
                                      {79, 62, 43, 58, 58, 43}, // Canal 3
                                        {55, 74, 43, 46 , 46, 55},// Canal 4
                                      {67, 62, 48, 70, 70, 67}, // Canal 5
                                      {55, 50, 72, 58, 58, 55}};// Canal 6 SEVILLA ARBOLITO 
  const uint8_t MODES3[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*TECHNO + MANZANO SEVILLA*/
  const uint8_t NOTES4[ 6 ][ 6 ] = { {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {55, 74, 72, 46, 46, 55}, // Canal 2 
                                      {79, 62, 43, 58, 58, 43}, // Canal 3
                                        {55, 74, 43, 46 , 46, 55},// Canal 4
                                      {67, 62, 48, 70, 70, 67}, // Canal 5
                                      {55, 50, 72, 58, 58, 55}};// Canal 6
  const uint8_t MODES4[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  
  /*VIDEO SEÑOR VIVERO + SOMOS AGUA SEVILLA */
  const uint8_t NOTES5[ 6 ][ 6 ] = { {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {55, 74, 72, 46, 46, 55}, // Canal 2 
                                      {79, 62, 43, 58, 58, 43}, // Canal 3
                                        {55, 74, 43, 46 , 46, 55},// Canal 4
                                      {67, 62, 48, 70, 70, 67}, // Canal 5
                                      {55, 50, 72, 58, 58, 55}};// Canal 6
  const uint8_t MODES5[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random

  
  /*VIDEO SEÑOR VIVERO + SOMOS AGUA SEVILLA */
  const uint8_t NOTES6[ 6 ][ 6 ] = {  {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {55, 74, 72, 46, 46, 55}, // Canal 2 
                                      {79, 62, 43, 58, 58, 43}, // Canal 3
                                        {55, 74, 43, 46 , 46, 55},// Canal 4
                                      {67, 62, 48, 70, 70, 67}, // Canal 5
                                      {55, 50, 72, 58, 58, 55}};// Canal 6
  const uint8_t MODES6[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*FIESTA DEL ARBOL -  Techno*/
  const uint8_t NOTES7[ 6 ][ 6 ] = { {0, 0, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {55, 74, 72, 46, 46, 55}, // Canal 2 
                                      {79, 62, 43, 58, 58, 43}, // Canal 3
                                        {55, 74, 43, 46 , 46, 55},// Canal 4
                                      {67, 62, 48, 70, 70, 67}, // Canal 5
                                      {55, 50, 72, 58, 58, 55}};// Canal 6
  const uint8_t MODES7[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*VIDEO ARBOL + CANCION CORTAR UN ARBOL - Arbol */
  const uint8_t NOTES8[ 6 ][ 6 ] = {  {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {48, 52, 55, 59, 60, 64}, // Canal 2 
                                      {60, 64, 67, 71, 72, 60}, // Canal 3
                                      {48, 52, 55, 59, 60, 64}, // Canal 4
                                      {60, 64, 67, 71, 72, 60}, // Canal 5
                                      {36, 40, 43, 47, 48, 52}};// Canal 6
  const uint8_t MODES8[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
    /*VIDEO ARBOL + CANCION CORTAR UN ARBOL - Arbol */
  const uint8_t NOTES9[ 6 ][ 6 ] = {  {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {48, 52, 55, 59, 60, 64}, // Canal 2 
                                      {60, 64, 67, 71, 72, 60}, // Canal 3
                                      {48, 52, 55, 59, 60, 64}, // Canal 4
                                      {60, 64, 67, 71, 72, 60}, // Canal 5
                                      {36, 40, 43, 47, 48, 52}};// Canal 6
  const uint8_t MODES9[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  
  
  /*PIEDRAS - Violencia */
  const uint8_t NOTES10[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {48, 52, 55, 59, 60, 64}, // Canal 2 
                                      {60, 64, 67, 71, 72, 60}, // Canal 3
                                      {48, 52, 55, 59, 60, 64}, // Canal 4
                                      {60, 64, 67, 71, 72, 60}, // Canal 5
                                      {36, 40, 43, 47, 48, 52}};// Canal 6
  const uint8_t MODES10[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*EXPEDICION TARDIGRADO + TEXTO DE CRISTINA - Expedición */
  const uint8_t NOTES11[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {48, 52, 55, 59, 60, 64}, // Canal 2 
                                      {60, 64, 67, 71, 72, 60}, // Canal 3
                                      {48, 52, 55, 59, 60, 64}, // Canal 4
                                      {60, 64, 67, 71, 72, 60}, // Canal 5
                                      {36, 40, 43, 47, 48, 52}};// Canal 6
  const uint8_t MODES11[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*TEXTO COLEGIO en CAMBIO DE ENERGIA -  Escuela */
  const uint8_t NOTES12[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {48, 52, 55, 59, 60, 64}, // Canal 2 
                                      {60, 64, 67, 71, 72, 60}, // Canal 3
                                      {48, 52, 55, 59, 60, 64}, // Canal 4
                                      {60, 64, 67, 71, 72, 60}, // Canal 5
                                      {36, 40, 43, 47, 48, 52}};// Canal 6
  const uint8_t MODES12[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*SOMOS AGUA - Somos Agua */
  const uint8_t NOTES13[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES13[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*TEXTO MARIA - Ojos Cerrados */
  const uint8_t NOTES14[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES14[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*VIDEO BBC - BBC */
  const uint8_t NOTES15[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES15[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*RIO BUENO -  Rio Bueno*/
  const uint8_t NOTES16[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES16[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*DAVID APAGA LUZ - Luz Final */
  const uint8_t NOTES17[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES17[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*APLAUSOS - Aplausos */
  const uint8_t NOTES18[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES18[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*COLOQUIO - Coloquio */
  const uint8_t NOTES19[ 6 ][ 6 ] = { {60, 64, 0, 0, 0, 0},     // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {35, 35, 35, 41, 43, 45}, // Canal 2 
                                      {35, 35, 35, 41, 43, 45}, // Canal 3
                                      {60, 61, 62, 63, 64, 72}, // Canal 4
                                      {60, 61, 62, 63, 64, 65}, // Canal 5
                                      {60, 61, 62, 63, 64, 65}};// Canal 6
  const uint8_t MODES19[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random
  /*NADA - Nada */
  const uint8_t NOTES20[ 6 ][ 6 ] = { {0, 0, 0, 0, 0, 0},   // Canal 1   < -- Mainstage 61 y 65 Resolume 60 y 64 
                                      {0, 0, 0, 0, 0, 0},   // Canal 2 
                                      {0, 0, 0, 0, 0, 0},   // Canal 3
                                      {0, 0, 0, 0, 0, 0},   // Canal 4
                                      {0, 0, 0, 0, 0, 0},   // Canal 5
                                      {0, 0, 0, 0, 0, 0}};  // Canal 6
  const uint8_t MODES20[6] = {0, 0, 0, 0, 0, 0};      //<-- Electrodes used for mode control (5 modes) // 0 is note on-off, 1 is notes with Control Change, 2 is proximity, 3 is counter, 4 is random

#endif

const uint8_t NOTE_OFF = 0x80;          //<-- Used Midi message types
const uint8_t NOTE_ON = 0x90;           //<-- Used Midi message types
const uint8_t CC = 0xB0;                //<-- Used Midi message types

//Do NOT touch this
//

//Sensor stuf
#define I2CADDR 0x5A
#define WINDOW 25
#define SENSORS 7 // number of sensors + proximity
#define MINPROX -25
#define MAXPROX 6
#define SATVAL -100

//Radio stuff
//const uint64_t PIPES[4] {0xF0F0F0F0D2LL,  0xF0F0F0F0C3LL, 0xF0F0F0F0B4LL, 0xF0F0F0F0A5LL};
#define PINCE 9
#define PINCS 10
//#define BUFF 4
