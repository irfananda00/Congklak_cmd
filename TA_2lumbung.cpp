#include <HX711.h>
#include <math.h> 

float GAMEREADY_BATASBAWAH = 3.1;
float GAMEREADY_BATASATAS = 5.3;
float GAMEREADY_BATAS = 3;

float AVG_BIJI = 1.4;

int lumbung_sekarang = 0;
int giliran_pemain = 0;
float biji_ditangan = 0;

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

HX711 cell4(23, 22);//D A
HX711 cell3(24, 25);//D B
HX711 cell5(27, 26);//D 1
HX711 cell2(41, 40);//N 2
HX711 cell1(30, 31);//D 3 
HX711 cell6(33, 32);//N 4
HX711 cell7(34, 35);//D 5
HX711 cell8(37, 36);//D 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(0));

  initial_state();
}

void initial_state(){
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
float r1 = 0;
float pr1 = 0;
float p1 = 0;
bool isn1_dilanggar = false;

float wc2 = -2191; //-2191
float n2 = 0;
float r2 = 0;
float pr2 = 0;
float p2 = 0;
bool isn2_dilanggar = false;

float wc3 = -440; //-440
float n3 = 0;
float r3 = 0;
float pr3 = 0;
float p3 = 0;
bool isn3_dilanggar = false;

float wc4 = -1991; //-1991
float n4 = 0;
float r4 = 0;
float pr4 = 0;
float p4 = 0;
bool isn4_dilanggar = false;

float wc5 = -2059; //-2059
float n5 = 0;
float r5 = 0;
float pr5 = 0;
float p5 = 0;
bool isn5_dilanggar = false;

float wc6 = -2125; //-2125
float n6 = 0;
float r6 = 0;
float pr6 = 0;
float p6 = 0;
bool isn6_dilanggar = false;

float wcA = -440; //-440
float A = 0;
float rA = 0;
float prA = 0;
float pA = 0;

float wcB = -445; //-445
float B = 0;
float rB = 0;
float prB = 0;
float pB = 0;

float count = 0;

void loop() {
  // put your main code here, to run repeatedly:

  detect_lumbung();  

  printValue();

  switch (GAME_STATE) {
    //PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
    case 0:
      Serial.println("---------- PERSIAPAN PERMAINAN !!! -------------");
      //Periksa apakah lumbung sudah terisi biji
        if(isSudahSiapMain()){
            Serial.println("---------- PERMAINAN SUDAH BISA DIMULAI !!! -------------");
            
            //Menentukan giliran pemain
            int r = random(300);
            if(r<150){
                Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
                giliran_pemain = 1;
            }else{
                Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
                giliran_pemain = 2;
            }
            GAME_STATE = 1;

            //init p values 
            if( (p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) ){
                p1 = n1;
                p2 = n2;
                p3 = n3;
                p4 = n4;
                p5 = n5;
                p6 = n6;
            }
        }
      break;
    //PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
    case 1:
        Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG !!! -------------");
        //jika biji masuk ke rumah pemain
        if(lumbung_sekarang == 7 && giliran_pemain == 1){
            //TODO: tampilkan yang boleh dipilih rumah 1 2 3
            detect_lumbung_dipilih();    
        }else if(lumbung_sekarang == 8 && giliran_pemain == 2){
            //TODO: tampilkan yang boleh dipilih rumah 4 5 6
            detect_lumbung_dipilih(); 
        }else{
            //TEMPORARY
            detect_lumbung_dipilih();
        }
        break;
    //PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
    case 2:
        Serial.println("---------- MASUKKAN BIJI KE LUMBUNG SELANJUTNYA !!! -------------");
        detect_if_biji_dimasukkan();
        break;
    //PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
    case 3:
        Serial.println("---------- ANDA BERHASIL MENEMBAK !!! SILAHKAN MASUKKAN BIJINYA KE RUMAH !!! -------------");
        detect_if_tembak_diambil();
        break;
    //GANTI GILIRAN
    case 4:
        Serial.println("---------- GANTI GILIRAN !!! -------------");
        ganti_giliran();
        break;
    //PERMAINAN SELESAI
    case 5:
        Serial.println("---------- PERMAINAN SELESAI !!! -------------");
        permainan_selesai();
        break;
  }

//  p1 = n1;
//  p2 = n2;
//  p3 = n3;
//  p4 = n4;
//  p5 = n5;
//  p6 = n6;

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

void permainan_selesai(){
    Serial.println("---------- PERMAINAN SELESAI !!! -------------");
    if(A > B){
        Serial.println("---------- PEMENANG : PEMAIN 1 -------------");
    }else if(A < B){
        Serial.println("---------- PEMENANG : PEMAIN 2 -------------");
    }
}

void detect_if_tembak_diambil(){
    if( lumbung_sekarang==1 ){
        if( (n1==0) && (n6==0) && (pA + (p1+p6) == A) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==3 ){
        if( (n3==0) && (n5==0) && (pA + (p3+p5) == A) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==5 ){
        if( (n5==0) && (n3==0) && (pB + (p3+p5) == B) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==6 ){
        if( (n6==0) && (n1==0) && (pB + (p1+p6) == B) ){
            GAME_STATE = 4;
        }
    }
}

void ganti_giliran(){
    if(giliran_pemain == 1){
        giliran_pemain == 2;
        Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
    }else if(giliran_pemain == 2){
        giliran_pemain == 1;
        Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
    }
    check_game_selesai();
}

void check_game_selesai(){
    if( (n1==0) && (n3==0) && (n5==0) && (n6==0) ){
        //permainan selesai karena biji pada lumbung sudah habis
        GAME_STATE = 5;
    }else{
        GAME_STATE = 1;
    }
}

void detect_lumbung_dipilih(){
    //Mendeteksi lumbung yang dipilih pemain
    if( (n1==0) && (n3==GAMEREADY_BATAS) && (n5==GAMEREADY_BATAS) && (n6==GAMEREADY_BATAS) ){
        lumbung_sekarang = 1;
        biji_ditangan = p1;
        p1 = n1;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 1 DIPILIH !!! -------------");
    }else if( (n3==0) && (n1==GAMEREADY_BATAS) && (n5==GAMEREADY_BATAS) && (n6==GAMEREADY_BATAS) ){
        lumbung_sekarang = 3;
        biji_ditangan = p3;
        p3 = n3;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    }else if( (n5==0) && (n1==GAMEREADY_BATAS) && (n3==GAMEREADY_BATAS) && (n6==GAMEREADY_BATAS) ){
        lumbung_sekarang = 5;
        biji_ditangan = p5;
        p5 = n5;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    }else if( (n6==0) && (n1==GAMEREADY_BATAS) && (n3==GAMEREADY_BATAS) && (n5==GAMEREADY_BATAS) ){
        lumbung_sekarang = 6;
        biji_ditangan = p6;
        p6 = n6;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 6 DIPILIH !!! -------------");
    }
}

void detect_if_biji_dimasukkan(){
    //deteksi jika 1 biji dimasukkan ke lumbung selanjutnya
    if(biji_ditangan == 0){
        biji_ditangan = 0;
        GAME_STATE = 1;
    }else{
        if( (p1+1 == n1) && (lumbung_sekarang == 6) && giliran_pemain==1 ){
            if( (giliran_pemain==1) && (p1==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p1==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 1;
            p1 = n1;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 1 !!! -------------");
        }else if( (p3+1 == n3) && (lumbung_sekarang == 1) ){
            if( (giliran_pemain==1) && (p3==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p3==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 3;
            p3 = n3;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
        }else if( (p5+1 == n5) && (lumbung_sekarang == 3) && giliran_pemain==2 ){
            if( (giliran_pemain==2) && (p5==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p5==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 5;
            p5 = n5;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 5 !!! -------------");
        }else if( (p6+1 == n6) && (lumbung_sekarang == 5) ){
            if( (giliran_pemain==2) && (p6==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p6==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 6;
            p6 = n6;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 6 !!! -------------");
        }else if( (pA+1 == A) && (lumbung_sekarang == 3) && giliran_pemain==1 ){
            lumbung_sekarang = 7;
            pA = A;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = A !!! -------------");
        }else if( (pB+1 == B) && (lumbung_sekarang == 6) && giliran_pemain==2 ){
            lumbung_sekarang = 8;
            pB = B;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = B !!! -------------");
        }else if( (p5+1 == n5) && (lumbung_sekarang == 7) && giliran_pemain==1 ){
            if( (giliran_pemain==1) && (p5==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 5;
            p5 = n5;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 5 !!! -------------");
        }else if( (p1+1 == n1) && (lumbung_sekarang == 8) && giliran_pemain==2 ){
            if( (giliran_pemain==2) && (p1==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 1;
            p1 = n1;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 1 !!! -------------");
        }
    }
}

bool isSudahSiapMain(){
    if( (n1==GAMEREADY_BATAS) && (n3==GAMEREADY_BATAS) && (n5==GAMEREADY_BATAS) && (n6==GAMEREADY_BATAS) ){
        return true;
    }else{
        return false;
    }
}

void printValue(){
  Serial.print( "Biji ditangan: " );
  Serial.println(biji_ditangan);
  
  Serial.print( " r1: " );
  Serial.print(r1);
  Serial.print( " # r2: " );
  Serial.print(r2);
  Serial.print( " # r3: " );
  Serial.print(r3);
  Serial.print( " # rA: " );
  Serial.print(rA);
  Serial.print( "  #|||# " );
  Serial.print( " # r4: " );
  Serial.print(r4);
  Serial.print( " # r5: " );
  Serial.print(r5);
  Serial.print( " # r6: " );
  Serial.print(r6);
  Serial.print( " # rB: " );
  Serial.println(rB);
  
  Serial.print( " p1: " );
  Serial.print(p1);
  Serial.print( " # p2: " );
  Serial.print(p2);
  Serial.print( " # p3: " );
  Serial.print(p3);
  Serial.print( " # pA: " );
  Serial.print(pA);
  Serial.print( "  #|||# " );
  Serial.print( " # p4: " );
  Serial.print(p4);
  Serial.print( " # p5: " );
  Serial.print(p5);
  Serial.print( " # p6: " );
  Serial.print(p6);
  Serial.print( " # pB: " );
  Serial.println(pB);
  
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

  rB = cell3.get_units(), 10;
  if (rB < 0) {
    rB = 0.00;
  }
  B = floor(rB);
}

void detectA() {
  cell4.set_scale(wcA);

  rA = cell4.get_units(), 10;  
  if (rA < 0) {
    rA = 0.00;
  }
  A = floor(rA);
}

void detect6() {
  cell8.set_scale(wc6);

  r6 = cell8.get_units(), 10;
  if (r6 < 0) {
    r6 = 0.00;
  }
  n6 = floor(r6);
}

void detect5() {
  cell7.set_scale(wc5);

  r5 = cell7.get_units(), 10;
  if (r5 < 0) {
    r5 = 0.00;
  }
  n5 = floor(r5);
}

void detect4() {
  cell6.set_scale(wc4);

  r4 = cell6.get_units(), 10;  
  if (r4 < 0) {
    r4 = 0.00;
  }
  n4 = floor(r4);
}

void detect3() {
  cell1.set_scale(wc3);

  r3 = cell1.get_units(), 10;
  if (r3 < 0) {
    r3 = 0.00;
  }
  n3 = floor(r3);
}

void detect2() {
  cell2.set_scale(wc2);

  r2 = cell2.get_units(), 10;
  if (r2 < 0) {
    r2 = 0.00;
  }
  n2 = floor(r2);
}

void detect1() {
  cell5.set_scale(wc1);

  r1 = cell5.get_units(), 10;
  if (r1 < 0) {
    r1 = 0.00;
  }
  n1 = floor(r1);
}

//  Serial.print(n1);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc1);
//  Serial.println();



