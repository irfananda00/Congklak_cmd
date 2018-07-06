#include <HX711.h>

float GAMEREADY_BATASBAWAH = 3.5;
float GAMEREADY_BATASATAS = 4.9;

int lumbung_dipilih = 0;
int giliran_pemain = 0;

bool isGameReady = false;
bool isPeraturanDilanggar = false;
int GAME_STATE = 0;
// 0 = PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
// 1 = PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
// 2 = PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
// 3 = PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
// 4 = GANTI GILIRAN
// 5 = PERMAINAN SELESAI

//1-6 / n1-n6 = lumbung
//7 / A = Rumah pemain 1
//8 / B = Rumah pemain 2

HX711 cell1(22, 23);
HX711 cell2(24, 25);
HX711 cell3(27, 26);
HX711 cell4(29, 28);
HX711 cell5(30, 31);
HX711 cell6(33, 32);
HX711 cell7(35, 34);
HX711 cell8(36, 37);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(0));

  init();
}

void init(){
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

float wc1 = -440; //-440
float n1 = 0;
bool isn1_dilanggar = false;

float wc2 = -2191; //-2191
float n2 = 0;
bool isn2_dilanggar = false;

float wc3 = -440; //-440
float n3 = 0;
bool isn3_dilanggar = false;

float wc4 = -2061; //-2061
float n4 = 0;
bool isn4_dilanggar = false;

float wc5 = -1911; //-1911
float n5 = 0;
bool isn5_dilanggar = false;

float wc6 = -2080; //-1911
float n6 = 0;
bool isn6_dilanggar = false;

float wcA = -446; //-446
float A = 0;
float skor_pemain1 = 0;

float wcB = -447; //-447
float B = 0;
float skor_pemain2 = 0;

float count = 0;

void loop() {
  // put your main code here, to run repeatedly:

  detect_lumbung();  

  printValue();

  switch (GAME_STATE) {
    //PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
    case 0:
      //Periksa apakah lumbung sudah terisi biji
        if(isSudahSiapMain() && !isGameReady ){
            Serial.println("---------- PERMAINAN SUDAH BISA DIMULAI !!! -------------");
            isGameReady = true;
            
            //Menentukan giliran pemain
            int r = random(300);
            if(r<150){
                giliran_pemain = 1;
            }else{
                giliran_pemain = 2;
            }
            GAME_STATE = 1;
        }else{
            isGameReady = false;
        }
      break;
    //PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
    case 1:
      detect_lumbung_dipilih();
      break;
    //PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
    case 2:
      
      break;
    //PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
    case 3:
      
      break;
    //GANTI GILIRAN
    case 4:
      
      break;
    //PERMAINAN SELESAI
    case 5:
      
      break;
  }

  delay(500);
}

void detect_lumbung(){
    //DONE
    detectA();
    //DONE
    detectB();
    //DONE
    detect6();
    //DONE
    detect5();
    //DONE
    detect4();
    //DONE
    detect3();
    //DONE
    detect2();
    //DONE
    detect1();
}

void detect_lumbung_dipilih(){
  if(isGameReady && giliran_pemain!=0){
    //Pemain mengambil biji dari lumbung milik lawan main.
    if( n1<=0.8 && giliran_pemain = 2 ){        
        melanggar_aturan_bijikurang(1);
        isn1_dilanggar = true;
    }else if( n2<=0.8 && giliran_pemain = 2 ){
        melanggar_aturan_bijikurang(2);
        isn2_dilanggar = true;
    }else if( n3<=0.8 && giliran_pemain = 2 ){
        melanggar_aturan_bijikurang(3);
        isn3_dilanggar = true;
    }
    //Pemain mengambil biji dari lumbung milik lawan main.
    if( n4<=0.8 && giliran_pemain = 1 ){
        melanggar_aturan_bijikurang(4);
        isn4_dilanggar = true;
    }else if( n5<=0.8 && giliran_pemain = 1 ){
        melanggar_aturan_bijikurang(5);
        isn5_dilanggar = true;
    }else if( n6<=0.8 && giliran_pemain = 1 ){
        melanggar_aturan_bijikurang(6);
        isn6_dilanggar = true;
    }

    //Pemain mengambil biji dari rumah miliknya atau milik lawan main.
    if( A<=0.8 && skor_pemain1>0 ){
      melanggar_aturan_bijikurang(7);
    }else if( B<=0.8 && skor_pemain2>0 ){
      melanggar_aturan_bijikurang(8);
    }

    //Pemain tidak mengambil semua biji dari lumbung yang dipilih.
    if( n1>=0.8 && n1<=GAMEREADY_BATASBAWAH ){
      melanggar_aturan_bijilebih(1);
      isn1_dilanggar = true;
    }else if( n2>=0.8 && n2<=GAMEREADY_BATASBAWAH ){
      melanggar_aturan_bijilebih(2);
      isn2_dilanggar = true;
    }else if( n3>=0.8 && n3<=GAMEREADY_BATASBAWAH ){
      melanggar_aturan_bijilebih(3);
      isn3_dilanggar = true;
    }else if( n4>=0.8 && n4<=GAMEREADY_BATASBAWAH ){
      melanggar_aturan_bijilebih(4);
      isn4_dilanggar = true;
    }else if( n5>=0.8 && n5<=GAMEREADY_BATASBAWAH ){
      melanggar_aturan_bijilebih(5);
      isn5_dilanggar = true;
    }else if( n6>=0.8 && n6<=GAMEREADY_BATASBAWAH ){
      melanggar_aturan_bijilebih(6);
      isn6_dilanggar = true;
    }

    //Pemain mengambil biji dari dua lumbung atau lebih.
    

    //Mendeteksi lumbung yang dipilih pemain
    if( (n1<=0.8) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_dipilih = 1;
        reset_pelanggaran();
    }else if( (n2<=0.8) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_dipilih = 2;
        reset_pelanggaran();
    }else if( (n3<=0.8) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_dipilih = 3;
        reset_pelanggaran();
    }else if( (n4<=0.8) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_dipilih = 4;
        reset_pelanggaran();
    }else if( (n5<=0.8) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_dipilih = 5;
        reset_pelanggaran();
    }else if( (n6<=0.8) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) ){
        lumbung_dipilih = 6;
        reset_pelanggaran();
    }

  }
}

void reset_pelanggaran(){
    isn1_dilanggar = false;
    isn2_dilanggar = false;
    isn3_dilanggar = false;
    isn4_dilanggar = false;
    isn5_dilanggar = false;
    isn6_dilanggar = false;
}

void melanggar_aturan_bijikurang(int lumbung_yang_salah){
  Serial.print("---------- (-) BIJI KURANG PADA LUMBUNG : ");
  Serial.print(lumbung_yang_salah);
  Serial.println(" ------------");
  isPeraturanDilanggar = true;
}

void melanggar_aturan_bijilebih(int lumbung_yang_salah){
  Serial.print("---------- (+) BIJI LEBIH PADA LUMBUNG : ");
  Serial.print(lumbung_yang_salah);
  Serial.println(" ------------");
  isPeraturanDilanggar = true;
}

bool isSudahSiapMain(){
  if( ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
    return true;
  }else{
    return false;
  }
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
}

void detectA() {
  cell4.set_scale(wcA);

  A = cell4.get_units(), 10;
  if (A < 0) {
    A = 0.00;
  }
}

void detect6() {
  cell8.set_scale(wc6);

  n6 = cell8.get_units(), 10;
  if (n6 < 0) {
    n6 = 0.00;
  }
}

void detect5() {
  cell7.set_scale(wc5);

  n5 = cell7.get_units(), 10;
  if (n5 < 0) {
    n5 = 0.00;
  }
}

void detect4() {
  cell6.set_scale(wc4);

  n4 = cell6.get_units(), 10;
  if (n4 < 0) {
    n4 = 0.00;
  }
}

void detect3() {
  cell1.set_scale(wc3);

  n3 = cell1.get_units(), 10;
  if (n3 < 0) {
    n3 = 0.00;
  }
}

void detect2() {
  cell2.set_scale(wc2);

  n2 = cell2.get_units(), 10;
  if (n2 < 0) {
    n2 = 0.00;
  }
}

void detect1() {
  cell5.set_scale(wc1);

  n1 = cell5.get_units(), 10;
  if (n1 < 0) {
    n1 = 0.00;
  }
}

//  Serial.print(n1);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc1);
//  Serial.println();



