#include <HX711.h>
#include <math.h>
#include <MultiShiftRegister.h>

int BIJI_YANG_DIGUNAKAN = 3;
int TEMP_BIJI = 3;

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

 int latchPin = 8; //8 STCP
 int dataPin = 11; //11 DS
 int clockPin = 12; //12 SHCP

 int numberOfRegisters = 3;

 MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin);

int GAME_STATE = 0;
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

int wc1 = -885; //-898
int n1 = 0;
float r1 = 0;
int p1 = 0;

int wc2 = -2121; //-1921
int n2 = 0;
float r2 = 0;
int p2 = 0;

int wc6 = -868; //-868
int n6 = 0;
float r6 = 0;
int p6 = 0;

int wc7 = -880; //-450
int n7 = 0;
float r7 = 0;
int p7 = 0;

int wc8 = -811; //-361
int n8 = 0;
float r8 = 0;
int p8 = 0;

int wc9 = -842; //-842
int n9 = 0;
float r9 = 0;
int p9 = 0;

int wc13 = -790; //-405
int n13 = 0;
float r13 = 0;
int p13 = 0;

int wc14 = -840; //-840
int n14 = 0;
float r14 = 0;
int p14 = 0;

// ^-------------------^

int wc3 = -905; //-890
int n3 = 0;
float r3 = 0;
int p3 = 0;

int wc4 = -2200; //-2200
int n4 = 0;
float r4 = 0;
int p4 = 0;

int wc5 = -1160; //-1160
int n5 = 0;
float r5 = 0;
int p5 = 0;

int wc10 = -762; //-762
int n10 = 0;
float r10 = 0;
int p10 = 0;

int wc11 = -867; //-867
int n11 = 0;
float r11 = 0;
int p11 = 0;

int wc12 = -871; //-871
int n12 = 0;
float r12 = 0;
int p12 = 0;

int wcA = -810; //-810
int A = 0;
float rA = 0;
int pA = 0;

int wcB = -847; //-850
int B = 0;
float rB = 0;
int pB = 0;

void loop() {

  detect_lumbung();

  printValue();

  // congklak_game();

//  delay(200);
}

void congklak_game() {
  switch (GAME_STATE) {
    case 0:
      Serial.println("---------- PERSIAPAN PERMAINAN !!! -------------");
      //Periksa apakah lumbung sudah terisi biji
      if (isSudahSiapMain()) {
        Serial.println("---------- PERMAINAN SUDAH BISA DIMULAI !!! -------------");

        //Menentukan giliran pemain
        int r = random(300);
        if (r < 150) {
          Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
          giliran_pemain = 1;
        } else {
          Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
          giliran_pemain = 2;
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
    //PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
    case 1:
      // && (n1!=0 || n2!=0 || n3!=0 || n4!=0 || n5!=0 || n6!=0 || n7!=0)
      // && (n8!=0 || n9!=0 || n10!=0 || n11!=0 || n12!=0 || n13!=0 || n14!=0)
      if (check_giliran_selesai() && lumbung_terpilih) {
        GAME_STATE = 4;
        lumbung_terpilih = false;
      } else if ( (lumbung_sekarang == lA && giliran_pemain == 1) ||
                  (lumbung_sekarang == lB && giliran_pemain == 2) ||
                  (lumbung_sekarang == 0 && giliran_pemain == 1) ||
                  (lumbung_sekarang == 0 && giliran_pemain == 2) ) {
        if (giliran_pemain == 1) {          
          if (n3 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 3 -------------");
            msr.set_shift(20);
            //TODO: nyala hijau
          }
          if (n4 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 4 -------------");
            msr.set_shift(21);
            //TODO: nyala hijau
          }
          if (n5 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 5 -------------");
            msr.set_shift(17);
            //TODO: nyala hijau
          }
        } else {
          if (n10 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 10 -------------");
            msr.set_shift(11);
            //TODO: nyala hijau
          }
          if (n11 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 11 -------------");
            msr.set_shift(2);
            //TODO: nyala hijau
          }
          if (n12 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 12 -------------");
            msr.set_shift(7);
            //TODO: nyala hijau
          }
        }
        //TODO: tampilkan yang boleh dipilih lumbung
        detect_lumbung_dipilih_dari_rumah();
      } else {
        //TODO: tampilkan yang boleh dipilih lumbung lumbung_sekarang
        Serial.print("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : ");
        Serial.print(lumbung_sekarang);
        Serial.println(" -------------");
        if(lumbung_sekarang==3){
          msr.set_shift(20);
          //TODO: nyala hijau
        }
        else if(lumbung_sekarang==4){
          msr.set_shift(21);
          //TODO: nyala hijau
        }
        else if(lumbung_sekarang==5){
          msr.set_shift(17);
          //TODO: nyala hijau
        }
        else if(lumbung_sekarang==10){
          msr.set_shift(11);
          //TODO: nyala hijau
        }
        else if(lumbung_sekarang==11){
          msr.set_shift(2);
          //TODO: nyala hijau
        }
        else if(lumbung_sekarang==12){
          msr.set_shift(7);
          //TODO: nyala hijau
        }
        detect_lumbung_dipilih();
      }
      break;
    //PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
    case 2:
      // Serial.println("---------- MASUKKAN BIJI KE LUMBUNG SELANJUTNYA !!! -------------");
      show_next_lumbung();
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
  Serial.println("---------- PERMAINAN SELESAI !!! -------------");
  if (A > B) {
    Serial.println("---------- PEMENANG : PEMAIN 1 -------------");
  } else if (A < B) {
    Serial.println("---------- PEMENANG : PEMAIN 2 -------------");
  }
}

void show_next_lumbung() {  
  // A / 15
  if ( lumbung_sekarang == lA ) {
    if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
      lumbung_lanjut = 10;
      msr.set_shift(10);
      //TODO: nyala kuning
    }
  }
  // B / 16
  else if ( lumbung_sekarang == lB ) {
    if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
      lumbung_lanjut = 3;
      msr.set_shift(18);
      //TODO: nyala kuning
    }
  } 
  // 1-6
  else if ( (lumbung_sekarang == 3) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 4;
    msr.set_shift(22);
    //TODO: nyala kuning
  } else if ( (lumbung_sekarang == 4) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 5;
    msr.set_shift(16);
    //TODO: nyala kuning
  } else if ( (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = lA;
    msr.set_shift(13);
    //TODO: nyala kuning
  } else if ( (lumbung_sekarang == 5) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 10;
    msr.set_shift(10);
    //TODO: nyala kuning
  }
  // 8-13
  else if ( (lumbung_sekarang == 10) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 11;
    msr.set_shift(0);
    //TODO: nyala kuning
  } else if ( (lumbung_sekarang == 11) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 12;
    msr.set_shift(8);
    //TODO: nyala kuning
  } else if ( (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = lB;
    msr.set_shift(3);
    //TODO: nyala kuning
  } else if ( (lumbung_sekarang == 12) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 3;
    msr.set_shift(18);
    //TODO: nyala kuning
  }
  // else {
  //   lumbung_lanjut = lumbung_sekarang + 1;
  // }
  Serial.print("---------- MASUKKAN BIJI KE LUMBUNG : ");
  Serial.print(lumbung_lanjut);
  Serial.println(" -------------");
}

void detect_if_tembak_diambil() {
  if ( lumbung_sekarang == 3 || lumbung_sekarang == 12 ) {
    msr.set_shift(20);
    msr.set_shift(7);
    //TODO: nyala hijau
    //TODO: nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 3 dan 12 -------------");
    if ( (n3 == 0) && (n12 == 0) ) {
      if (giliran_pemain == 1) {
        msr.set_shift(12);
        //TODO: nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p3 + p12) == A) ) {
          msr.clear_shift(20);
          msr.clear_shift(7);
          msr.clear_shift(12);
          //TODO: semua mati
          p3 = n3;
          p12 = n12;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(4);
        //TODO: nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p3 + p12) == B) ) {
          msr.clear_shift(20);
          msr.clear_shift(7);
          msr.clear_shift(4);
          //TODO: semua mati
          p3 = n3;
          p12 = n12;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 4 || lumbung_sekarang == 11 ) {
    msr.set_shift(21);
    msr.set_shift(2);
    //TODO: nyala hijau
    //TODO: nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 4 dan 11 -------------");
    if ( (n4 == 0) && (n11 == 0) ) {
      if (giliran_pemain == 1) {
        msr.set_shift(12);
        //TODO: nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p4 + p11) == A) ) {
          msr.clear_shift(21);
          msr.clear_shift(2);
          msr.clear_shift(12);
          //TODO: semua mati
          p4 = n4;
          p11 = n11;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(4);
        //TODO: nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p4 + p11) == B) ) {
          msr.clear_shift(21);
          msr.clear_shift(2);
          msr.clear_shift(12);
          //TODO: semua mati
          p4 = n4;
          p11 = n11;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 5 || lumbung_sekarang == 10 ) {
    msr.set_shift(17);
    msr.set_shift(11);
    //TODO: nyala hijau
    //TODO: nyala hijau
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 5 dan 10 -------------");
    if ( (n5 == 0) && (n10 == 0) ) {
      if (giliran_pemain == 1) {
        msr.set_shift(12);
        //TODO: nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p5 + p10) == A) ) {
          msr.clear_shift(17);
          msr.clear_shift(11);
          msr.clear_shift(12);
          //TODO: semua mati
          p5 = n5;
          p10 = n10;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(4);
        //TODO: nyala hijau
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p5 + p10) == B) ) {
          msr.clear_shift(17);
          msr.clear_shift(11);
          msr.clear_shift(4);
          //TODO: semua mati
          p5 = n5;
          p10 = n10;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if (
    ( (lumbung_sekarang == 1) || (lumbung_sekarang == 14) && ((n1 == 0) || (n14 == 0)) ) ||
    ( (lumbung_sekarang == 2) || (lumbung_sekarang == 13) && ((n2 == 0) || (n13 == 0)) ) ||
    ( (lumbung_sekarang == 3) || (lumbung_sekarang == 12) && ((n3 == 0) || (n12 == 0)) ) ||
    ( (lumbung_sekarang == 4) || (lumbung_sekarang == 11) && ((n4 == 0) || (n11 == 0)) ) ||
    ( (lumbung_sekarang == 5) || (lumbung_sekarang == 10) && ((n5 == 0) || (n10 == 0)) ) ||
    ( (lumbung_sekarang == 6) || (lumbung_sekarang == 9) && ((n6 == 0) || (n9 == 0)) ) ||
    ( (lumbung_sekarang == 7) || (lumbung_sekarang == 8) && ((n7 == 0) || (n8 == 0)) )
  ) {
    check_aturan();
  }
}

void ganti_giliran() {
  if (giliran_pemain == 1) {
    giliran_pemain = 2;
    lumbung_sekarang = 0;
    Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
  } else if (giliran_pemain == 2) {
    giliran_pemain = 1;
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
  if ( giliran_pemain == 1 && (n3 == 0) && (n4 == 0) && (n5 == 0) ) {
    //permainan selesai karena biji pada lumbung sudah habis
    GAME_STATE = 4;
    return true;
  } else if ( giliran_pemain == 2 && (n10 == 0) && (n11 == 0) && (n12 == 0) ) {
    GAME_STATE = 4;
    return true;
  }
  return false;
}

void clear_all_green(){
  //TODO: semua hijau mati
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
  if ( (p3 != 0) && (n3 == 0) && (n1 == p1) && (n2 == p2) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 3;
    biji_ditangan = p3;
    p3 = n3;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p4 != 0) && (n4 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 4;
    biji_ditangan = p4;
    p4 = n4;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p5 != 0) && (n5 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 5;
    biji_ditangan = p5;
    p5 = n5;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p10 != 0) && (n10 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 10;
    biji_ditangan = p10;
    p10 = n10;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 10 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p11 != 0) && (n11 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 11;
    biji_ditangan = p11;
    p11 = n11;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 11 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p12 != 0) && (n12 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
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
  if ( (p3 != 0) && (n3 == 0) && (n1 == p1) && (n2 == p2) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 3) ) {
    lumbung_sekarang = 3;
    biji_ditangan = p3;
    p3 = n3;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p4 != 0) && (n4 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 4) ) {
    lumbung_sekarang = 4;
    biji_ditangan = p4;
    p4 = n4;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p5 != 0) && (n5 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 5) ) {
    lumbung_sekarang = 5;
    biji_ditangan = p5;
    p5 = n5;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p10 != 0) && (n10 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 10) ) {
    lumbung_sekarang = 10;
    biji_ditangan = p10;
    p10 = n10;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 10 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p11 != 0) && (n11 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 11) ) {
    lumbung_sekarang = 11;
    biji_ditangan = p11;
    p11 = n11;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 11 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p12 != 0) && (n12 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 12) ) {
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

void check_aturan() {  
  if ( n3 < p3 ) {
    clear_all_green();
    msr.set_shift(18);
    //TODO: semua hijau mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
  } else if ( n3 == p3 ) {
    msr.clear_shift(18);
    msr.clear_shift(19);
    //TODO: lampu mati
  } else if ( n3 > p3 ) {
    clear_all_green();
    msr.set_shift(19);
    //TODO: semua hijau mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
  }
  if ( n4 < p4 ) {
    clear_all_green();
    msr.set_shift(22);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
  } else if ( n4 == p4 ) {
    msr.clear_shift(22);
    msr.clear_shift(23);
    //TODO: lampu mati
  } else if ( n4 > p4 ) {
    clear_all_green();
    msr.set_shift(23);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
  }
  if ( n5 < p5 ) {
    clear_all_green();
    msr.set_shift(16);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
  } else if ( n5 == p5 ) {
    msr.clear_shift(16);
    msr.clear_shift(15);
    //TODO: lampu mati
  } else if ( n5 > p5 ) {
    clear_all_green();
    msr.set_shift(15);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
  }
  if ( n10 < p10 ) {
    clear_all_green();
    msr.set_shift(10);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
  } else if ( n10 == p10 ) {
    msr.clear_shift(10);
    msr.clear_shift(9);
    //TODO: lampu mati
  } else if ( n10 > p10 ) {
    clear_all_green();
    msr.set_shift(9);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
  }
  if ( n11 < p11 ) {
    clear_all_green();
    msr.set_shift(0);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)    
    Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
  } else if ( n11 == p11 ) {
    msr.clear_shift(1);
    msr.clear_shift(0);
    //TODO: lampu mati
  } else if ( n11 > p11 ) {
    clear_all_green();
    msr.set_shift(1);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
  }
  if ( n12 < p12 ) {
    clear_all_green();
    msr.set_shift(8);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
  } else if ( n12 == p12 ) {
    msr.clear_shift(6);
    msr.clear_shift(8);
    //TODO: lampu mati
  } else if ( n12 > p12 ) {
    clear_all_green();
    msr.set_shift(6);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
  }
  if ( A < pA ) {
    clear_all_green();
    msr.set_shift(13);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
  } else if ( A == pA ) {
    msr.clear_shift(14);
    msr.clear_shift(13);
    //TODO: lampu mati
  } else if ( A > pA ) {
    clear_all_green();
    msr.set_shift(14);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }
  if ( B < pB ) {
    clear_all_green();
    msr.set_shift(3);
    //TODO: semua mati
    //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
  } else if ( B == pB ) {
    msr.clear_shift(5);
    msr.clear_shift(3);
    //TODO: lampu mati
  } else if ( B > pB ) {
    clear_all_green();
    msr.set_shift(5);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }
}

bool isSudahSiapMain() {  
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n3 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(18);
      //TODO: semua mati
      //TODO: lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
    } else if ( n3 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(19);
      msr.clear_shift(18);
      //TODO: lampu mati
    } else if ( n3 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(19);
      //TODO: semua mati
      //TODO: lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n4 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(22);
      //TODO: semua mati
      //TODO: lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
    } else if ( n4 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(23);
      msr.clear_shift(22);
      //TODO: lampu mati
    } else if ( n4 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(23);
      //TODO: semua mati
      //TODO: lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n5 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(16);
      //TODO: semua mati
      //TODO: lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
    } else if ( n5 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(15);
      msr.clear_shift(16);
      //TODO: lampu mati
    } else if ( n5 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(15);
      //TODO: semua mati
      //TODO: lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n10 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(10);
      //TODO: semua mati
      //TODO: lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
    } else if ( n10 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(9);
      msr.clear_shift(10);
      //TODO: lampu mati
    } else if ( n10 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(9);
      //TODO: semua mati
      //TODO: lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n11 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(0);
      //TODO: semua mati
      //TODO: lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
    } else if ( n11 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(1);
      msr.clear_shift(0);
      //TODO: lampu mati
    } else if ( n11 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(1);
      //TODO: semua mati
      //TODO: lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n12 < BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(8);
      //TODO: semua mati
      //TODO: lampu kuning (bijinya kurang)
      Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
    } else if ( n12 == BIJI_YANG_DIGUNAKAN ) {
      msr.clear_shift(6);
      msr.clear_shift(8);
      //TODO: lampu mati
    } else if ( n12 > BIJI_YANG_DIGUNAKAN ) {
      clear_all_green();
      msr.set_shift(6);
      //TODO: semua mati
      //TODO: lampu merah (bijinya lebih)
      Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
    }
  }
  if ( A > 0 ) {
    clear_all_green();
    msr.set_shift(14);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }else{
    msr.clear_shift(14);
    //TODO: semua mati
  }
  if ( B > 0 ) {
    clear_all_green();
    msr.set_shift(15);
    //TODO: semua mati
    //TODO: lampu merah (bijinya lebih)    
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }else{
    msr.clear_shift(15);
    //TODO: semua mati
  }

  if( (BIJI_YANG_DIGUNAKAN==3) ){
      if ( (n3 == BIJI_YANG_DIGUNAKAN) &&
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
      //TODO: semua mati
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
      //TODO: semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 5 !!! -------------");
    } else if ( (pA + 1 == A) && (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lA;
      pA = A;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(13);
      //TODO: semua mati
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
      //TODO: semua mati
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
      //TODO: semua mati
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
      //TODO: semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 12 !!! -------------");
    } else if ( (pB + 1 == B) && (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lB;
      pB = B;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(3);
      //TODO: semua mati
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
      //TODO: semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == lA) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(10);
      //TODO: semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == lB) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
      //TODO: semua mati
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else {
      check_aturan();
    }
  }
}

void printValue() {
  Serial.print( "Biji ditangan: " );
  Serial.print(biji_ditangan);
  Serial.print( " # Lumbung sekarang: " );
  Serial.print(lumbung_sekarang);
  Serial.print( " # giliran: " );
  Serial.print(giliran_pemain);
  Serial.print( " # lumbung dipakai: " );
  Serial.print(BIJI_YANG_DIGUNAKAN);
  Serial.print( " # state: " );
  Serial.println(GAME_STATE);

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

  if( (BIJI_YANG_DIGUNAKAN==3) ){
      Serial.print( " # p3: " );
      Serial.print(p3);
      Serial.print( " # p4: " );
      Serial.print(p4);
      Serial.print( " # p5: " );
      Serial.print(p5);
      Serial.print( " # pA: " );
      Serial.print(pA);
      Serial.print( "  ## " );
      Serial.print( " # p10: " );
      Serial.print(p10);
      Serial.print( " # p11: " );
      Serial.print(p11);
      Serial.print( " # p12: " );
      Serial.print(p12);
      Serial.print( " # pB: " );
      Serial.println(pB);
  }

  if( (BIJI_YANG_DIGUNAKAN==3) ){
      Serial.print( " # n3: " );
      Serial.print(n3);
      Serial.print( " # n4: " );
      Serial.print(n4);
      Serial.print( " # n5: " );
      Serial.print(n5);
      Serial.print( " #  A: " );
      Serial.print(A);
      Serial.print( "  ## " );
      Serial.print( " # n10: " );
      Serial.print(n10);
      Serial.print( " # n11: " );
      Serial.print(n11);
      Serial.print( " # n12: " );
      Serial.print(n12);
      Serial.print( " #  B: " );
      Serial.println(B);
  }
  
}

void detect3() {
  cell3.set_scale(wc3);

  r3 = cell3.get_units(), 10;
  if (r3 < 0) {
    r3 = 0.00;
  }
  n3 = round(r3 - 1) / AVG_BIJI;
}

void detect4() {
  cell4.set_scale(wc4);

  r4 = cell4.get_units(), 10;
  if (r4 < 0) {
    r4 = 0.00;
  }
  n4 = round(r4) / AVG_BIJI;
}

void detect5() {
  cell5.set_scale(wc5);

  r5 = cell5.get_units(), 10;
  if (r5 < 0) {
    r5 = 0.00;
  }
  n5 = round(r5) / AVG_BIJI;
}

void detect10() {
  cell10.set_scale(wc10);

  r10 = cell10.get_units(), 10;
  if (r10 < 0) {
    r10 = 0.00;
  }
  n10 = round(r10) / AVG_BIJI;
}

void detect11() {
  cell11.set_scale(wc11);

  r11 = cell11.get_units(), 10;
  if (r11 < 0) {
    r11 = 0.00;
  }
  n11 = round(r11) / AVG_BIJI;
}

void detect12() {
  cell12.set_scale(wc12);

  r12 = cell12.get_units(), 10;
  if (r12 < 0) {
    r12 = 0.00;
  }
  n12 = round(r12) / AVG_BIJI;
}

void detectA() {
  cellA.set_scale(wcA);

  rA = cellA.get_units(), 10;
  if (rA < 0) {
    rA = 0.00;
  }
  A = round(rA) / AVG_BIJI;
}

void detectB() {
  cellB.set_scale(wcB);

  rB = cellB.get_units(), 10;
  if (rB < 0) {
    rB = 0.00;
  }
  B = round(rB) / AVG_BIJI;
}

