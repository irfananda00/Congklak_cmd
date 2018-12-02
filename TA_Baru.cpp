#include <HX711.h>
#include <math.h>
#include <MultiShiftRegister.h>

int BIJI_YANG_DIGUNAKAN = 3;
int TEMP_BIJI = 3;
float TOLERANSI = 4.25;

float BATAS_BAWAH = 100.1;
float BATAS_ATAS = 100.3;

float AVG_BIJI = 5;

int lumbung_sekarang = 0;
int lumbung_lanjut = 0;

int giliran_pemain = 0;

int biji_ditangan = 0;
bool lumbung_terpilih = false;

int lA = 15;
int lB = 16;
//1-6 / n1-n6 = lumbung
//15 / A = Rumah pemain 1
//16 / B = Rumah pemain 2

 int latchPin = 8; //8 STCP GREEN
 int dataPin = 11; //11 DS BLUE
 int clockPin = 12; //12 SHCP YELLOW

 int playerAturn = 7;
 int playerBturn = 6;

 int numberOfRegisters = 3;

 MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin);

int GAME_STATE = 0;
// -1 = KALIBRASI BERAT SENSOR
// 0 = PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
// 1 = PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
// 2 = PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
// 3 = PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
// 4 = GANTI GILIRAN
// 5 = PERMAINAN SELESAI

HX711 cell12(26, 27);//
HX711 cell11(28, 29);//
HX711 cell10(30, 31);//
HX711 cell5(40, 41);//
HX711 cell4(42, 43);//
HX711 cell3(44, 45);//
HX711 cellA(50, 51);//
HX711 cellB(52, 53);//

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  msr.shift();

  pinMode(playerAturn, OUTPUT);
  pinMode(playerBturn, OUTPUT);

  randomSeed(analogRead(0));

  initial_state();
}

void initial_state() {
  cell3.set_scale();
  cell3.tare();
  cell4.set_scale();
  cell4.tare();
  cell5.set_scale();
  cell5.tare();
  cell10.set_scale();
  cell10.tare();
  cell11.set_scale();
  cell11.tare();
  cell12.set_scale();
  cell12.tare();
  cellA.set_scale();
  cellA.tare();
  cellB.set_scale();
  cellB.tare();
}

int loopt = 0;

int wc3 = -900; //-900
int n3 = 0;
float r3 = 0;
bool rt3 = false;
int p3 = 0;

int wc4 = -2201; //-2200
int n4 = 0;
float r4 = 0;
bool rt4 = false;
int p4 = 0;

int wc5 = -1161; //-1160
int n5 = 0;
float r5 = 0;
bool rt5 = false;
int p5 = 0;

int wc10 = -766; //-766
int n10 = 0;
float r10 = 0;
bool rt10 = false;
int p10 = 0;

int wc11 = -866; //-866
int n11 = 0;
float r11 = 0;
bool rt11 = false;
int p11 = 0;

int wc12 = -885; //-885
int n12 = 0;
float r12 = 0;
bool rt12 = false;
int p12 = 0;

int wcA = -807; //-807
int A = 0;
float rA = 0;
bool rtA = false;
int pA = 0;

int wcB = -849; //-849
int B = 0;
float rB = 0;
bool rtB = false;
int pB = 0;

void loop() {

  detect_lumbung();

  printValue();

  if (Serial.available() > 0) {
    char inp = Serial.read();
    if(inp == '0'){
        clear_all_green();
        GAME_STATE = 0;
        Serial.print( "wc3: " );
        Serial.println(wc3);
        Serial.print( "wc4: " );
        Serial.println(wc4);
        Serial.print( "wc5: " );
        Serial.println(wc5);
        Serial.print( "wc10: " );
        Serial.println(wc10);
        Serial.print( "wc11: " );
        Serial.println(wc11);
        Serial.print( "wc12: " );
        Serial.println(wc12);
        Serial.print( "wcA: " );
        Serial.println(wcA);
        Serial.print( "wcB: " );
        Serial.println(wcB);
    }
  }

//  msr.set_shift(1);

  congklak_game();

//  delay(200);
}

void congklak_game() {
  switch (GAME_STATE) {
    case -1:
//    Serial.println("---------- KALIBRASI BERAT !!! -------------");
    //KALIBRASI BERAT
      if( (r3>=BATAS_BAWAH) && (r3<BATAS_ATAS) ){
        rt3 = true;
        msr.set_shift(20);
        Serial.print( "FIXED wc3: " );
        Serial.println(wc3);
      }else if(!rt3 && r3>BATAS_ATAS){
        wc3 = wc3 - 1;
        msr.clear_shift(20);
      }else if(!rt3 && r3<BATAS_BAWAH){
        wc3 = wc3 + 1;
        msr.clear_shift(20);
      }
      if( (r4>=BATAS_BAWAH) && (r4<BATAS_ATAS) ){
        rt4 = true;
        msr.set_shift(21);
        Serial.print( "FIXED wc4: " );
        Serial.println(wc4);
      }else if(!rt4 && r4>BATAS_ATAS){
        wc4 = wc4 - 1;
        msr.clear_shift(21);
      }else if(!rt4 && r4<BATAS_BAWAH){
        wc4 = wc4 + 1;
        msr.clear_shift(21);
      }
      if( (r5>=BATAS_BAWAH) && (r5<BATAS_ATAS) ){
        rt5 = true;
        msr.set_shift(17);
        Serial.print( "FIXED wc5: " );
        Serial.println(wc5);
      }else if(!rt5 && r5>BATAS_ATAS){
        wc5 = wc5 - 1;
        msr.clear_shift(17);
      }else if(!rt5 && r5<BATAS_BAWAH){
        wc5 = wc5 + 1;
        msr.clear_shift(17);
      }
      if( (r10>=BATAS_BAWAH) && (r10<BATAS_ATAS) ){
        rt10 = true;
        msr.set_shift(11);
        Serial.print( "FIXED wc10: " );
        Serial.println(wc10);
      }else if(!rt10 && r10>BATAS_ATAS){
        wc10 = wc10 - 1;
        msr.clear_shift(11);
      }else if(!rt10 && r10<BATAS_BAWAH){
        wc10 = wc10 + 1;
        msr.clear_shift(11);
      }
      if( (r11>=BATAS_BAWAH) && (r11<BATAS_ATAS) ){
        rt11 = true;
        msr.set_shift(2);
        Serial.print( "FIXED wc11: " );
        Serial.println(wc11);
      }else if(!rt11 && r11>BATAS_ATAS){
        wc11 = wc11 - 1;
        msr.clear_shift(2);
      }else if(!rt11 && r11<BATAS_BAWAH){
        wc11 = wc11 + 1;
        msr.clear_shift(2);
      }
      if( (r12>=BATAS_BAWAH) && (r12<BATAS_ATAS) ){
        rt12 = true;
        msr.set_shift(7);
        Serial.print( "FIXED wc12: " );
        Serial.println(wc12);
      }else if(!rt12 && r12>BATAS_ATAS){
        wc12 = wc12 - 1;
        msr.clear_shift(7);
      }else if(!rt12 && r12<BATAS_BAWAH){
        wc12 = wc12 + 1;
        msr.clear_shift(7);
      }
      if( (rA>=BATAS_BAWAH) && (rA<BATAS_ATAS) ){
        rtA = true;
        msr.set_shift(12);
        Serial.print( "FIXED wcA: " );
        Serial.println(wcA);
      }else if(!rtA && rA>BATAS_ATAS){
        wcA = wcA - 1;
        msr.clear_shift(12);
      }else if(!rtA && rA<BATAS_BAWAH){
        wcA = wcA + 1;
        msr.clear_shift(12);
      }
      if( (rB>=BATAS_BAWAH) && (rB<BATAS_ATAS) ){
        rtB = true;
        msr.set_shift(4);
        Serial.print( "FIXED wcB: " );
        Serial.println(wcB);
      }else if(!rtB && rB>BATAS_ATAS){
        wcB = wcB - 1;
        msr.clear_shift(4);
      }else if(!rtB && rB<BATAS_BAWAH){
        wcB = wcB + 1;
        msr.clear_shift(4);
      }
      if( rt3 &&
          rt4 &&
          rt5 &&
          rt10 &&
          rt11 &&
          rt12 &&
          rtA &&
          rtB 
          ){
        clear_all_green();
        GAME_STATE = 0;
        Serial.print( "wc3: " );
        Serial.println(wc3);
        Serial.print( "wc4: " );
        Serial.println(wc4);
        Serial.print( "wc5: " );
        Serial.println(wc5);
        Serial.print( "wc10: " );
        Serial.println(wc10);
        Serial.print( "wc11: " );
        Serial.println(wc11);
        Serial.print( "wc12: " );
        Serial.println(wc12);
        Serial.print( "wcA: " );
        Serial.println(wcA);
        Serial.print( "wcB: " );
        Serial.println(wcB);
      }
      break;
    case 0:
    //PERSIAPAN PERMAINAN, memasukkan 3 biji ke tiap lumbung
      digitalWrite(playerAturn, LOW);
      digitalWrite(playerBturn, LOW);
      Serial.println("---------- PERSIAPAN PERMAINAN !!! -------------");
      //Periksa apakah lumbung sudah terisi biji
      if (isSudahSiapMain()) {
        Serial.println("---------- PERMAINAN SUDAH BISA DIMULAI !!! -------------");

        //Menentukan giliran pemain
        int r = random(300);
        if (r < 150) {
          Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
          giliran_pemain = 1;
          digitalWrite(playerAturn, HIGH);
          digitalWrite(playerBturn, LOW);
          //nyala giliran 1
        } else {
          Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
          giliran_pemain = 2;
          digitalWrite(playerAturn, LOW);
          digitalWrite(playerBturn, HIGH);
          //nyala giliran 2
        }

        GAME_STATE = 1;

        //init p values
        if ( 
             (p3 == 0) &&
             (p4 == 0) &&
             (p5 == 0) &&
             (p10 == 0) &&
             (p11 == 0) &&
             (p12 == 0) 
           ) {
          p3 = n3;
          p4 = n4;
          p5 = n5;
          p10 = n10;
          p11 = n11;
          p12 = n12;
        }
      }
      break;
    case 1:
    //PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
      // && (n1!=0 || n2!=0 || n3!=0 || n4!=0 || n5!=0 || n6!=0 || n7!=0)
      // && (n8!=0 || n9!=0 || n10!=0 || n11!=0 || n12!=0 || n13!=0 || n14!=0)
      if (check_giliran_selesai() && lumbung_terpilih) {
        GAME_STATE = 4;
        lumbung_terpilih = false;
      } else if ( (lumbung_sekarang == lA && giliran_pemain == 1) ||
                  (lumbung_sekarang == lB && giliran_pemain == 2) ||
                  (lumbung_sekarang == 0 && giliran_pemain == 1) ||
                  (lumbung_sekarang == 0 && giliran_pemain == 2) ) {                    
        lumbung_terpilih = false;
        if (giliran_pemain == 1) {          
          if (n3 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 3 -------------");
            msr.set_shift(20);
            //  nyala hijau
          }
          if (n4 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 4 -------------");
            msr.set_shift(21);
            //  nyala hijau
          }
          if (n5 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 5 -------------");
            msr.set_shift(17);
            //  nyala hijau
          }
        } else {
          if (n10 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 10 -------------");
            msr.set_shift(11);
            //  nyala hijau
          }
          if (n11 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 11 -------------");
            msr.set_shift(2);
            //  nyala hijau
          }
          if (n12 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 12 -------------");
            msr.set_shift(7);
            //  nyala hijau
          }
        }
        //  tampilkan yang boleh dipilih lumbung
        detect_lumbung_dipilih_dari_rumah();
      } else {
        //  tampilkan yang boleh dipilih lumbung lumbung_sekarang
        Serial.print("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : ");
        Serial.print(lumbung_sekarang);
        Serial.println(" -------------");
        if(lumbung_sekarang==3){
          msr.set_shift(20);
          //  nyala hijau
        }
        else if(lumbung_sekarang==4){
          msr.set_shift(21);
          //  nyala hijau
        }
        else if(lumbung_sekarang==5){
          msr.set_shift(17);
          //  nyala hijau
        }
        else if(lumbung_sekarang==10){
          msr.set_shift(11);
          //  nyala hijau
        }
        else if(lumbung_sekarang==11){
          msr.set_shift(2);
          //  nyala hijau
        }
        else if(lumbung_sekarang==12){
          msr.set_shift(7);
          //  nyala hijau
        }
        detect_lumbung_dipilih();
      }
      break;
    case 2:
    //PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
      // Serial.println("---------- MASUKKAN BIJI KE LUMBUNG SELANJUTNYA !!! -------------");
      show_next_lumbung();
      detect_if_biji_dimasukkan();
      break;
    case 3:
    //PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
      Serial.println("---------- ANDA BERHASIL MENEMBAK !!! SILAHKAN MASUKKAN BIJINYA KE RUMAH !!! -------------");
      detect_if_tembak_diambil();
      break;
    case 4:
    //GANTI GILIRAN
      Serial.println("---------- GANTI GILIRAN !!! -------------");
      ganti_giliran();
      break;
    case 5:
    //PERMAINAN SELESAI
      Serial.println("---------- PERMAINAN SELESAI !!! -------------");
      permainan_selesai();
      break;
  }
}

void detect_lumbung() {
  if( (BIJI_YANG_DIGUNAKAN==3) ){
    detect3();
    detect12();
    detect4();
    detect11();
    detect5();
    detect10();
  }
  detectA();
  detectB();
}


void permainan_selesai() {
  digitalWrite(playerAturn, LOW);
  digitalWrite(playerBturn, LOW);
  Serial.println("---------- PERMAINAN SELESAI !!! -------------");
  if (A > B) {
    msr.set_shift(12);
    delay(200);
    msr.clear_shift(12);
    delay(200);
    Serial.println("---------- PEMENANG : PEMAIN 1 -------------");
  } else if (A < B) {
    msr.set_shift(4);
    delay(200);
    msr.clear_shift(4);
    delay(200);
    Serial.println("---------- PEMENANG : PEMAIN 2 -------------");
  }
}

void show_next_lumbung() {  
  // A / 15
  if ( lumbung_sekarang == lA ) {
    if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
      lumbung_lanjut = 10;
      msr.set_shift(10);
      //  nyala kuning
    }
  }
  // B / 16
  else if ( lumbung_sekarang == lB ) {
    if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
      lumbung_lanjut = 3;
      msr.set_shift(18);
      //  nyala kuning
    }
  } 
  // 1-6
  else if ( (lumbung_sekarang == 3) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 4;
    msr.set_shift(22);
    //  nyala kuning
  } else if ( (lumbung_sekarang == 4) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 5;
    msr.set_shift(16);
    //  nyala kuning
  } else if ( (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = lA;
    msr.set_shift(13);
    //  nyala kuning
  } else if ( (lumbung_sekarang == 5) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 10;
    msr.set_shift(10);
    //  nyala kuning
  }
  // 8-13
  else if ( (lumbung_sekarang == 10) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 11;
    msr.set_shift(0);
    //  nyala kuning
  } else if ( (lumbung_sekarang == 11) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 12;
    msr.set_shift(8);
    //  nyala kuning
  } else if ( (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = lB;
    msr.set_shift(3);
    //  nyala kuning
  } else if ( (lumbung_sekarang == 12) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 3;
    msr.set_shift(18);
    //  nyala kuning
  }
  // else {
  //   lumbung_lanjut = lumbung_sekarang + 1;
  // }
  Serial.print("---------- MASUKKAN BIJI KE LUMBUNG : ");
  Serial.print(lumbung_lanjut);
  Serial.println(" -------------");
}
// TODO: Perbaiki aturan
void detect_if_tembak_diambil() {
  if ( lumbung_sekarang == 3 || lumbung_sekarang == 12 ) {        
    if (n3 == 0){
      msr.clear_shift(20);
    }else{
      msr.set_shift(20);
    }
    if (n12 == 0){
      msr.clear_shift(7);
    }else{
      msr.set_shift(7);
    }
    //  nyala hijau
    //  nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 3 dan 12 -------------");
    if ( (n3 == 0) && (n12 == 0) ) {
      if (giliran_pemain == 1) {
        msr.set_shift(13);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p3 + p12) == A) ) {          
          msr.clear_shift(13);
          //  semua mati
          p3 = n3;
          p12 = n12;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {        
        msr.set_shift(3);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p3 + p12) == B) ) {          
          msr.clear_shift(3);
          //  semua mati
          p3 = n3;
          p12 = n12;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
    check_aturan_tembak(3,12);
  } else if ( lumbung_sekarang == 4 || lumbung_sekarang == 11 ) {
    if (n4 == 0){
      msr.clear_shift(21);
    }else{
      msr.set_shift(21);
    }
    if (n11 == 0){
      msr.clear_shift(2);
    }else{
      msr.set_shift(2);
    }
    //  nyala hijau
    //  nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 4 dan 11 -------------");
    if ( (n4 == 0) && (n11 == 0) ) {
      if (giliran_pemain == 1) {
        msr.set_shift(13);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p4 + p11) == A) ) {          
          msr.clear_shift(13);
          //  semua mati
          p4 = n4;
          p11 = n11;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {        
        msr.set_shift(3);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p4 + p11) == B) ) {          
          msr.clear_shift(3);
          //  semua mati
          p4 = n4;
          p11 = n11;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
    check_aturan_tembak(4,11);
  } else if ( lumbung_sekarang == 5 || lumbung_sekarang == 10 ) {
    if (n5 == 0){
      msr.clear_shift(17);
    }else{
      msr.set_shift(17);
    }
    if (n10 == 0){
      msr.clear_shift(11);
    }else{
      msr.set_shift(11);
    }
    //  nyala hijau
    //  nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 5 dan 10 -------------");
    if ( (n5 == 0) && (n10 == 0) ) {
      if (giliran_pemain == 1) {
        msr.set_shift(13);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p5 + p10) == A) ) {          
          msr.clear_shift(13);
          //  semua mati
          p5 = n5;
          p10 = n10;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(3);
        //  nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p5 + p10) == B) ) {          
          msr.clear_shift(3);
          //  semua mati
          p5 = n5;
          p10 = n10;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
    check_aturan_tembak(5,10);
  }   

  // else if (
  //   ( (lumbung_sekarang == 3) || (lumbung_sekarang == 12) && ((n3 == 0) || (n12 == 0)) ) ||
  //   ( (lumbung_sekarang == 4) || (lumbung_sekarang == 11) && ((n4 == 0) || (n11 == 0)) ) ||
  //   ( (lumbung_sekarang == 5) || (lumbung_sekarang == 10) && ((n5 == 0) || (n10 == 0)) ) 
  // ) {
  //   check_aturan();
  // } 
}

void ganti_giliran() {
  if (giliran_pemain == 1) {
    giliran_pemain = 2;
    digitalWrite(playerAturn, LOW);
    digitalWrite(playerBturn, HIGH);
    lumbung_sekarang = 0;
    Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
  } else if (giliran_pemain == 2) {
    giliran_pemain = 1;
    digitalWrite(playerAturn, HIGH);
    digitalWrite(playerBturn, LOW);
    lumbung_sekarang = 0;
    Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
  }
  check_game_selesai();
}

void check_game_selesai() {
  if ( (n3 == 0) && (n4 == 0) && (n5 == 0) && (n10 == 0) && (n11 == 0) && (n12 == 0) ) {
    //permainan selesai karena biji pada lumbung sudah habis
    GAME_STATE = 5;
  } else {
    GAME_STATE = 1;
  }
}

bool check_giliran_selesai() {
  //permainan selesai karena biji pada lumbung sudah habis
  if ( giliran_pemain == 1 && (n3 == 0) && (n4 == 0) && (n5 == 0) ) {
    GAME_STATE = 4;
    return true;
  } else if ( giliran_pemain == 2 && (n10 == 0) && (n11 == 0) && (n12 == 0) ) {
    GAME_STATE = 4;
    return true;
  }
  return false;
}

void clear_all_green(){
  //  semua hijau mati
  msr.clear_shift(20);
  msr.clear_shift(21);
  msr.clear_shift(17);
  msr.clear_shift(12);
  msr.clear_shift(11);
  msr.clear_shift(2);
  msr.clear_shift(7);
  msr.clear_shift(4);
}

void detect_lumbung_dipilih_dari_rumah() {
  //Mendeteksi lumbung yang dipilih pemain 1 = 1 2 3 4 5 6 7 || pemain 2 = 8 9 10 11 12 13 14
  if ( (p3 != 0) && (n3 == 0) && (n4 == p4) && (n5 == p5) && (n10 == p10) && (n11 == p11) && (n12 == p12) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 3;
    biji_ditangan = p3;
    p3 = n3;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p4 != 0) && (n4 == 0) && (n3 == p3) && (n5 == p5)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 4;
    biji_ditangan = p4;
    p4 = n4;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p5 != 0) && (n5 == 0)  && (n3 == p3) && (n4 == p4)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 5;
    biji_ditangan = p5;
    p5 = n5;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p10 != 0) && (n10 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n11 == p11) && (n12 == p12)   && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 10;
    biji_ditangan = p10;
    p10 = n10;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 10 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p11 != 0) && (n11 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n12 == p12)   && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 11;
    biji_ditangan = p11;
    p11 = n11;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 11 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p12 != 0) && (n12 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n11 == p11)   && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 12;
    biji_ditangan = p12;
    p12 = n12;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 12 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else {
    check_aturan();
  }
  if(lumbung_terpilih){
    clear_all_green();
  }
}

void detect_lumbung_dipilih() {
  //Mendeteksi lumbung yang dipilih pemain
  if ( (p3 != 0) && (n3 == 0)  && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 3) ) {
    lumbung_sekarang = 3;
    biji_ditangan = p3;
    p3 = n3;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p4 != 0) && (n4 == 0)  && (n3 == p3) && (n5 == p5)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 4) ) {
    lumbung_sekarang = 4;
    biji_ditangan = p4;
    p4 = n4;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p5 != 0) && (n5 == 0)  && (n3 == p3) && (n4 == p4)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 5) ) {
    lumbung_sekarang = 5;
    biji_ditangan = p5;
    p5 = n5;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p10 != 0) && (n10 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 10) ) {
    lumbung_sekarang = 10;
    biji_ditangan = p10;
    p10 = n10;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 10 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p11 != 0) && (n11 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n12 == p12)   && (lumbung_sekarang == 11) ) {
    lumbung_sekarang = 11;
    biji_ditangan = p11;
    p11 = n11;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 11 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p12 != 0) && (n12 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n11 == p11)   && (lumbung_sekarang == 12) ) {
    lumbung_sekarang = 12;
    biji_ditangan = p12;
    p12 = n12;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 12 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else {
    check_aturan();
  }

  if(lumbung_terpilih){
    clear_all_green();
  }
}
//TODO: Ketika sensor mendeteksi beban lebih krn ada tangan yang mengambil biji, jangan dideteksi pelanggaran
void check_aturan() {  
  if( r3 > 70 ){
    msr.clear_shift(18);
    msr.clear_shift(19);
    //  lampu mati
  }else if ( n3 < p3 ) {
    clear_all_green();
    msr.set_shift(18);
    msr.clear_shift(19);
    //  semua hijau mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
  } else if ( n3 == p3 ) {
    msr.clear_shift(18);
    msr.clear_shift(19);
    //  lampu mati
  } else if ( n3 > p3 ) {
    clear_all_green();
    msr.set_shift(19);
    msr.clear_shift(18);
    //  semua hijau mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
  }
  if( r4 > 70 ){
    msr.clear_shift(22);
    msr.clear_shift(23);
    //  lampu mati
  }else if ( n4 < p4 ) {
    clear_all_green();
    msr.set_shift(22);
    msr.clear_shift(23);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
  } else if ( n4 == p4 ) {
    msr.clear_shift(22);
    msr.clear_shift(23);
    //  lampu mati
  } else if ( n4 > p4 ) {
    clear_all_green();
    msr.set_shift(23);
    msr.clear_shift(22);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
  }
  if( r5 > 70 ){
    msr.clear_shift(16);
    msr.clear_shift(15);
    //  lampu mati
  }else if ( n5 < p5 ) {
    clear_all_green();
    msr.set_shift(16);
    msr.clear_shift(15);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
  } else if ( n5 == p5 ) {
    msr.clear_shift(16);
    msr.clear_shift(15);
    //  lampu mati
  } else if ( n5 > p5 ) {
    clear_all_green();
    msr.set_shift(15);
    msr.clear_shift(16);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
  }
  if( r10 > 70 ){
    msr.clear_shift(10);
    msr.clear_shift(9);
    //  lampu mati
  }else if ( n10 < p10 ) {
    clear_all_green();
    msr.set_shift(10);
    msr.clear_shift(9);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
  } else if ( n10 == p10 ) {
    msr.clear_shift(10);
    msr.clear_shift(9);
    //  lampu mati
  } else if ( n10 > p10 ) {
    clear_all_green();
    msr.set_shift(9);
    msr.clear_shift(10);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
  }
  if( r11 > 70 ){
    msr.clear_shift(1);
    msr.clear_shift(0);
    //  lampu mati
  }else if ( n11 < p11 ) {
    clear_all_green();
    msr.set_shift(0);
    msr.clear_shift(1);
    //  semua mati
    //  lampu kuning (bijinya kurang)    
    Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
  } else if ( n11 == p11 ) {
    msr.clear_shift(1);
    msr.clear_shift(0);
    //  lampu mati
  } else if ( n11 > p11 ) {
    clear_all_green();
    msr.set_shift(1);
    msr.clear_shift(0);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
  }
  if( r12 > 70 ){
    msr.clear_shift(6);
    msr.clear_shift(8);
    //  lampu mati
  }else if ( n12 < p12 ) {
    clear_all_green();
    msr.set_shift(8);
    msr.clear_shift(6);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
  } else if ( n12 == p12 ) {
    msr.clear_shift(6);
    msr.clear_shift(8);
    //  lampu mati
  } else if ( n12 > p12 ) {
    clear_all_green();
    msr.set_shift(6);
    msr.clear_shift(8);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
  }
  if ( A < pA ) {
    clear_all_green();
    msr.set_shift(13);
    msr.clear_shift(14);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
  } else if ( A == pA ) {
    msr.clear_shift(14);
    msr.clear_shift(13);
    //  lampu mati
  } else if ( A > pA ) {
    clear_all_green();
    msr.set_shift(14);
    msr.clear_shift(13);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }
  if ( B < pB ) {
    clear_all_green();
    msr.set_shift(3);
    msr.clear_shift(5);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
  } else if ( B == pB ) {
    msr.clear_shift(5);
    msr.clear_shift(3);
    //  lampu mati
  } else if ( B > pB ) {
    clear_all_green();
    msr.set_shift(5);
    msr.clear_shift(3);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }
}

void check_aturan_tembak(int lumb1, int lumb2) {  
  if( r3 > 70 ){
    msr.clear_shift(18);
    msr.clear_shift(19);
    // do nothing
  }else if ( lumb1 != 3 && n3 < p3 ) {
    clear_all_green();
    msr.set_shift(18);
    msr.clear_shift(19);
    //  semua hijau mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");    
  } else if ( n3 > p3 ) {
    clear_all_green();
    msr.set_shift(19);
    msr.clear_shift(18);
    //  semua hijau mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(18);
    msr.clear_shift(19);
    //  lampu mati
  }
  if( r4 > 70 ){
    msr.clear_shift(22);
    msr.clear_shift(23);
    // do nothing
  }else if ( lumb1 != 4 && n4 < p4 ) {
    clear_all_green();
    msr.set_shift(22);
    msr.clear_shift(23);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
  } else if ( n4 > p4 ) {
    clear_all_green();
    msr.set_shift(23);
    msr.clear_shift(22);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(22);
    msr.clear_shift(23);
    //  lampu mati
  }
  if( r5 > 70 ){
    msr.clear_shift(16);
    msr.clear_shift(15);
    // do nothing
  }else if ( lumb1 != 5 && n5 < p5 ) {
    clear_all_green();
    msr.set_shift(16);
    msr.clear_shift(15);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
  } else if ( n5 > p5 ) {
    clear_all_green();
    msr.set_shift(15);
    msr.clear_shift(16);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(16);
    msr.clear_shift(15);
    //  lampu mati
  }
  if( r10 > 70 ){
    msr.clear_shift(10);
    msr.clear_shift(9);
    // do nothing
  }else if ( lumb2 != 10 && n10 < p10 ) {
    clear_all_green();
    msr.set_shift(10);
    msr.clear_shift(9);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
  } else if ( n10 > p10 ) {
    clear_all_green();
    msr.set_shift(9);
    msr.clear_shift(10);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(10);
    msr.clear_shift(9);
    //  lampu mati
  }
  if( r11 > 70 ){
    msr.clear_shift(0);
    msr.clear_shift(1);
    // do nothing
  }else if ( lumb2 != 11 && n11 < p11 ) {
    clear_all_green();
    msr.set_shift(0);
    msr.clear_shift(1);
    //  semua mati
    //  lampu kuning (bijinya kurang)    
    Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
  } else if ( n11 > p11 ) {
    clear_all_green();
    msr.set_shift(1);
    msr.clear_shift(0);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(1);
    msr.clear_shift(0);
    //  lampu mati
  }
  if( r12 > 70 ){
    msr.clear_shift(6);
    msr.clear_shift(8);
    // do nothing
  }else if ( lumb2 != 12 && n12 < p12 ) {
    clear_all_green();
    msr.set_shift(8);
    msr.clear_shift(6);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
  } else if ( n12 > p12 ) {
    clear_all_green();
    msr.set_shift(6);
    msr.clear_shift(8);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(6);
    msr.clear_shift(8);
    //  lampu mati
  }
  if ( A < pA ) {
    clear_all_green();
    msr.set_shift(13);
    msr.clear_shift(14);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
  } else if ( A > pA ) {
    clear_all_green();
    msr.set_shift(14);
    msr.clear_shift(13);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(14);
    msr.clear_shift(13);
    //  lampu mati
  }
  if ( B < pB ) {
    clear_all_green();
    msr.set_shift(3);
    msr.clear_shift(5);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
  } else if ( B > pB ) {
    clear_all_green();
    msr.set_shift(5);
    msr.clear_shift(3);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  } else {
    msr.clear_shift(5);
    msr.clear_shift(3);
    //  lampu mati
  }
}

bool isSudahSiapMain() {  
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n3 > 70 ){
    // do nothing
    }else if ( n3 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(18);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
    } else if ( n3 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(19);
      msr.clear_shift(18);
      //  lampu mati
    } else if ( n3 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(19);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n4 > 70 ){
    // do nothing
    }else if ( n4 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(22);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
    } else if ( n4 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(23);
      msr.clear_shift(22);
      //  lampu mati
    } else if ( n4 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(23);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n5 > 70 ){
    // do nothing
    }else if ( n5 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(16);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
    } else if ( n5 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(15);
      msr.clear_shift(16);
      //  lampu mati
    } else if ( n5 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(15);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n10 > 70 ){
    // do nothing
    }else if ( n10 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(10);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
    } else if ( n10 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(9);
      msr.clear_shift(10);
      //  lampu mati
    } else if ( n10 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(9);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n11 > 70 ){
    // do nothing
    }else if ( n11 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(0);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
    } else if ( n11 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(1);
      msr.clear_shift(0);
      //  lampu mati
    } else if ( n11 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(1);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n12 > 70 ){
    // do nothing
    }else if ( n12 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(8);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
    } else if ( n12 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(6);
      msr.clear_shift(8);
      //  lampu mati
    } else if ( n12 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(6);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
    }
  }
  if ( A > 0 ) {
    clear_all_green();
    msr.set_shift(14);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }else{
    msr.clear_shift(14);
    //  semua mati
  }
  if ( B > 0 ) {
    clear_all_green();
    msr.set_shift(5);
    //  semua mati
    //  lampu merah (bijinya lebih)    
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }else{
    msr.clear_shift(5);
    //  semua mati
  }

  if( (BIJI_YANG_DIGUNAKAN==3) ){
      if ( 
        (n3 == BIJI_YANG_DIGUNAKAN) &&
        (n4 == BIJI_YANG_DIGUNAKAN) &&
        (n5 == BIJI_YANG_DIGUNAKAN) &&
        (n10 == BIJI_YANG_DIGUNAKAN) &&
        (n11 == BIJI_YANG_DIGUNAKAN) &&
        (n12 == BIJI_YANG_DIGUNAKAN) &&
        (A == 0) && (B == 0) ) {
        return true;
      } else {
        return false;
      }
  }
}

void detect_if_biji_dimasukkan() {
  //deteksi jika 1 biji dimasukkan ke lumbung selanjutnya
  if (biji_ditangan == 0) {
    GAME_STATE = 1;
    lumbung_terpilih = false;
  } else {
    //1-7
    if ( (p4 + 1 == n4) && (lumbung_sekarang == 3) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p4 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p4 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 4;
      p4 = n4;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(22);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 4 !!! -------------");
    } else if ( (p5 + 1 == n5) && (lumbung_sekarang == 4) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p5 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p5 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 5;
      p5 = n5;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(16);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 5 !!! -------------");
    } else if ( (pA + 1 == A) && (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lA;
      pA = A;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(13);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = A !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == 5) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(10);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p11 + 1 == n11) && (lumbung_sekarang == 10) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p11 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p11 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 11;
      p11 = n11;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(0);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 11 !!! -------------");
    } else if ( (p12 + 1 == n12) && (lumbung_sekarang == 11) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p12 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p12 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 12;
      p12 = n12;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(8);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 12 !!! -------------");
    } else if ( (pB + 1 == B) && (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lB;
      pB = B;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(3);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = B !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == 12) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == lA) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(10);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == lB) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
      //  semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else {
      check_aturan();
    }
  }
}

void printValue() {
  // Serial.print( "Biji ditangan: " );
  // Serial.print(biji_ditangan);
  // Serial.print( " # Lumbung sekarang: " );
  // Serial.print(lumbung_sekarang);
  // Serial.print( " # giliran: " );
  // Serial.print(giliran_pemain);
  // Serial.print( " # lumbung dipakai: " );
  // Serial.print(BIJI_YANG_DIGUNAKAN);
  // Serial.print( " # state: " );
  // Serial.println(GAME_STATE);

  if( (BIJI_YANG_DIGUNAKAN==3) ){
      Serial.print( " # r3: " );
      Serial.print(r3);
      Serial.print( " # r4: " );
      Serial.print(r4);
      Serial.print( " # r5: " );
      Serial.print(r5);
      Serial.print( " # rA: " );
      Serial.print(rA);
      Serial.print( "  ## " );
      Serial.print( " # r10: " );
      Serial.print(r10);
      Serial.print( " # r11: " );
      Serial.print(r11);
      Serial.print( " # r12: " );
      Serial.print(r12);
      Serial.print( " # rB: " );
      Serial.println(rB);
  }

//  if( (BIJI_YANG_DIGUNAKAN==3) ){
//      Serial.print( " # p3: " );
//      Serial.print(p3);
//      Serial.print( " # p4: " );
//      Serial.print(p4);
//      Serial.print( " # p5: " );
//      Serial.print(p5);
//      Serial.print( " # pA: " );
//      Serial.print(pA);
//      Serial.print( "  ## " );
//      Serial.print( " # p10: " );
//      Serial.print(p10);
//      Serial.print( " # p11: " );
//      Serial.print(p11);
//      Serial.print( " # p12: " );
//      Serial.print(p12);
//      Serial.print( " # pB: " );
//      Serial.println(pB);
//  }
//
//  if( (BIJI_YANG_DIGUNAKAN==3) ){
//      Serial.print( " # n3: " );
//      Serial.print(n3);
//      Serial.print( " # n4: " );
//      Serial.print(n4);
//      Serial.print( " # n5: " );
//      Serial.print(n5);
//      Serial.print( " #  A: " );
//      Serial.print(A);
//      Serial.print( "  ## " );
//      Serial.print( " # n10: " );
//      Serial.print(n10);
//      Serial.print( " # n11: " );
//      Serial.print(n11);
//      Serial.print( " # n12: " );
//      Serial.print(n12);
//      Serial.print( " #  B: " );
//      Serial.println(B);
//  }
  
}

void detect3() {
  cell3.set_scale(wc3);

  r3 = cell3.get_units(), 10;
  if (r3 < 0) {
    r3 = 0.00;
  }
  n3 = round(r3 - 2) / AVG_BIJI;
   float val = (r3 - 2) - (n3 * AVG_BIJI);
   if (val > TOLERANSI){
     n3 = n3+1;
   }
}

void detect4() {
  cell4.set_scale(wc4);

  r4 = cell4.get_units(), 10;
  if (r4 < 0) {
    r4 = 0.00;
  }
  n4 = round(r4) / AVG_BIJI;
   float val = r4 - (n4 * AVG_BIJI);
   if (val > TOLERANSI){
     n4 = n4+1;
   }
}

void detect5() {
  cell5.set_scale(wc5);

  r5 = cell5.get_units(), 10;
  if (r5 < 0) {
    r5 = 0.00;
  }
  n5 = round(r5) / AVG_BIJI;
   float val = r5 - (n5 * AVG_BIJI);
   if (val > TOLERANSI){
     n5 = n5+1;
   }
}

void detect10() {
  cell10.set_scale(wc10);

  r10 = cell10.get_units(), 10;
  if (r10 < 0) {
    r10 = 0.00;
  }
  n10 = round(r10) / AVG_BIJI;
   float val = r10 - (n10 * AVG_BIJI);
   if (val > TOLERANSI){
     n10 = n10+1;
   }
}

void detect11() {
  cell11.set_scale(wc11);

  r11 = cell11.get_units(), 10;
  if (r11 < 0) {
    r11 = 0.00;
  }
  n11 = round(r11) / AVG_BIJI;
   float val = r11 - (n11 * AVG_BIJI);
   if (val > TOLERANSI){
     n11 = n11+1;
   }
}

void detect12() {
  cell12.set_scale(wc12);

  r12 = cell12.get_units(), 10;
  if (r12 < 0) {
    r12 = 0.00;
  }
  n12 = round(r12) / AVG_BIJI;
   float val = r12 - (n12 * AVG_BIJI);
   if (val > TOLERANSI){
     n12 = n12+1;
   }
}

void detectA() {
  cellA.set_scale(wcA);

  rA = cellA.get_units(), 10;
  if (rA < 0) {
    rA = 0.00;
  }
  A = round(rA) / AVG_BIJI;
   float val = rA - (A * AVG_BIJI);
   if (val > TOLERANSI){
     A = A+1;
   }
}

void detectB() {
  cellB.set_scale(wcB);

  rB = cellB.get_units(), 10;
  if (rB < 0) {
    rB = 0.00;
  }
  B = round(rB) / AVG_BIJI;
   float val = rB - (B * AVG_BIJI);
   if (val > TOLERANSI){
     B = B+1;
   }
}

