#include <HX711.h>
#include <math.h>
#include <MultiShiftRegister.h>

int BIJI_YANG_DIGUNAKAN = 3;
float TOLERANSI = 4.25;
//float TOLERANSI_R = 4.75;

float BATAS_BAWAH = 100.1;
float BATAS_ATAS = 100.3;

float AVG_BIJI = 5;

bool pilih_ulang_dari_rumah = false;
bool boleh_pilih_ulang = false;

int lumbung_sekarang = 0;
int lumbung_pre = -1;
int lumbung_lanjut = 0;
int lumbung_diambil_awal = -1;

int giliran_pemain = 0;

int biji_ditangan = 0;
bool lumbung_terpilih = false;
int preP = -1;

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

int GAME_STATE = -1;
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

float subs3 = 0;
float subs4 = 0;
float subs5 = 0;
float subs10 = 0;
float subs11 = 0;
float subs12 = 0;
float subsA = 0;
float subsB = 0;

int wc3 = -2131; //-909
int n3 = 0;
float r3 = 0;
bool rt3 = false;
int p3 = 0;

int wc4 = -2200; //-2200
int n4 = 0;
float r4 = 0;
bool rt4 = false;
int p4 = 0;

int wc5 = -1159; //-1159
int n5 = 0;
float r5 = 0;
bool rt5 = false;
int p5 = 0;

int wc10 = -767; //-767
int n10 = 0;
float r10 = 0;
bool rt10 = false;
int p10 = 0;

int wc11 = -865; //-865
int n11 = 0;
float r11 = 0;
bool rt11 = false;
int p11 = 0;

int wc12 = -868; //-868
int n12 = 0;
float r12 = 0;
bool rt12 = false;
int p12 = 0;

int wcA = -808; //-808
int A = 0;
float rA = 0;
bool rtA = false;
int pA = 0;

int wcB = -848; //-848
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

  congklak_game();

//  delay(200);
}

void congklak_game() {
  switch (GAME_STATE) {
    case -1:
//    Serial.println("---------- KALIBRASI BERAT !!! -------------");
    //KALIBRASI BERAT
      kalibrasi_berat();
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
      if (check_giliran_selesai()) {
        GAME_STATE = 4;
      } else if ( pilih_ulang_dari_rumah ||
                  (lumbung_sekarang == lA && giliran_pemain == 1) ||
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
        lumbung_terpilih = false;
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
      if(biji_ditangan!=0){
        show_next_lumbung();
      }
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
  detect3();
  detect12();
  detect4();
  detect11();
  detect5();
  detect10();
  detectA();
  detectB();
}


void permainan_selesai() {
  digitalWrite(playerAturn, LOW);
  digitalWrite(playerBturn, LOW);
  Serial.println("---------- PERMAINAN SELESAI !!! -------------");
  if (A > B) {
    msr.set_shift(14);
    msr.set_shift(12);
    msr.set_shift(13);
    delay(200);
    msr.clear_shift(14);
    msr.clear_shift(12);
    msr.clear_shift(13);
    delay(200);
    Serial.println("---------- PEMENANG : PEMAIN 1 -------------");
  } else if (A < B) {
    msr.set_shift(3);
    msr.set_shift(4);
    msr.set_shift(5);
    delay(200);
    msr.clear_shift(3);
    msr.clear_shift(4);
    msr.clear_shift(5);
    delay(200);
    Serial.println("---------- PEMENANG : PEMAIN 2 -------------");
  } else {
    msr.set_shift(14);
    msr.set_shift(12);
    msr.set_shift(13);
    msr.set_shift(3);
    msr.set_shift(4);
    msr.set_shift(5);
    delay(200);
    msr.clear_shift(14);
    msr.clear_shift(12);
    msr.clear_shift(13);
    msr.clear_shift(3);
    msr.clear_shift(4);
    msr.clear_shift(5);
    delay(200);
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

void detect_if_tembak_diambil() {
  if ( (lumbung_sekarang == 3) || (lumbung_sekarang == 12) ) {        
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
    if (giliran_pemain == 1) {    
        msr.set_shift(13);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
    } else if (giliran_pemain == 2) {        
        msr.set_shift(3);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
    }
    //  nyala hijau
    //  nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 3 dan 12 -------------");
    if ( (n3 == 0) && (n12 == 0) ) {
      if ((giliran_pemain == 1) && (pA + (p3 + p12) == A)) {
          msr.clear_shift(13);
          //  semua mati
          p3 = n3;
          p12 = n12;
          pA = A;
          GAME_STATE = 4;
      } else if ((giliran_pemain == 2) && (pB + (p3 + p12) == B)) {  
          msr.clear_shift(3);
          //  semua mati
          p3 = n3;
          p12 = n12;
          pB = B;
          GAME_STATE = 4;
      }
    }
    check_aturan_tembak(3,12,(p3 + p12));
  } else if ( (lumbung_sekarang == 4) || (lumbung_sekarang == 11) ) {        
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
    if (giliran_pemain == 1) {    
        msr.set_shift(13);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
    } else if (giliran_pemain == 2) {        
        msr.set_shift(3);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
    }
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 4 dan 11 -------------");
    if ( (n4 == 0) && (n11 == 0) ) {
      if ((giliran_pemain == 1) && (pA + (p4 + p11) == A)) { 
          msr.clear_shift(13);
          //  semua mati
          p4 = n4;
          p11 = n11;
          pA = A;
          GAME_STATE = 4;
      } else if ((giliran_pemain == 2) && (pB + (p4 + p11) == B)) {
          msr.clear_shift(3);
          //  semua mati
          p4 = n4;
          p11 = n11;
          pB = B;
          GAME_STATE = 4;
      }
    }
    check_aturan_tembak(4,11,(p4 + p11));
  } else if ( (lumbung_sekarang == 5) || (lumbung_sekarang == 10) ) {        
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
    if (giliran_pemain == 1) {    
        msr.set_shift(13);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
    } else if (giliran_pemain == 2) {        
        msr.set_shift(3);
        //  nyala kuning
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
    }
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 5 dan 10 -------------");
    if ( (n5 == 0) && (n10 == 0) ) {
      if ((giliran_pemain == 1) && (pA + (p5 + p10) == A)) { 
          msr.clear_shift(13);
          //  semua mati
          p5 = n5;
          p10 = n10;
          pA = A;
          GAME_STATE = 4;
      } else if ((giliran_pemain == 2) && (pB + (p5 + p10) == B)) {
          msr.clear_shift(3);
          //  semua mati
          p5 = n5;
          p10 = n10;
          pB = B;
          GAME_STATE = 4;
      }
    }
    check_aturan_tembak(5,10,(p5 + p10));
  } else{
    GAME_STATE = 4;
  }  
}

void ganti_giliran() {
  clear_all_green();
  if (giliran_pemain == 1 && ( (p10 != 0) || (p11 != 0) || (p12 != 0) ) ) {
    giliran_pemain = 2;
    digitalWrite(playerAturn, LOW);
    digitalWrite(playerBturn, HIGH);
    lumbung_sekarang = 0;
    preP = -1;
    lumbung_pre = -1;
    lumbung_diambil_awal = -1;
    lumbung_terpilih = false;
    Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
  } else if (giliran_pemain == 2 && ( (p3 != 0) || (p4 != 0) || (p5 != 0) ) ) {
    giliran_pemain = 1;
    digitalWrite(playerAturn, HIGH);
    digitalWrite(playerBturn, LOW);
    lumbung_sekarang = 0;
    preP = -1;
    lumbung_pre = -1;
    lumbung_diambil_awal = -1;
    lumbung_terpilih = false;
    Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
  }
  check_game_selesai();
}

void check_game_selesai() {
  if ( (p3 == 0) && (p4 == 0) && (p5 == 0) && (p10 == 0) && (p11 == 0) && (p12 == 0) ) {
    //permainan selesai karena biji pada lumbung sudah habis
    GAME_STATE = 5;
  } else {
    GAME_STATE = 1;
    lumbung_sekarang = 0;
    preP = -1;
    lumbung_pre = -1;
    lumbung_diambil_awal = -1;
    lumbung_terpilih = false;
    Serial.println("---------- GILIRAN TETAP !!! -------------");
  }
}

bool check_giliran_selesai() {
  //permainan selesai karena biji pada lumbung sudah habis
  if ( giliran_pemain == 1 && (p3 == 0) && (p4 == 0) && (p5 == 0) && (lumbung_sekarang == lA || lumbung_sekarang == 0 )) {
    GAME_STATE = 4;
    return true;
  } else if ( giliran_pemain == 2 && (p10 == 0) && (p11 == 0) && (p12 == 0) && (lumbung_sekarang == lB || lumbung_sekarang == 0 ) ) {
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
    lumbung_pre = lumbung_sekarang;
    lumbung_sekarang = 3;
    biji_ditangan = p3;
    preP = p3;
    p3 = n3;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 3;
    pilih_ulang_dari_rumah = false;
  } else if ( (p4 != 0) && (n4 == 0) && (n3 == p3) && (n5 == p5)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_pre = lumbung_sekarang;
    lumbung_sekarang = 4;
    biji_ditangan = p4;
    preP = p4;
    p4 = n4;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 4;
    pilih_ulang_dari_rumah = false;
  } else if ( (p5 != 0) && (n5 == 0)  && (n3 == p3) && (n4 == p4)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_pre = lumbung_sekarang;
    lumbung_sekarang = 5;
    biji_ditangan = p5;
    preP = p5;
    p5 = n5;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 5;
    pilih_ulang_dari_rumah = false;
  } else if ( (p10 != 0) && (n10 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n11 == p11) && (n12 == p12)   && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_pre = lumbung_sekarang;
    lumbung_sekarang = 10;
    biji_ditangan = p10;
    preP = p10;
    p10 = n10;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 10 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 10;
    pilih_ulang_dari_rumah = false;
  } else if ( (p11 != 0) && (n11 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n12 == p12)   && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_pre = lumbung_sekarang;
    lumbung_sekarang = 11;
    biji_ditangan = p11;
    preP = p11;
    p11 = n11;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 11 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 11;
    pilih_ulang_dari_rumah = false;
  } else if ( (p12 != 0) && (n12 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n11 == p11)   && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_pre = lumbung_sekarang;
    lumbung_sekarang = 12;
    biji_ditangan = p12;
    preP = p12;
    p12 = n12;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 12 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 12;
    pilih_ulang_dari_rumah = false;
  } else {
    if(GAME_STATE == 1 || GAME_STATE == 2){
        check_if_pilih_ulang();      
    }
    check_aturan();
  }
  if(lumbung_terpilih){
    clear_all_green();
  }
}

// TODO
void check_if_pilih_ulang(){
  if(boleh_pilih_ulang){
    if( biji_ditangan==preP && (lumbung_diambil_awal==3) && (n3==preP) && (n4==p4) && (n5==p5) && (n10==p10) && (n11==p11) && (n12==p12) ){
      GAME_STATE = 1;
      p3 = preP;
      lumbung_sekarang = lumbung_pre;
      Serial.println("---------- LUMBUNG 3 DIKEMBALIKAN !!! -------------");
      Serial.print("LUMBUNG PRE = ");
      Serial.println(lumbung_pre);    
      if(lumbung_pre == lA || lumbung_pre == lB){
        pilih_ulang_dari_rumah = true;
      }
      preP = -1;
      lumbung_pre = -1;
      lumbung_diambil_awal = -1;
      lumbung_terpilih = false;
    }
    else if( biji_ditangan==preP && (lumbung_diambil_awal==4) && (n3==p3) && (n4==preP) && (n5==p5) && (n10==p10) && (n11==p11) && (n12==p12) ){
      GAME_STATE = 1;
      p4 = preP;
      lumbung_sekarang = lumbung_pre; 
      Serial.println("---------- LUMBUNG 4 DIKEMBALIKAN !!! -------------");
      Serial.print("LUMBUNG PRE = ");
      Serial.println(lumbung_pre);  
      if(lumbung_pre == lA || lumbung_pre == lB){
        pilih_ulang_dari_rumah = true;
      } 
      preP = -1;
      lumbung_pre = -1;
      lumbung_diambil_awal = -1;
      lumbung_terpilih = false;
    }
    else if( biji_ditangan==preP && (lumbung_diambil_awal==5) && (n3==p3) && (n4==p4) && (n5==preP) && (n10==p10) && (n11==p11) && (n12==p12) ){
      GAME_STATE = 1;
      p5 = preP;
      lumbung_sekarang = lumbung_pre;
      Serial.println("---------- LUMBUNG 5 DIKEMBALIKAN !!! -------------");
      Serial.print("LUMBUNG PRE = ");
      Serial.println(lumbung_pre);    
      if(lumbung_pre == lA || lumbung_pre == lB){
        pilih_ulang_dari_rumah = true;
      }
      preP = -1;
      lumbung_pre = -1;
      lumbung_diambil_awal = -1;
      lumbung_terpilih = false;
    }
    else if( biji_ditangan==preP && (lumbung_diambil_awal==10) && (n3==p3) && (n4==p4) && (n5==p5) && (n10==preP) && (n11==p11) && (n12==p12) ){
      GAME_STATE = 1;
      p10 = preP;
      lumbung_sekarang = lumbung_pre;
      Serial.println("---------- LUMBUNG 10 DIKEMBALIKAN !!! -------------");
      Serial.print("LUMBUNG PRE = ");
      Serial.println(lumbung_pre);    
      if(lumbung_pre == lA || lumbung_pre == lB){
        pilih_ulang_dari_rumah = true;
      }
      preP = -1;
      lumbung_pre = -1;
      lumbung_diambil_awal = -1;
      lumbung_terpilih = false;
    }
    else if( biji_ditangan==preP && (lumbung_diambil_awal==11) && (n3==p3) && (n4==p4) && (n5==p5) && (n10==p10) && (n11==preP) && (n12==p12) ){
      GAME_STATE = 1;
      p11 = preP;
      lumbung_sekarang = lumbung_pre;
      Serial.println("---------- LUMBUNG 11 DIKEMBALIKAN !!! -------------");
      Serial.print("LUMBUNG PRE = ");
      Serial.println(lumbung_pre);    
      if(lumbung_pre == lA || lumbung_pre == lB){
        pilih_ulang_dari_rumah = true;
      }
      preP = -1;
      lumbung_pre = -1;
      lumbung_diambil_awal = -1;
      lumbung_terpilih = false;
    }
    else if( biji_ditangan==preP && (lumbung_diambil_awal==12) && (n3==p3) && (n4==p4) && (n5==p5) && (n10==p10) && (n11==p11) && (n12==preP) ){
      GAME_STATE = 1;
      p12 = preP;
      lumbung_sekarang = lumbung_pre;
      Serial.println("---------- LUMBUNG 12 DIKEMBALIKAN !!! -------------");
      Serial.print("LUMBUNG PRE = ");
      Serial.println(lumbung_pre);    
      if(lumbung_pre == lA || lumbung_pre == lB){
        pilih_ulang_dari_rumah = true;
      }
      preP = -1;
      lumbung_pre = -1;
      lumbung_diambil_awal = -1;
      lumbung_terpilih = false;
    }
  }
}

void detect_lumbung_dipilih() {
  //Mendeteksi lumbung yang dipilih pemain
  if ( (p3 != 0) && (n3 == 0)  && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 3) ) {    
    lumbung_sekarang = 3;
    lumbung_pre = lumbung_sekarang;
    biji_ditangan = p3;
    preP = p3;
    p3 = n3;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 3;
  } else if ( (p4 != 0) && (n4 == 0)  && (n3 == p3) && (n5 == p5)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 4) ) {
    lumbung_sekarang = 4;
    lumbung_pre = lumbung_sekarang;
    biji_ditangan = p4;
    preP = p4;
    p4 = n4;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 4;
  } else if ( (p5 != 0) && (n5 == 0)  && (n3 == p3) && (n4 == p4)  && (n10 == p10) && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 5) ) {
    lumbung_sekarang = 5;
    lumbung_pre = lumbung_sekarang;
    biji_ditangan = p5;
    preP = p5;
    p5 = n5;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 5;
  } else if ( (p10 != 0) && (n10 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n11 == p11) && (n12 == p12)   && (lumbung_sekarang == 10) ) {
    lumbung_sekarang = 10;
    lumbung_pre = lumbung_sekarang;
    biji_ditangan = p10;
    preP = p10;
    p10 = n10;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 10 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 10;
  } else if ( (p11 != 0) && (n11 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n12 == p12)   && (lumbung_sekarang == 11) ) {
    lumbung_sekarang = 11;
    lumbung_pre = lumbung_sekarang;
    biji_ditangan = p11;
    preP = p11;
    p11 = n11;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 11 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 11;
  } else if ( (p12 != 0) && (n12 == 0)  && (n3 == p3) && (n4 == p4) && (n5 == p5)  && (n10 == p10) && (n11 == p11)   && (lumbung_sekarang == 12) ) {
    lumbung_sekarang = 12;
    lumbung_pre = lumbung_sekarang;
    biji_ditangan = p12;
    preP = p12;
    p12 = n12;
    GAME_STATE = 2;
    boleh_pilih_ulang = true;
    Serial.println("---------- LUMBUNG 12 DIPILIH !!! -------------");
    lumbung_terpilih = true;
    lumbung_diambil_awal = 12;
  } else {
    if(GAME_STATE == 1 || GAME_STATE == 2){
        check_if_pilih_ulang();      
    }
    check_aturan();
  }

  if(lumbung_terpilih){
    clear_all_green();
  }
}

void check_aturan() {  
  if( r3 > 70 ){
    msr.clear_shift(18);
    msr.clear_shift(19);
    //  lampu mati
  }else if ( n3 < p3 ) {
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(6);
    msr.clear_shift(8);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
  }
  if ( A < pA ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(14);
    msr.clear_shift(13);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }
  if ( B < pB ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(5);
    msr.clear_shift(3);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }
}

void check_aturan_tembak(int lumb1, int lumb2, int tot_biji_tembak) {  
  if( r3 > 70 ){
    msr.clear_shift(18);
    msr.clear_shift(19);
    // do nothing
  } else if ( lumb1 != 3 && n3 < p3 ) {
    // clear_all_green();
    msr.set_shift(18);
    msr.clear_shift(19);
    //  semua hijau mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");    
  } else if ( n3 > p3 ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(22);
    msr.clear_shift(23);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
  } else if ( n4 > p4 ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(16);
    msr.clear_shift(15);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
  } else if ( n5 > p5 ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(10);
    msr.clear_shift(9);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
  } else if ( n10 > p10 ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(0);
    msr.clear_shift(1);
    //  semua mati
    //  lampu kuning (bijinya kurang)    
    Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
  } else if ( n11 > p11 ) {
    // clear_all_green();
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
    // clear_all_green();
    msr.set_shift(8);
    msr.clear_shift(6);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
  } else if ( n12 > p12 ) {
    // clear_all_green();
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
  if ( A < (pA+tot_biji_tembak) && giliran_pemain==1 ) {
    // clear_all_green();
    msr.set_shift(13);
    msr.clear_shift(14);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
  } else if ( A > (pA+tot_biji_tembak) && giliran_pemain==1 ) {
    // clear_all_green();
    msr.set_shift(14);
    msr.clear_shift(13);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  } else if ( A < pA && giliran_pemain==2 ) {
    // clear_all_green();
    msr.set_shift(13);
    msr.clear_shift(14);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
  } else if ( A > pA && giliran_pemain==2 ) {
    // clear_all_green();
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
  if ( B < (pB+tot_biji_tembak) && giliran_pemain==2 ) {
    // clear_all_green();
    msr.set_shift(3);
    msr.clear_shift(5);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
  } else if ( B > (pB+tot_biji_tembak) && giliran_pemain==2 ) {
    // clear_all_green();
    msr.set_shift(5);
    msr.clear_shift(3);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  } else if ( B < pB && giliran_pemain==1 ) {
    // clear_all_green();
    msr.set_shift(3);
    msr.clear_shift(5);
    //  semua mati
    //  lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
  } else if ( B > pB && giliran_pemain==1 ) {
    // clear_all_green();
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
      // clear_all_green();
      msr.set_shift(18);
      msr.clear_shift(19);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
    } else if ( n3 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(19);
      msr.clear_shift(18);
      //  lampu mati
    } else if ( n3 > BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(19);
      msr.clear_shift(18);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n4 > 70 ){
    // do nothing
    }else if ( n4 < BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(22);
      msr.clear_shift(23);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
    } else if ( n4 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(23);
      msr.clear_shift(22);
      //  lampu mati
    } else if ( n4 > BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(23);
      msr.clear_shift(22);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n5 > 70 ){
    // do nothing
    }else if ( n5 < BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(16);
      msr.clear_shift(15);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
    } else if ( n5 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(15);
      msr.clear_shift(16);
      //  lampu mati
    } else if ( n5 > BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(15);
      msr.clear_shift(16);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n10 > 70 ){
    // do nothing
    }else if ( n10 < BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(10);
      msr.clear_shift(9);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
    } else if ( n10 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(9);
      msr.clear_shift(10);
      //  lampu mati
    } else if ( n10 > BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(9);
      msr.clear_shift(10);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n11 > 70 ){
    // do nothing
    }else if ( n11 < BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(0);
      msr.clear_shift(1);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
    } else if ( n11 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(1);
      msr.clear_shift(0);
      //  lampu mati
    } else if ( n11 > BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(1);
      msr.clear_shift(0);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if( n12 > 70 ){
    // do nothing
    }else if ( n12 < BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(8);
      msr.clear_shift(6);
      //  semua mati
      //  lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
    } else if ( n12 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(6);
      msr.clear_shift(8);
      //  lampu mati
    } else if ( n12 > BIJI_YANG_DIGUNAKAN ) {
      // clear_all_green();
      msr.set_shift(6);
      msr.clear_shift(8);
      //  semua mati
      //  lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
    }
  }
  if ( A > 0 ) {
    // clear_all_green();
    msr.set_shift(14);
    //  semua mati
    //  lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }else{
    msr.clear_shift(14);
    //  semua mati
  }
  if ( B > 0 ) {
    // clear_all_green();
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
      if ( (giliran_pemain == 1) && (p4 == 0) && (p11 != 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p4 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 4;
      lumbung_pre = lumbung_sekarang;
      preP = p4;
      p4 = n4;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(22);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 4 !!! -------------");
    } else if ( (p5 + 1 == n5) && (lumbung_sekarang == 4) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p5 == 0) && (p10 != 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p5 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 5;
      lumbung_pre = lumbung_sekarang;
      preP = p5;
      p5 = n5;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(16);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 5 !!! -------------");
    } else if ( (pA + 1 == A) && (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lA;
      lumbung_pre = lumbung_sekarang;
      pA = A;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(13);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = A !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == 5) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p10 == 0) && (p5 != 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      lumbung_pre = lumbung_sekarang;
      preP = p10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(10);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p11 + 1 == n11) && (lumbung_sekarang == 10) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p11 == 0) && (p4 != 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p11 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 11;
      lumbung_pre = lumbung_sekarang;
      preP = p11;
      p11 = n11;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(0);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 11 !!! -------------");
    } else if ( (p12 + 1 == n12) && (lumbung_sekarang == 11) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p12 == 0) && (p3 != 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p12 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 12;
      lumbung_pre = lumbung_sekarang;
      preP = p12;
      p12 = n12;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(8);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 12 !!! -------------");
    } else if ( (pB + 1 == B) && (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lB;
      lumbung_pre = lumbung_sekarang;      
      pB = B;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(3);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = B !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == 12) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p3 == 0) && (p12 != 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      lumbung_pre = lumbung_sekarang;
      preP = p3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == lA) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      // if ( (giliran_pemain == 1) && (p10 == 0) && (p5 != 0) && (biji_ditangan == 1) ) {
      if ( (giliran_pemain == 1) && (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      lumbung_pre = lumbung_sekarang;
      preP = p10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(10);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == lB) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      // if ( (giliran_pemain == 2) && (p3 == 0) && (p12 != 0) && (biji_ditangan == 1) ) {
      if ( (giliran_pemain == 2) && (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      lumbung_pre = lumbung_sekarang;
      preP = p3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
      //  semua mati
      boleh_pilih_ulang = false;
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else {
      if(GAME_STATE == 1 || GAME_STATE == 2){
        check_if_pilih_ulang();      
      }
      check_aturan();
    }
  }
}

void kalibrasi_berat(){
      if( ((r3-subs3)>=BATAS_BAWAH) && ((r3-subs3)<BATAS_ATAS) ){
        rt3 = true;
        msr.set_shift(20);
        Serial.print( "FIXED wc3: " );
        Serial.println(wc3);
      }else if(!rt3 && (r3-subs3)>BATAS_ATAS && r3 > 20){
        wc3 = wc3 - 1;
        msr.clear_shift(20);
      }else if(!rt3 && (r3-subs3)<BATAS_BAWAH && r3 > 20){
        wc3 = wc3 + 1;
        msr.clear_shift(20);
      }
      if( (r4>=BATAS_BAWAH) && (r4<BATAS_ATAS) ){
        rt4 = true;
        msr.set_shift(21);
        Serial.print( "FIXED wc4: " );
        Serial.println(wc4);
      }else if(!rt4 && r4>BATAS_ATAS && r4 > 20){
        wc4 = wc4 - 1;
        msr.clear_shift(21);
      }else if(!rt4 && r4<BATAS_BAWAH && r4 > 20){
        wc4 = wc4 + 1;
        msr.clear_shift(21);
      }
      if( (r5>=BATAS_BAWAH) && (r5<BATAS_ATAS) ){
        rt5 = true;
        msr.set_shift(17);
        Serial.print( "FIXED wc5: " );
        Serial.println(wc5);
      }else if(!rt5 && r5>BATAS_ATAS && r5 > 20){
        wc5 = wc5 - 1;
        msr.clear_shift(17);
      }else if(!rt5 && r5<BATAS_BAWAH && r5 > 20){
        wc5 = wc5 + 1;
        msr.clear_shift(17);
      }
      if( (r10>=BATAS_BAWAH) && (r10<BATAS_ATAS) ){
        rt10 = true;
        msr.set_shift(11);
        Serial.print( "FIXED wc10: " );
        Serial.println(wc10);
      }else if(!rt10 && r10>BATAS_ATAS && r10 > 20){
        wc10 = wc10 - 1;
        msr.clear_shift(11);
      }else if(!rt10 && r10<BATAS_BAWAH && r10 > 20){
        wc10 = wc10 + 1;
        msr.clear_shift(11);
      }
      if( (r11>=BATAS_BAWAH) && (r11<BATAS_ATAS) ){
        rt11 = true;
        msr.set_shift(2);
        Serial.print( "FIXED wc11: " );
        Serial.println(wc11);
      }else if(!rt11 && r11>BATAS_ATAS && r11 > 20){
        wc11 = wc11 - 1;
        msr.clear_shift(2);
      }else if(!rt11 && r11<BATAS_BAWAH && r11 > 20){
        wc11 = wc11 + 1;
        msr.clear_shift(2);
      }
      if( (r12>=BATAS_BAWAH) && (r12<BATAS_ATAS) ){
        rt12 = true;
        msr.set_shift(7);
        Serial.print( "FIXED wc12: " );
        Serial.println(wc12);
      }else if(!rt12 && r12>BATAS_ATAS && r12 > 20){
        wc12 = wc12 - 1;
        msr.clear_shift(7);
      }else if(!rt12 && r12<BATAS_BAWAH && r12 > 20){
        wc12 = wc12 + 1;
        msr.clear_shift(7);
      }
      if( (rA>=BATAS_BAWAH) && (rA<BATAS_ATAS) ){
        rtA = true;
        msr.set_shift(12);
        Serial.print( "FIXED wcA: " );
        Serial.println(wcA);
      }else if(!rtA && rA>BATAS_ATAS && rA > 20){
        wcA = wcA - 1;
        msr.clear_shift(12);
      }else if(!rtA && rA<BATAS_BAWAH && rA > 20){
        wcA = wcA + 1;
        msr.clear_shift(12);
      }
      if( ((rB-subsB)>=BATAS_BAWAH) && ((rB-subsB)<BATAS_ATAS) ){
        rtB = true;
        msr.set_shift(4);
        Serial.print( "FIXED wcB: " );
        Serial.println(wcB);
      }else if(!rtB && (rB-subsB)>BATAS_ATAS && rB > 20){
        wcB = wcB - 1;
        msr.clear_shift(4);
      }else if(!rtB && (rB-subsB)<BATAS_BAWAH && rB > 20){
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
}

void printValue() {
//   Serial.print( "Biji ditangan: " );
//   Serial.print(biji_ditangan);
//   Serial.print( " # Lumbung sekarang: " );
//   Serial.print(lumbung_sekarang);
//   Serial.print( " # giliran: " );
//   Serial.print(giliran_pemain);
//   Serial.print( " # state: " );
//   Serial.print(GAME_STATE);
//   Serial.print( " # preP: " );
//   Serial.print(preP);
//   Serial.print( " # lumbung_diambil_awal: " );
//   Serial.println(lumbung_diambil_awal);

//      Serial.print( " # subs3: " );
//      Serial.print(subs3);
//      Serial.print( " # subs4: " );
//      Serial.print(subs4);
//      Serial.print( " # subs5: " );
//      Serial.print(subs5);
//      Serial.print( " # subsA: " );
//      Serial.print(subsA);
//      Serial.print( "  ## " );
//      Serial.print( " # subs10: " );
//      Serial.print(subs10);
//      Serial.print( " # subs11: " );
//      Serial.print(subs11);
//      Serial.print( " # subs12: " );
//      Serial.print(subs12);
//      Serial.print( " # subsB: " );
//      Serial.println(subsB);

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

//     Serial.print( " # p3: " );
//     Serial.print(p3);
//     Serial.print( " # p4: " );
//     Serial.print(p4);
//     Serial.print( " # p5: " );
//     Serial.print(p5);
//     Serial.print( " # pA: " );
//     Serial.print(pA);
//     Serial.print( "  ## " );
//     Serial.print( " # p10: " );
//     Serial.print(p10);
//     Serial.print( " # p11: " );
//     Serial.print(p11);
//     Serial.print( " # p12: " );
//     Serial.print(p12);
//     Serial.print( " # pB: " );
//     Serial.println(pB);
//
//    Serial.print( " # n3: " );
//    Serial.print(n3);
//    Serial.print( " # n4: " );
//    Serial.print(n4);
//    Serial.print( " # n5: " );
//    Serial.print(n5);
//    Serial.print( " #  A: " );
//    Serial.print(A);
//    Serial.print( "  ## " );
//    Serial.print( " # n10: " );
//    Serial.print(n10);
//    Serial.print( " # n11: " );
//    Serial.print(n11);
//    Serial.print( " # n12: " );
//    Serial.print(n12);
//    Serial.print( " #  B: " );
//    Serial.println(B);
  
}

void detect3() {
  cell3.set_scale(wc3);

  r3 = cell3.get_units(), 10;
  if (r3 < 0) {
    r3 = 0.00;
  }
  n3 = round(r3 - subs3) / AVG_BIJI;
   float val = (r3 - subs3) - (n3 * AVG_BIJI);
   if (val > TOLERANSI){
     n3 = n3+1;
   }
  // if (r3 >= 1 && r3 <= 2 && n3 == 0){
  //   subs3 = r3;
  // }else if (r3 < 0.9 && n3 == 0){
  //   subs3 = 0;
  // }
}

void detect4() {
  cell4.set_scale(wc4);

  r4 = cell4.get_units(), 10;
  if (r4 < 0) {
    r4 = 0.00;
  }
  n4 = round(r4 - subs4) / AVG_BIJI;
   float val = (r4 - subs4) - (n4 * AVG_BIJI);
   if (val > TOLERANSI){
     n4 = n4+1;
   }
  // if (r4 >= 1 && r4 <= 2 && n4 == 0){
  //   subs4 = r4;
  // }else if (r4 < 0.9 && n4 == 0){
  //   subs4 = 0;
  // }
}

void detect5() {
  cell5.set_scale(wc5);

  r5 = cell5.get_units(), 10;
  if (r5 < 0) {
    r5 = 0.00;
  }
  n5 = round(r5 - subs5) / AVG_BIJI;
   float val = (r5 - subs5) - (n5 * AVG_BIJI);
   if (val > TOLERANSI){
     n5 = n5+1;
   }
  // if (r5 >= 1 && r5 <= 2 && n5 == 0){
  //   subs5 = r5;
  // }else if (r5 < 0.9 && n5 == 0){
  //   subs5 = 0;
  // }
}

void detect10() {
  cell10.set_scale(wc10);

  r10 = cell10.get_units(), 10;
  if (r10 < 0) {
    r10 = 0.00;
  }
  n10 = round(r10 - subs10) / AVG_BIJI;
   float val = (r10 - subs10) - (n10 * AVG_BIJI);
   if (val > TOLERANSI){
     n10 = n10+1;
   }
  // if (r10 >= 1 && r10 <= 2 && n10 == 0){
  //   subs10 = r10;
  // }else if (r10 < 0.9 && n10 == 0){
  //   subs10 = 0;
  // }
}

void detect11() {
  cell11.set_scale(wc11);

  r11 = cell11.get_units(), 10;
  if (r11 < 0) {
    r11 = 0.00;
  }
  n11 = round(r11 - subs11) / AVG_BIJI;
   float val = (r11 - subs11) - (n11 * AVG_BIJI);
   if (val > TOLERANSI){
     n11 = n11+1;
   }
  // if (r11 >= 1 && r11 <= 2 && n11 == 0){
  //   subs11 = r11;
  // }else if (r11 < 0.9 && n11 == 0){
  //   subs11 = 0;
  // }
}

void detect12() {
  cell12.set_scale(wc12);

  r12 = cell12.get_units(), 10;
  if (r12 < 0) {
    r12 = 0.00;
  }
  n12 = round(r12 - subs12) / AVG_BIJI;
   float val = (r12 - subs12) - (n12 * AVG_BIJI);
   if (val > TOLERANSI){
     n12 = n12+1;
   }
  // if (r12 >= 1 && r12 <= 2 && n12 == 0){
  //   subs12 = r12;
  // }else if (r12 < 0.9 && n12 == 0){
  //   subs12 = 0;
  // }
}

void detectA() {
  cellA.set_scale(wcA);

  rA = cellA.get_units(), 10;
  if (rA < 0) {
    rA = 0.00;
  }
  A = round(rA - subsA) / AVG_BIJI;
//   float val = rA - (A * AVG_BIJI);
//   if (val > TOLERANSI_R){
//     A = A+1;
//   }
  // if (rA >= 1 && rA <= 2 && A == 0){
  //   subsA = rA;
  // }else if (rA < 0.9 && A == 0){
  //   subsA = 0;
  // }
}

void detectB() {
  cellB.set_scale(wcB);

  rB = cellB.get_units(), 10;
  if (rB < 0) {
    rB = 0.00;
  }
  B = round(rB - subsB) / AVG_BIJI;
//   float val = rB - (B * AVG_BIJI);
//   if (val > TOLERANSI_R){
//     B = B+1;
//   }
  // if (rB >= 1 && rB <= 2 && B == 0){
  //   subsB = rB;
  // }else if (rB < 0.9 && B == 0){
  //   subsB = 0;
  // }
}

