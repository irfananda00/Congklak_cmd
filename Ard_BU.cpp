#include <HX711.h>

HX711 cell4(23, 22);//D A
HX711 cell3(24, 25);//D B
HX711 cell5(27, 26);//D 1
HX711 cell2(41, 40);//N 2
HX711 cell1(30, 31);//D 3 
HX711 cell6(33, 32);//D 4
HX711 cell7(34, 35);//D 5
HX711 cell8(37, 36);//D 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(0));

  cell5.set_scale();
  cell5.tare();
  
  cell2.set_scale();
  cell2.tare();
  
  cell3.set_scale();
  cell3.tare();
  
  cell4.set_scale();
  cell4.tare();
  
  cell1.set_scale();
  cell1.tare();
  
  cell6.set_scale();
  cell6.tare();
  
  cell7.set_scale();
  cell7.tare();
  
  cell8.set_scale();
  cell8.tare();
}

float wc1 = -2191; //-2191
float n1 = 0;
bool isn1_dilanggar = false;

float wc2 = -2191; //-2191
float n2 = 0;
bool isn2_dilanggar = false;

float wc3 = -440; //-440
float n3 = 0;
bool isn3_dilanggar = false;

float wc4 = -1991; //-1991
float n4 = 0;
bool isn4_dilanggar = false;

float wc5 = -2059; //-2059
float n5 = 0;
bool isn5_dilanggar = false;

float wc6 = -2125; //-1911
float n6 = 0;
bool isn6_dilanggar = false;

float wcA = -440; //-440
float A = 0;
float skor_pemain1 = 0;

float wcB = -445; //-445
float B = 0;
float skor_pemain2 = 0;

float count = 0;

void loop() {
  // put your main code here, to run repeatedly:

    //DONE
    detectA();
    //DONE
    detectB();
    //
    detect6();
    //DONE
    detect5();
    //DONE
    detect4();
    //DONE
    detect3();
    //
    detect2();
    //DONE
    detect1();

//  printValue();

  delay(500);
}

void printValue(){
  Serial.print( " n1: " );
  Serial.print(n1);
  Serial.print( " # n2: " );
  Serial.print(n2);
  Serial.print( " # n3: " );
  Serial.print(n3);
  Serial.print( " # A: " );
  Serial.print(A);
  Serial.print( "  #|||# " );
  Serial.print( " # n4: " );
  Serial.print(n4);
  Serial.print( " # n5: " );
  Serial.print(n5);
  Serial.print( " # n6: " );
  Serial.print(n6);
  Serial.print( " # B: " );
  Serial.println(B);
}

void detectB() {
  cell3.set_scale(wcB);

  B = cell3.get_units(), 10;
  if (B < 0) {
    B = 0.00;
  }
//  Serial.print(B);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wcB);
//  Serial.println();
}

void detectA() {
  cell4.set_scale(wcA);

  A = cell4.get_units(), 10;
  if (A < 0) {
    A = 0.00;
  }
//  Serial.print(A);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wcA);
//  Serial.println();
}

void detect6() {
  cell8.set_scale(wc6);

  n6 = cell8.get_units(), 10;
  if (n6 < 0) {
    n6 = 0.00;
  }
//  Serial.print(n6);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc6);
//  Serial.println();
}

void detect5() {
  cell7.set_scale(wc5);

  n5 = cell7.get_units(), 10;
  if (n5 < 0) {
    n5 = 0.00;
  }
//  Serial.print(n5);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc5);
//  Serial.println();
}

void detect4() {
  cell6.set_scale(wc4);

  n4 = cell6.get_units(), 10;
  if (n4 < 0) {
    n4 = 0.00;
  }
//  Serial.print(n4);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc4);
//  Serial.println();
}

void detect3() {
  cell1.set_scale(wc3);

  n3 = cell1.get_units(), 10;
  if (n3 < 0) {
    n3 = 0.00;
  }
//  Serial.print(n3);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc3);
//  Serial.println();
}

void detect2() {
  cell2.set_scale(wc2);

  n2 = cell2.get_units(), 10;
  if (n2 < 0) {
    n2 = 0.00;
  }
  Serial.print(n2);
  Serial.print(" grams"); 
  Serial.print(" calibration_factor: ");
  Serial.print(wc2);
  Serial.println();
}

void detect1() {
  cell5.set_scale(wc1);

  n1 = cell5.get_units(), 10;
  if (n1 < 0) {
    n1 = 0.00;
  }
//  Serial.print(n1);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc1);
//  Serial.println();
}



