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

int plusPin = 5; 
int minusPin = 6; 
int startPin = 7; 

int latchPin = 8; //8 STCP
int dataPin = 11; //11 DS
int clockPin = 12; //12 SHCP

int numberOfRegisters = 11;

MultiShiftRegister msr (numberOfRegisters , latchPin, clockPin, dataPin);

int lA = 15;
int lB = 16;
//1-14 / n1-n14 = lumbung
//15 / A = Rumah pemain 1
//16 / B = Rumah pemain 2

int GAME_STATE = -1;
// 0 = PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
// 1 = PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
// 2 = PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
// 3 = PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
// 4 = GANTI GILIRAN
// 5 = PERMAINAN SELESAI

HX711 cell14(22, 23);//
HX711 cell13(24, 25);//
HX711 cell12(26, 27);//
HX711 cell11(28, 29);//
HX711 cell10(30, 31);//
HX711 cell9(32, 33);//
HX711 cell8(34, 35);//
HX711 cell7(36, 37);//
HX711 cell6(38, 39);//
HX711 cell5(40, 41);//
HX711 cell4(42, 43);//
HX711 cell3(44, 45);//
HX711 cell2(46, 47);//
HX711 cell1(48, 49);//
HX711 cellA(50, 51);//
HX711 cellB(52, 53);//

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(plusPin, INPUT);
  pinMode(minusPin, INPUT);
  pinMode(startPin, INPUT);

  randomSeed(analogRead(0));

  initial_state();

  if(GAME_STATE = -1){
    segmentNum(BIJI_YANG_DIGUNAKAN);
  }
}

void initial_state() {
  cell1.set_scale();
  cell1.tare();
  cell2.set_scale();
  cell2.tare();
  cell3.set_scale();
  cell3.tare();
  cell4.set_scale();
  cell4.tare();
  cell5.set_scale();
  cell5.tare();
  cell6.set_scale();
  cell6.tare();
  cell7.set_scale();
  cell7.tare();
  cell8.set_scale();
  cell8.tare();
  cell9.set_scale();
  cell9.tare();
  cell10.set_scale();
  cell10.tare();
  cell11.set_scale();
  cell11.tare();
  cell12.set_scale();
  cell12.tare();
  cell13.set_scale();
  cell13.tare();
  cell14.set_scale();
  cell14.tare();
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

int wc3 = -890; //-890
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

int wc13 = -790; //-405
int n13 = 0;
float r13 = 0;
int p13 = 0;

int wc14 = -840; //-840
int n14 = 0;
float r14 = 0;
int p14 = 0;

int wcA = -810; //-810
int A = 0;
float rA = 0;
int pA = 0;

int wcB = -850; //-850
int B = 0;
float rB = 0;
int pB = 0;

void loop() {
  // put your main code here, to run repeatedly:

  //  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  //  delay(100);                       // wait for half a second
  //  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  //  delay(100);

  detect_lumbung();

  printValue();

  msr.set_shift(8);
  msr.set_shift(9);
  msr.set_shift(10);
  msr.set_shift(11);
  msr.set_shift(12);

  congklak_game();

//  delay(200);
}

void segmentNum(int num) {
  if(num==3){
    msr.set_shift(4);
    msr.set_shift(5);
    msr.clear_shift(0);
    msr.clear_shift(1);
    msr.clear_shift(2);
    msr.clear_shift(3);
    msr.clear_shift(6);
  }else if(num==4){
    msr.set_shift(4);
    msr.set_shift(0);
    msr.set_shift(3);
    msr.clear_shift(1);
    msr.clear_shift(2);
    msr.clear_shift(5);
    msr.clear_shift(6);
  }else if(num==5){
    msr.set_shift(4);
    msr.set_shift(1);
    msr.clear_shift(0);
    msr.clear_shift(2);
    msr.clear_shift(3);
    msr.clear_shift(5);
    msr.clear_shift(6);
  }else if(num==6){
    msr.set_shift(1);
    msr.clear_shift(2);
    msr.clear_shift(3);
    msr.clear_shift(4);
    msr.clear_shift(5);
    msr.clear_shift(6);
  }else if(num==7){
    msr.set_shift(3);
    msr.set_shift(4);
    msr.set_shift(5);
    msr.set_shift(6);
    msr.clear_shift(0);
    msr.clear_shift(1);
    msr.clear_shift(2);
  }
}

void congklak_game() {
  switch (GAME_STATE) {
    //PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
    case -1:
      Serial.println("---------- MENENTUKAN JUMLAH BIJI CONGKLAK !!! -------------");
      if(digitalRead(plusPin) == HIGH){
          Serial.print("+ TEMP BIJI = ");
          TEMP_BIJI = TEMP_BIJI + 1;
          if(TEMP_BIJI >= 7){
              TEMP_BIJI = 7;
          }else if(TEMP_BIJI <= 3){
              TEMP_BIJI = 3;
          }
          Serial.println(TEMP_BIJI);
          segmentNum(TEMP_BIJI);
      }else if(digitalRead(minusPin) == HIGH){
          Serial.print("- TEMP BIJI = ");
          TEMP_BIJI = TEMP_BIJI - 1;
          if(TEMP_BIJI >= 7){
              TEMP_BIJI = 7;
          }else if(TEMP_BIJI <= 3){
              TEMP_BIJI = 3;
          }
          Serial.println(TEMP_BIJI);
          segmentNum(TEMP_BIJI);
      }else if(digitalRead(startPin) == HIGH){
          Serial.println("---------- START !!! -------------");
          BIJI_YANG_DIGUNAKAN = TEMP_BIJI;
          GAME_STATE = 0;
      }
      break;
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
        if ( (p1 == 0) &&
             (p2 == 0) &&
             (p3 == 0) &&
             (p4 == 0) &&
             (p5 == 0) &&
             (p6 == 0) &&
             (p7 == 0) &&
             (p8 == 0) &&
             (p9 == 0) &&
             (p10 == 0) &&
             (p11 == 0) &&
             (p12 == 0) &&
             (p13 == 0) &&
             (p14 == 0)
           ) {
          p1 = n1;
          p2 = n2;
          p3 = n3;
          p4 = n4;
          p5 = n5;
          p6 = n6;
          p7 = n7;
          p8 = n8;
          p9 = n9;
          p10 = n10;
          p11 = n11;
          p12 = n12;
          p13 = n13;
          p14 = n14;
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
          if (n1 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 1 -------------");
            msr.set_shift(55);
          }
          if (n2 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 2 -------------");
            msr.set_shift(58);
          }
          if (n3 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 3 -------------");
            msr.set_shift(63);
          }
          if (n4 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 4 -------------");
            msr.set_shift(70);
          }
          if (n5 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 5 -------------");
            msr.set_shift(73);
          }
          if (n6 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 6 -------------");
            msr.set_shift(82);
          }
          if (n7 != 0 && (BIJI_YANG_DIGUNAKAN==7) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 7 -------------");
            msr.set_shift(87);
          }
        } else {
          if (n8 != 0 && (BIJI_YANG_DIGUNAKAN==7) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 8 -------------");
            msr.set_shift(10);
          }
          if (n9 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 9 -------------");
            msr.set_shift(15);
          }
          if (n10 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 10 -------------");
            msr.set_shift(20);
          }
          if (n11 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 11 -------------");
            msr.set_shift(24);
          }
          if (n12 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 12 -------------");
            msr.set_shift(29);
          }
          if (n13 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 13 -------------");
            msr.set_shift(36);
          }
          if (n14 != 0 && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
            Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 14 -------------");
            msr.set_shift(40);
          }
        }
        //TODO: tampilkan yang boleh dipilih lumbung
        detect_lumbung_dipilih_dari_rumah();
      } else {
        //TODO: tampilkan yang boleh dipilih lumbung lumbung_sekarang
        Serial.print("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : ");
        Serial.print(lumbung_sekarang);
        Serial.println(" -------------");
        if(lumbung_sekarang==1){
          msr.set_shift(55);
        }
        else if(lumbung_sekarang==2){
          msr.set_shift(58);
        }
        else if(lumbung_sekarang==3){
          msr.set_shift(63);
        }
        else if(lumbung_sekarang==4){
          msr.set_shift(70);
        }
        else if(lumbung_sekarang==5){
          msr.set_shift(73);
        }
        else if(lumbung_sekarang==6){
          msr.set_shift(82);
        }
        else if(lumbung_sekarang==7){
          msr.set_shift(87);
        }
        else if(lumbung_sekarang==8){
          msr.set_shift(10);
        }
        else if(lumbung_sekarang==9){
          msr.set_shift(15);
        }
        else if(lumbung_sekarang==10){
          msr.set_shift(20);
        }
        else if(lumbung_sekarang==11){
          msr.set_shift(24);
        }
        else if(lumbung_sekarang==12){
          msr.set_shift(29);
        }
        else if(lumbung_sekarang==13){
          msr.set_shift(36);
        }
        else if(lumbung_sekarang==14){
          msr.set_shift(40);
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
  if( (BIJI_YANG_DIGUNAKAN==7) ){
    detect1();
    detect14();
    detect2();
    detect13();
    detect3();
    detect12();
    detect4();
    detect11();
    detect5();
    detect10();
    detect6();
    detect9();
    detect7();
    detect8();
  } else if( (BIJI_YANG_DIGUNAKAN==6) ){
    detect1();
    detect14();
    detect2();
    detect13();
    detect3();
    detect12();
    detect4();
    detect11();
    detect5();
    detect10();
    detect6();
    detect9();
  } else if( (BIJI_YANG_DIGUNAKAN==5) ){
    detect2();
    detect13();
    detect3();
    detect12();
    detect4();
    detect11();
    detect5();
    detect10();
    detect6();
    detect9();
  } else if( (BIJI_YANG_DIGUNAKAN==4) ){
    detect2();
    detect13();
    detect3();
    detect12();
    detect4();
    detect11();
    detect5();
    detect10();
  } else if( (BIJI_YANG_DIGUNAKAN==3) ){
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
  // 14
  if ( (lumbung_sekarang == 14) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
    lumbung_lanjut = 1;
    msr.set_shift(57);
  } else if ( (lumbung_sekarang == 14) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
    lumbung_lanjut = lB;
    msr.set_shift(43);
  }
  // 7
  else if ( (lumbung_sekarang == 7) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==7) ) {
    lumbung_lanjut = lA;
    // msr.set_shift(43);
  } else if ( (lumbung_sekarang == 7) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==7) ) {
    lumbung_lanjut = 8;
    msr.set_shift(9);
  }
  // A / 15
  else if ( lumbung_sekarang == lA ) {
    if( (BIJI_YANG_DIGUNAKAN==7) ){
      lumbung_lanjut = 8;
      msr.set_shift(9);
    }else if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ){
      lumbung_lanjut = 9;
      msr.set_shift(14);
    }else if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
      lumbung_lanjut = 10;
      msr.set_shift(18);
    }
  }
  // B / 16
  else if ( lumbung_sekarang == lB ) {
    lumbung_lanjut = 1;
    if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ){
      lumbung_lanjut = 1;
      msr.set_shift(57);
    }else if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ){
      lumbung_lanjut = 2;
      msr.set_shift(60);
    }else if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
      lumbung_lanjut = 3;
      msr.set_shift(65);
    }
  } 
  // 1-6
  else if ( (lumbung_sekarang == 1) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
    lumbung_lanjut = 2;
    msr.set_shift(60);
  } else if ( (lumbung_sekarang == 2) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
    lumbung_lanjut = 3;
    msr.set_shift(65);
  } else if ( (lumbung_sekarang == 3) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 4;
    msr.set_shift(68);
  } else if ( (lumbung_sekarang == 4) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 5;
    msr.set_shift(74);
  } else if ( (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = lA;
    // msr.set_shift(74);
  } else if ( (lumbung_sekarang == 5) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 10;
    msr.set_shift(18);
  } else if ( (lumbung_sekarang == 5) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
    lumbung_lanjut = 6;
    msr.set_shift(80);
  } else if ( (lumbung_sekarang == 6) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
    lumbung_lanjut = lA;
    // msr.set_shift(80);
  } else if ( (lumbung_sekarang == 6) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
    lumbung_lanjut = 9;
    msr.set_shift(14);
  } else if ( (lumbung_sekarang == 6) && (BIJI_YANG_DIGUNAKAN==7) ) {
    lumbung_lanjut = 7;
    msr.set_shift(9);
  }
  // 8-13
  else if ( (lumbung_sekarang == 8) && (BIJI_YANG_DIGUNAKAN==7) ) {
    lumbung_lanjut = 9;
    msr.set_shift(14);
  } else if ( (lumbung_sekarang == 9) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
    lumbung_lanjut = 10;
    msr.set_shift(18);
  } else if ( (lumbung_sekarang == 10) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 11;
    msr.set_shift(25);
  } else if ( (lumbung_sekarang == 11) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 12;
    msr.set_shift(28);
  } else if ( (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = lB;
    msr.set_shift(43);
  } else if ( (lumbung_sekarang == 12) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==3) ) {
    lumbung_lanjut = 3;
    msr.set_shift(65);
  } else if ( (lumbung_sekarang == 12) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
    lumbung_lanjut = 13;
    msr.set_shift(34);
  } else if ( (lumbung_sekarang == 13) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
    lumbung_lanjut = lB;
    msr.set_shift(43);
  } else if ( (lumbung_sekarang == 13) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
    lumbung_lanjut = 2;
    msr.set_shift(60);
  } else if ( (lumbung_sekarang == 13) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
    lumbung_lanjut = 14;
    msr.set_shift(41);
  }
  // else {
  //   lumbung_lanjut = lumbung_sekarang + 1;
  // }
  Serial.print("---------- MASUKKAN BIJI KE LUMBUNG : ");
  Serial.print(lumbung_lanjut);
  Serial.println(" -------------");
}

void detect_if_tembak_diambil() {
  if ( lumbung_sekarang == 1 || lumbung_sekarang == 14 ) {
    msr.set_shift(53);
    msr.set_shift(39);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 1 dan 14 -------------");
    if ( (n1 == 0) && (n14 == 0)) {
      if (giliran_pemain == 1) {
        // msr.set_shift(60);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p1 + p14) == A) ) {
          msr.clear_shift(53);
          msr.clear_shift(39);
          // msr.clear_shift(43);
          p1 = n1;
          p14 = n14;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p1 + p14) == B) ) {
          msr.clear_shift(53);
          msr.clear_shift(39);
          msr.clear_shift(43);
          p1 = n1;
          p14 = n14;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 2 || lumbung_sekarang == 13 ) {
    msr.set_shift(33);
    msr.set_shift(62);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 2 dan 13 -------------");
    if ( (n13 == 0) && (n2 == 0) ) {
      if (giliran_pemain == 1) {
        // msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p2 + p13) == A) ) {
          msr.clear_shift(33);
          msr.clear_shift(62);
          // msr.clear_shift(43);
          p2 = n2;
          p13 = n13;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (B + (p2 + p13) == B) ) {
          msr.clear_shift(33);
          msr.clear_shift(62);
          msr.clear_shift(43);
          p2 = n2;
          p13 = n13;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 3 || lumbung_sekarang == 12 ) {
    msr.set_shift(30);
    msr.set_shift(66);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 3 dan 12 -------------");
    if ( (n3 == 0) && (n12 == 0) ) {
      if (giliran_pemain == 1) {
        // msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p3 + p12) == A) ) {
          msr.clear_shift(30);
          msr.clear_shift(66);
          // msr.clear_shift(43);
          p3 = n3;
          p12 = n12;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p3 + p12) == B) ) {
          msr.clear_shift(30);
          msr.clear_shift(66);
          msr.clear_shift(43);
          p3 = n3;
          p12 = n12;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 4 || lumbung_sekarang == 11 ) {
    msr.set_shift(27);
    msr.set_shift(72);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 4 dan 11 -------------");
    if ( (n4 == 0) && (n11 == 0) ) {
      if (giliran_pemain == 1) {
        // msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p4 + p11) == A) ) {
          msr.clear_shift(27);
          msr.clear_shift(72);
          // msr.clear_shift(43);
          p4 = n4;
          p11 = n11;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p4 + p11) == B) ) {
          msr.clear_shift(27);
          msr.clear_shift(72);
          msr.clear_shift(43);
          p4 = n4;
          p11 = n11;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 5 || lumbung_sekarang == 10 ) {
    msr.set_shift(19);
    msr.set_shift(77);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 5 dan 10 -------------");
    if ( (n5 == 0) && (n10 == 0) ) {
      if (giliran_pemain == 1) {
        // msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p5 + p10) == A) ) {
          msr.clear_shift(19);
          msr.clear_shift(77);
          // msr.clear_shift(43);
          p5 = n5;
          p10 = n10;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p5 + p10) == B) ) {
          msr.clear_shift(19);
          msr.clear_shift(77);
          msr.clear_shift(43);
          p5 = n5;
          p10 = n10;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 6 || lumbung_sekarang == 9 ) {
    msr.set_shift(13);
    msr.set_shift(78);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 6 dan 9 -------------");
    if ( (n6 == 0) && (n9 == 0) ) {
      if (giliran_pemain == 1) {
        // msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p6 + p9) == A) ) {
          msr.clear_shift(13);
          msr.clear_shift(78);
          // msr.clear_shift(43);
          p6 = n6;
          p9 = n9;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p6 + p9) == B) ) {
          msr.clear_shift(13);
          msr.clear_shift(78);
          msr.clear_shift(43);
          p6 = n6;
          p9 = n9;
          pB = B;
          GAME_STATE = 4;
        }
      }
    }
  } else if ( lumbung_sekarang == 7 || lumbung_sekarang == 8 ) {
    msr.set_shift(83);
    msr.set_shift(8);
    Serial.println("---------- AMBIL BIJI PADA LUMBUNG 7 dan 8 -------------");
    if ( (n7 == 0) && (n8 == 0) ) {
      if (giliran_pemain == 1) {
        // msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
        if ( (pA + (p7 + p8) == A) ) {
          msr.clear_shift(83);
          msr.clear_shift(8);
          // msr.clear_shift(43);
          p7 = n7;
          p8 = n8;
          pA = A;
          GAME_STATE = 4;
        }
      } else if (giliran_pemain == 2) {
        msr.set_shift(43);
        Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
        if ( (pB + (p7 + p8) == B) ) {
          msr.clear_shift(83);
          msr.clear_shift(8);
          msr.clear_shift(43);
          p7 = n7;
          p8 = n8;
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
  if ( (n1 == 0) && (n2 == 0) && (n3 == 0) && (n4 == 0) && (n5 == 0) && (n6 == 0) && (n7 == 0) && (n8 == 0) && (n9 == 0) && (n10 == 0) && (n11 == 0) && (n12 == 0) && (n13 == 0) && (n14 == 0) ) {
    //permainan selesai karena biji pada lumbung sudah habis
    GAME_STATE = 5;
  } else {
    GAME_STATE = 1;
  }
}

bool check_giliran_selesai() {
  if ( giliran_pemain == 1 && (n1 == 0) && (n2 == 0) && (n3 == 0) && (n4 == 0) && (n5 == 0) && (n6 == 0) && (n7 == 0) ) {
    //permainan selesai karena biji pada lumbung sudah habis
    GAME_STATE = 4;
    return true;
  } else if ( giliran_pemain == 2 && (n8 == 0) && (n9 == 0) && (n10 == 0) && (n11 == 0) && (n12 == 0) && (n13 == 0) && (n14 == 0) ) {
    GAME_STATE = 4;
    return true;
  }
  return false;
}

void clear_all_green(){
    msr.clear_shift(55);
    msr.clear_shift(58);
    msr.clear_shift(63);
    msr.clear_shift(70);
    msr.clear_shift(73);
    msr.clear_shift(82);
    msr.clear_shift(87);
    msr.clear_shift(10);
    msr.clear_shift(15);
    msr.clear_shift(20);
    msr.clear_shift(24);
    msr.clear_shift(29);
    msr.clear_shift(36);
    msr.clear_shift(40);
    msr.clear_shift(44);
    // msr.clear_shift();
}

void detect_lumbung_dipilih_dari_rumah() {
  //Mendeteksi lumbung yang dipilih pemain 1 = 1 2 3 4 5 6 7 || pemain 2 = 8 9 10 11 12 13 14
  if ( (p1 != 0) && (n1 == 0) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 1;
    biji_ditangan = p1;
    p1 = n1;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 1 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p2 != 0) && (n2 == 0) && (n1 == p1) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 2;
    biji_ditangan = p2;
    p2 = n2;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 2 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p3 != 0) && (n3 == 0) && (n1 == p1) && (n2 == p2) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
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
  } else if ( (p6 != 0) && (n6 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 6;
    biji_ditangan = p6;
    p6 = n6;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 6 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p7 != 0) && (n7 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lA) || (lumbung_sekarang == 0)) && (giliran_pemain == 1) ) {
    lumbung_sekarang = 7;
    biji_ditangan = p7;
    p7 = n7;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 7 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p8 != 0) && (n8 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 8;
    biji_ditangan = p8;
    p8 = n8;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 8 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p9 != 0) && (n9 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 9;
    biji_ditangan = p9;
    p9 = n9;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 9 DIPILIH !!! -------------");
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
  } else if ( (p13 != 0) && (n13 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n14 == p14) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 13;
    biji_ditangan = p13;
    p13 = n13;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 13 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p14 != 0) && (n14 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && ((lumbung_sekarang == lB) || (lumbung_sekarang == 0)) && (giliran_pemain == 2) ) {
    lumbung_sekarang = 14;
    biji_ditangan = p14;
    p14 = n14;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 14 DIPILIH !!! -------------");
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
  if ( (p1 != 0) && (n1 == 0) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 1) ) {
    lumbung_sekarang = 1;
    biji_ditangan = p1;
    p1 = n1;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 1 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p2 != 0) && (n2 == 0) && (n1 == p1) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 2) ) {
    lumbung_sekarang = 2;
    biji_ditangan = p2;
    p2 = n2;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 2 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p3 != 0) && (n3 == 0) && (n1 == p1) && (n2 == p2) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 3) ) {
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
  } else if ( (p6 != 0) && (n6 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 6) ) {
    lumbung_sekarang = 6;
    biji_ditangan = p6;
    p6 = n6;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 6 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p7 != 0) && (n7 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 7) ) {
    lumbung_sekarang = 7;
    biji_ditangan = p7;
    p7 = n7;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 7 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p8 != 0) && (n8 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 8) ) {
    lumbung_sekarang = 8;
    biji_ditangan = p8;
    p8 = n8;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 8 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p9 != 0) && (n9 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (n14 == p14) && (lumbung_sekarang == 9) ) {
    lumbung_sekarang = 9;
    biji_ditangan = p9;
    p9 = n9;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 9 DIPILIH !!! -------------");
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
  } else if ( (p13 != 0) && (n13 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n14 == p14) && (lumbung_sekarang == 13) ) {
    lumbung_sekarang = 13;
    biji_ditangan = p13;
    p13 = n13;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 13 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else if ( (p14 != 0) && (n14 == 0) && (n1 == p1) && (n2 == p2) && (n3 == p3) && (n4 == p4) && (n5 == p5) && (n6 == p6) && (n7 == p7) && (n8 == p8) && (n9 == p9) && (n10 == p10) && (n11 == p11) && (n12 == p12) && (n13 == p13) && (lumbung_sekarang == 14) ) {
    lumbung_sekarang = 14;
    biji_ditangan = p14;
    p14 = n14;
    GAME_STATE = 2;
    Serial.println("---------- LUMBUNG 14 DIPILIH !!! -------------");
    lumbung_terpilih = true;
  } else {
    check_aturan();
  }

  if(lumbung_terpilih){
    clear_all_green();
  }
}

void check_aturan() {
  if ( n1 < p1 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(54);
    msr.clear_shift(56);
    Serial.println("---------- LUMBUNG 1 KURANG BIJI !!! -------------");
  } else if ( n1 == p1 ) {
    //TODO: lampu mati
    msr.clear_shift(54);
    msr.clear_shift(56);
  } else if ( n1 > p1 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(56);
    msr.clear_shift(54);
    Serial.println("---------- LUMBUNG 1 LEBIH BIJI !!! -------------");
  }
  if ( n2 < p2 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(59);
    msr.clear_shift(61);
    Serial.println("---------- LUMBUNG 2 KURANG BIJI !!! -------------");
  } else if ( n2 == p2 ) {
    //TODO: lampu mati
    msr.clear_shift(59);
    msr.clear_shift(61);
  } else if ( n2 > p2 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(61);
    msr.clear_shift(59);
    Serial.println("---------- LUMBUNG 2 LEBIH BIJI !!! -------------");
  }
  if ( n3 < p3 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(64);
    msr.clear_shift(67);
    Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
  } else if ( n3 == p3 ) {
    //TODO: lampu mati
    msr.clear_shift(64);
    msr.clear_shift(67);
  } else if ( n3 > p3 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(67);
    msr.clear_shift(64);
    Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
  }
  if ( n4 < p4 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(71);
    msr.clear_shift(69);
    Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
  } else if ( n4 == p4 ) {
    //TODO: lampu mati
    msr.clear_shift(71);
    msr.clear_shift(69);
  } else if ( n4 > p4 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(69);
    msr.clear_shift(71);
    Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
  }
  if ( n5 < p5 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(76);
    msr.clear_shift(75);
    Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
  } else if ( n5 == p5 ) {
    //TODO: lampu mati
    msr.clear_shift(76);
    msr.clear_shift(75);
  } else if ( n5 > p5 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(75);
    msr.clear_shift(76);
    Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
  }
  if ( n6 < p6 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(79);
    msr.clear_shift(81);
    Serial.println("---------- LUMBUNG 6 KURANG BIJI !!! -------------");
  } else if ( n6 == p6 ) {
    //TODO: lampu mati
    msr.clear_shift(79);
    msr.clear_shift(81);
  } else if ( n6 > p6 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(81);
    msr.clear_shift(79);
    Serial.println("---------- LUMBUNG 6 LEBIH BIJI !!! -------------");
  }
  if ( n7 < p7 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(85);
    msr.clear_shift(84);
    Serial.println("---------- LUMBUNG 7 KURANG BIJI !!! -------------");
  } else if ( n7 == p7 ) {
    //TODO: lampu mati
    msr.clear_shift(85);
    msr.clear_shift(84);
  } else if ( n7 > p7 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(84);
    msr.clear_shift(85);
    Serial.println("---------- LUMBUNG 7 LEBIH BIJI !!! -------------");
  }
  if ( n8 < p8 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(12);
    msr.clear_shift(11);
    Serial.println("---------- LUMBUNG 8 KURANG BIJI !!! -------------");
  } else if ( n8 == p8 ) {
    //TODO: lampu mati
    msr.clear_shift(12);
    msr.clear_shift(11);
  } else if ( n8 > p8 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(11);
    msr.clear_shift(12);
    Serial.println("---------- LUMBUNG 8 LEBIH BIJI !!! -------------");
  }
  if ( n9 < p9 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(17);
    msr.clear_shift(16);
    Serial.println("---------- LUMBUNG 9 KURANG BIJI !!! -------------");
  } else if ( n9 == p9 ) {
    //TODO: lampu mati
    msr.clear_shift(17);
    msr.clear_shift(16);
  } else if ( n9 > p9 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(16);
    msr.clear_shift(17);
    Serial.println("---------- LUMBUNG 9 LEBIH BIJI !!! -------------");
  }
  if ( n10 < p10 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(21);
    msr.clear_shift(22);
    Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
  } else if ( n10 == p10 ) {
    //TODO: lampu mati
    msr.clear_shift(21);
    msr.clear_shift(22);
  } else if ( n10 > p10 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(22);
    msr.clear_shift(21);
    Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
  }
  if ( n11 < p11 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(23);
    msr.clear_shift(26);
    Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
  } else if ( n11 == p11 ) {
    //TODO: lampu mati
    msr.clear_shift(23);
    msr.clear_shift(26);
  } else if ( n11 > p11 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(26);
    msr.clear_shift(23);
    Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
  }
  if ( n12 < p12 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(32);
    msr.clear_shift(31);
    Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
  } else if ( n12 == p12 ) {
    //TODO: lampu mati
    msr.clear_shift(32);
    msr.clear_shift(31);
  } else if ( n12 > p12 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(31);
    msr.clear_shift(32);
    Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
  }
  if ( n13 < p13 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(35);
    msr.clear_shift(37);
    Serial.println("---------- LUMBUNG 13 KURANG BIJI !!! -------------");
  } else if ( n13 == p13 ) {
    //TODO: lampu mati
    msr.clear_shift(35);
    msr.clear_shift(37);
  } else if ( n13 > p13 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(37);
    msr.clear_shift(35);
    Serial.println("---------- LUMBUNG 13 LEBIH BIJI !!! -------------");
  }
  if ( n14 < p14 ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(38);
    msr.clear_shift(42);
    Serial.println("---------- LUMBUNG 14 KURANG BIJI !!! -------------");
  } else if ( n14 == p14 ) {
    //TODO: lampu mati
    msr.clear_shift(38);
    msr.clear_shift(42);
  } else if ( n14 > p14 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(42);
    msr.clear_shift(38);
    Serial.println("---------- LUMBUNG 14 LEBIH BIJI !!! -------------");
  }
  if ( A < pA ) {
    //TODO: lampu kuning (bijinya kurang)
    // msr.set_shift(38);
    // msr.clear_shift(42);
    Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
  } else if ( A == pA ) {
    //TODO: lampu mati
    // msr.clear_shift(38);
    // msr.clear_shift(42);
  } else if ( A > pA ) {
    //TODO: lampu merah (bijinya lebih)
    // msr.set_shift(42);
    // msr.clear_shift(38);
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }
  if ( B < pB ) {
    //TODO: lampu kuning (bijinya kurang)
    msr.set_shift(45);
    msr.clear_shift(46);
    Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
  } else if ( B == pB ) {
    //TODO: lampu mati
    msr.clear_shift(45);
    msr.clear_shift(46);
  } else if ( B > pB ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(46);
    msr.clear_shift(45);
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }
}

bool isSudahSiapMain() {
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ){
    if ( n1 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(54);
      msr.clear_shift(56);
      Serial.println("---------- LUMBUNG 1 KURANG BIJI !!! -------------");
    } else if ( n1 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(54);
      msr.clear_shift(56);
    } else if ( n1 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(56);
      msr.clear_shift(54);
      Serial.println("---------- LUMBUNG 1 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ){
    if ( n2 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(59);
      msr.clear_shift(61);
      Serial.println("---------- LUMBUNG 2 KURANG BIJI !!! -------------");
    } else if ( n2 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(59);
      msr.clear_shift(61);
    } else if ( n2 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(61);
      msr.clear_shift(59);
      Serial.println("---------- LUMBUNG 2 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n3 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(64);
      msr.clear_shift(67);
      Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
    } else if ( n3 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(64);
      msr.clear_shift(67);
    } else if ( n3 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(67);
      msr.clear_shift(64);
      Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n4 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(71);
      msr.clear_shift(69);
      Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
    } else if ( n4 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(71);
      msr.clear_shift(69);
    } else if ( n4 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(69);
      msr.clear_shift(71);
      Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n5 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(76);
      msr.clear_shift(75);
      Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
    } else if ( n5 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(76);
      msr.clear_shift(75);
    } else if ( n5 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(75);
      msr.clear_shift(76);
      Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ){
    if ( n6 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(79);
      msr.clear_shift(81);
      Serial.println("---------- LUMBUNG 6 KURANG BIJI !!! -------------");
    } else if ( n6 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(79);
      msr.clear_shift(81);
    } else if ( n6 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(81);
      msr.clear_shift(79);
      Serial.println("---------- LUMBUNG 6 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7) ){
    if ( n7 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(85);
      msr.clear_shift(84);
      Serial.println("---------- LUMBUNG 7 KURANG BIJI !!! -------------");
    } else if ( n7 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(85);
      msr.clear_shift(84);
    } else if ( n7 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(84);
      msr.clear_shift(85);
      Serial.println("---------- LUMBUNG 7 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7) ){
    if ( n8 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(12);
      msr.clear_shift(11);
      Serial.println("---------- LUMBUNG 8 KURANG BIJI !!! -------------");
    } else if ( n8 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(12);
      msr.clear_shift(11);
    } else if ( n8 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(11);
      msr.clear_shift(12);
      Serial.println("---------- LUMBUNG 8 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ){
    if ( n9 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(17);
      msr.clear_shift(16);
      Serial.println("---------- LUMBUNG 9 KURANG BIJI !!! -------------");
    } else if ( n9 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(17);
      msr.clear_shift(16);
    } else if ( n9 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(16);
      msr.clear_shift(17);
      Serial.println("---------- LUMBUNG 9 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n10 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(21);
      msr.clear_shift(22);
      Serial.println("---------- LUMBUNG 10 KURANG BIJI !!! -------------");
    } else if ( n10 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(21);
      msr.clear_shift(22);
    } else if ( n10 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(22);
      msr.clear_shift(21);
      Serial.println("---------- LUMBUNG 10 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n11 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(23);
      msr.clear_shift(26);
      Serial.println("---------- LUMBUNG 11 KURANG BIJI !!! -------------");
    } else if ( n11 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(23);
      msr.clear_shift(26);
    } else if ( n11 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(26);
      msr.clear_shift(23);
      Serial.println("---------- LUMBUNG 11 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ){
    if ( n12 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(32);
      msr.clear_shift(31);
      Serial.println("---------- LUMBUNG 12 KURANG BIJI !!! -------------");
    } else if ( n12 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(32);
      msr.clear_shift(31);
    } else if ( n12 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(31);
      msr.clear_shift(32);
      Serial.println("---------- LUMBUNG 12 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ){
    if ( n13 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(35);
      msr.clear_shift(37);
      Serial.println("---------- LUMBUNG 13 KURANG BIJI !!! -------------");
    } else if ( n13 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(35);
      msr.clear_shift(37);
    } else if ( n13 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(37);
      msr.clear_shift(35);
      Serial.println("---------- LUMBUNG 13 LEBIH BIJI !!! -------------");
    }
  }
  if( (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ){
    if ( n14 < BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu kuning (bijinya kurang)
      msr.set_shift(38);
      msr.clear_shift(42);
      Serial.println("---------- LUMBUNG 14 KURANG BIJI !!! -------------");
    } else if ( n14 == BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu mati
      msr.clear_shift(38);
      msr.clear_shift(42);
    } else if ( n14 > BIJI_YANG_DIGUNAKAN ) {
      //TODO: lampu merah (bijinya lebih)
      msr.set_shift(42);
      msr.clear_shift(38);
      Serial.println("---------- LUMBUNG 14 LEBIH BIJI !!! -------------");
    }
  }
  if ( A > 0 ) {
    //TODO: lampu merah (bijinya lebih)
    // msr.set_shift(38);
    Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
  }else{
    // msr.clear_shift(42);
  }
  if ( B > 0 ) {
    //TODO: lampu merah (bijinya lebih)
    msr.set_shift(46);
    Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
  }else{
    msr.clear_shift(46);
  }

  if( (BIJI_YANG_DIGUNAKAN==7) ){
      if ( (n1 == BIJI_YANG_DIGUNAKAN) &&
              (n2 == BIJI_YANG_DIGUNAKAN) &&
              (n3 == BIJI_YANG_DIGUNAKAN) &&
              (n4 == BIJI_YANG_DIGUNAKAN) &&
              (n5 == BIJI_YANG_DIGUNAKAN) &&
              (n6 == BIJI_YANG_DIGUNAKAN) &&
              (n7 == BIJI_YANG_DIGUNAKAN) &&
              (n8 == BIJI_YANG_DIGUNAKAN) &&
              (n9 == BIJI_YANG_DIGUNAKAN) &&
              (n10 == BIJI_YANG_DIGUNAKAN) &&
              (n11 == BIJI_YANG_DIGUNAKAN) &&
              (n12 == BIJI_YANG_DIGUNAKAN) &&
              (n13 == BIJI_YANG_DIGUNAKAN) &&
              (n14 == BIJI_YANG_DIGUNAKAN) &&
              (A == 0) && (B == 0) ) {
        return true;
      } else {
        return false;
      }
  } else if( (BIJI_YANG_DIGUNAKAN==6) ){
      if ( (n1 == BIJI_YANG_DIGUNAKAN) &&
              (n2 == BIJI_YANG_DIGUNAKAN) &&
              (n3 == BIJI_YANG_DIGUNAKAN) &&
              (n4 == BIJI_YANG_DIGUNAKAN) &&
              (n5 == BIJI_YANG_DIGUNAKAN) &&
              (n6 == BIJI_YANG_DIGUNAKAN) &&
              (n9 == BIJI_YANG_DIGUNAKAN) &&
              (n10 == BIJI_YANG_DIGUNAKAN) &&
              (n11 == BIJI_YANG_DIGUNAKAN) &&
              (n12 == BIJI_YANG_DIGUNAKAN) &&
              (n13 == BIJI_YANG_DIGUNAKAN) &&
              (n14 == BIJI_YANG_DIGUNAKAN) &&
              (A == 0) && (B == 0) ) {
        return true;
      } else {
        return false;
      }
  } else if( (BIJI_YANG_DIGUNAKAN==5) ){
      if ( (n2 == BIJI_YANG_DIGUNAKAN) &&
              (n3 == BIJI_YANG_DIGUNAKAN) &&
              (n4 == BIJI_YANG_DIGUNAKAN) &&
              (n5 == BIJI_YANG_DIGUNAKAN) &&
              (n6 == BIJI_YANG_DIGUNAKAN) &&
              (n9 == BIJI_YANG_DIGUNAKAN) &&
              (n10 == BIJI_YANG_DIGUNAKAN) &&
              (n11 == BIJI_YANG_DIGUNAKAN) &&
              (n12 == BIJI_YANG_DIGUNAKAN) &&
              (n13 == BIJI_YANG_DIGUNAKAN) &&
              (A == 0) && (B == 0) ) {
        return true;
      } else {
        return false;
      }
  } else if( (BIJI_YANG_DIGUNAKAN==4) ){
      if ( (n2 == BIJI_YANG_DIGUNAKAN) &&
              (n3 == BIJI_YANG_DIGUNAKAN) &&
              (n4 == BIJI_YANG_DIGUNAKAN) &&
              (n5 == BIJI_YANG_DIGUNAKAN) &&
              (n10 == BIJI_YANG_DIGUNAKAN) &&
              (n11 == BIJI_YANG_DIGUNAKAN) &&
              (n12 == BIJI_YANG_DIGUNAKAN) &&
              (n13 == BIJI_YANG_DIGUNAKAN) &&
              (A == 0) && (B == 0) ) {
        return true;
      } else {
        return false;
      }
  } else if( (BIJI_YANG_DIGUNAKAN==3) ){
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
    if ( (p2 + 1 == n2) && (lumbung_sekarang == 1) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
      if ( (giliran_pemain == 1) && (p2 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p2 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 2;
      p2 = n2;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(60);
      Serial.println("---------- LUMBUNG SEKARANG = 2 !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == 2) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
      if ( (giliran_pemain == 1) && (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(65);
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else if ( (p4 + 1 == n4) && (lumbung_sekarang == 3) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p4 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p4 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 4;
      p4 = n4;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(68);
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
      msr.clear_shift(74);
      Serial.println("---------- LUMBUNG SEKARANG = 5 !!! -------------");
    } else if ( (p6 + 1 == n6) && (lumbung_sekarang == 5) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
      if ( (giliran_pemain == 1) && (p6 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p6 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 6;
      p6 = n6;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(80);
      Serial.println("---------- LUMBUNG SEKARANG = 6 !!! -------------");
    } else if ( (pA + 1 == A) && (lumbung_sekarang == 5) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lA;
      pA = A;
      biji_ditangan = biji_ditangan - 1;
      // msr.clear_shift(82);
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
      msr.clear_shift(18);
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p7 + 1 == n7) && (lumbung_sekarang == 6) && (BIJI_YANG_DIGUNAKAN==7) ) {
      if ( (giliran_pemain == 1) && (p7 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p7 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 7;
      p7 = n7;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(86);
      Serial.println("---------- LUMBUNG SEKARANG = 7 !!! -------------");
    } else if ( (pA + 1 == A) && (lumbung_sekarang == 6) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
      lumbung_sekarang = lA;
      pA = A;
      biji_ditangan = biji_ditangan - 1;
      // msr.clear_shift(20);
      Serial.println("---------- LUMBUNG SEKARANG = A !!! -------------");
    } else if ( (p9 + 1 == n9) && (lumbung_sekarang == 6) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
      if ( (giliran_pemain == 2) && (p9 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p9 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 9;
      p9 = n9;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(14);
      Serial.println("---------- LUMBUNG SEKARANG = 9 !!! -------------");
    } else if ( (pA + 1 == A) && (lumbung_sekarang == 7) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==7) ) {
      lumbung_sekarang = lA;
      pA = A;
      biji_ditangan = biji_ditangan - 1;
      // msr.clear_shift(20);
      Serial.println("---------- LUMBUNG SEKARANG = A !!! -------------");
    } else if ( (p8 + 1 == n8) && (lumbung_sekarang == 7 && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==7) ) ) {
      if ( (giliran_pemain == 2) && (p8 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p8 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 8;
      p8 = n8;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(9);
      Serial.println("---------- LUMBUNG SEKARANG = 8 !!! -------------");
    } 
    //8-14 
    else if ( (p9 + 1 == n9) && (lumbung_sekarang == 8) && (BIJI_YANG_DIGUNAKAN==7) ) {
      if ( (giliran_pemain == 2) && (p9 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p9 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 9;
      p9 = n9;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(14);
      Serial.println("---------- LUMBUNG SEKARANG = 9 !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == 9) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
      if ( (giliran_pemain == 2) && (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
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
      msr.clear_shift(25);
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
      msr.clear_shift(28);
      Serial.println("---------- LUMBUNG SEKARANG = 12 !!! -------------");
    } else if ( (p13 + 1 == n13) && (lumbung_sekarang == 12) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
      if ( (giliran_pemain == 2) && (p13 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p13 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 13;
      p13 = n13;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(34);
      Serial.println("---------- LUMBUNG SEKARANG = 13 !!! -------------");
    } else if ( (pB + 1 == B) && (lumbung_sekarang == 12) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      lumbung_sekarang = lB;
      pB = B;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(43);
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
      msr.clear_shift(65);
      Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
    } else if ( (p14 + 1 == n14) && (lumbung_sekarang == 13) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
      if ( (giliran_pemain == 2) && (p14 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p14 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 14;
      p14 = n14;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(41);
      Serial.println("---------- LUMBUNG SEKARANG = 14 !!! -------------");
    } else if ( (pB + 1 == B) && (lumbung_sekarang == 13) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
      lumbung_sekarang = lB;
      pB = B;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(43);
      Serial.println("---------- LUMBUNG SEKARANG = B !!! -------------");
    } else if ( (p2 + 1 == n2) && (lumbung_sekarang == 13) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
      if ( (giliran_pemain == 1) && (p2 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p2 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 2;
      p2 = n2;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(60);
      Serial.println("---------- LUMBUNG SEKARANG = 2 !!! -------------");
    } else if ( (pB + 1 == B) && (lumbung_sekarang == 14) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
      lumbung_sekarang = lB;
      pB = B;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(43);
      Serial.println("---------- LUMBUNG SEKARANG = B !!! -------------");
    } else if ( (p1 + 1 == n1) && (lumbung_sekarang == 14) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
      if ( (giliran_pemain == 1) && (p1 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 3;
      } else if ( (p1 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 1;
      p1 = n1;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(57);
      Serial.println("---------- LUMBUNG SEKARANG = 1 !!! -------------");
    } else if ( (p8 + 1 == n8) && (lumbung_sekarang == lA) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==7) ) {
      if ( (giliran_pemain == 1) && (p8 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 8;
      p8 = n8;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(9);
      Serial.println("---------- LUMBUNG SEKARANG = 8 !!! -------------");
    } else if ( (p9 + 1 == n9) && (lumbung_sekarang == lA) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==6 || BIJI_YANG_DIGUNAKAN==5) ) {
      if ( (giliran_pemain == 1) && (p9 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 9;
      p9 = n9;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(14);
      Serial.println("---------- LUMBUNG SEKARANG = 9 !!! -------------");
    } else if ( (p10 + 1 == n10) && (lumbung_sekarang == lA) && (giliran_pemain == 1) && (BIJI_YANG_DIGUNAKAN==4 || BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 1) && (p10 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 10;
      p10 = n10;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(18);
      Serial.println("---------- LUMBUNG SEKARANG = 10 !!! -------------");
    } else if ( (p1 + 1 == n1) && (lumbung_sekarang == lB) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==7 || BIJI_YANG_DIGUNAKAN==6) ) {
      if ( (giliran_pemain == 2) && (p1 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 1;
      p1 = n1;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(57);
      Serial.println("---------- LUMBUNG SEKARANG = 1 !!! -------------");
    } else if ( (p2 + 1 == n2) && (lumbung_sekarang == lB) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==5 || BIJI_YANG_DIGUNAKAN==4) ) {
      if ( (giliran_pemain == 2) && (p2 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 2;
      p2 = n2;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(60);
      Serial.println("---------- LUMBUNG SEKARANG = 2 !!! -------------");
    } else if ( (p3 + 1 == n3) && (lumbung_sekarang == lB) && (giliran_pemain == 2) && (BIJI_YANG_DIGUNAKAN==3) ) {
      if ( (giliran_pemain == 2) && (p3 == 0) && (biji_ditangan == 1) ) {
        GAME_STATE = 4;
      }
      lumbung_sekarang = 3;
      p3 = n3;
      biji_ditangan = biji_ditangan - 1;
      msr.clear_shift(65);
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

  if( (BIJI_YANG_DIGUNAKAN==7) ){
      Serial.print( " r1: " );
      Serial.print(r1);
      Serial.print( " # r2: " );
      Serial.print(r2);
      Serial.print( " # r3: " );
      Serial.print(r3);
      Serial.print( " # r4: " );
      Serial.print(r4);
      Serial.print( " # r5: " );
      Serial.print(r5);
      Serial.print( " # r6: " );
      Serial.print(r6);
      Serial.print( " # r7: " );
      Serial.print(r7);
      Serial.print( " # rA: " );
      Serial.print(rA);
      Serial.print( "  ## " );
      Serial.print( " # r8: " );
      Serial.print(r8);
      Serial.print( " # r9: " );
      Serial.print(r9);
      Serial.print( " # r10: " );
      Serial.print(r10);
      Serial.print( " # r11: " );
      Serial.print(r11);
      Serial.print( " # r12: " );
      Serial.print(r12);
      Serial.print( " # r13: " );
      Serial.print(r13);
      Serial.print( " # r14: " );
      Serial.print(r14);
      Serial.print( " # rB: " );
      Serial.println(rB);
  } else if( (BIJI_YANG_DIGUNAKAN==6) ){
     Serial.print( " r1: " );
      Serial.print(r1);
      Serial.print( " # r2: " );
      Serial.print(r2);
      Serial.print( " # r3: " );
      Serial.print(r3);
      Serial.print( " # r4: " );
      Serial.print(r4);
      Serial.print( " # r5: " );
      Serial.print(r5);
      Serial.print( " # r6: " );
      Serial.print(r6);
      Serial.print( " # rA: " );
      Serial.print(rA);
      Serial.print( "  ## " );
      Serial.print( " # r9: " );
      Serial.print(r9);
      Serial.print( " # r10: " );
      Serial.print(r10);
      Serial.print( " # r11: " );
      Serial.print(r11);
      Serial.print( " # r12: " );
      Serial.print(r12);
      Serial.print( " # r13: " );
      Serial.print(r13);
      Serial.print( " # r14: " );
      Serial.print(r14);
      Serial.print( " # rB: " );
      Serial.println(rB);
  } else if( (BIJI_YANG_DIGUNAKAN==5) ){
      Serial.print( " # r2: " );
      Serial.print(r2);
      Serial.print( " # r3: " );
      Serial.print(r3);
      Serial.print( " # r4: " );
      Serial.print(r4);
      Serial.print( " # r5: " );
      Serial.print(r5);
      Serial.print( " # r6: " );
      Serial.print(r6);
      Serial.print( " # rA: " );
      Serial.print(rA);
      Serial.print( "  ## " );
      Serial.print( " # r9: " );
      Serial.print(r9);
      Serial.print( " # r10: " );
      Serial.print(r10);
      Serial.print( " # r11: " );
      Serial.print(r11);
      Serial.print( " # r12: " );
      Serial.print(r12);
      Serial.print( " # r13: " );
      Serial.print(r13);
      Serial.print( " # rB: " );
      Serial.println(rB);
  } else if( (BIJI_YANG_DIGUNAKAN==4) ){
      Serial.print( " # r2: " );
      Serial.print(r2);
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
      Serial.print( " # r13: " );
      Serial.print(r13);
      Serial.print( " # rB: " );
      Serial.println(rB);
  } else if( (BIJI_YANG_DIGUNAKAN==3) ){
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

  if( (BIJI_YANG_DIGUNAKAN==7) ){
      Serial.print( " p1: " );
      Serial.print(p1);
      Serial.print( " # p2: " );
      Serial.print(p2);
      Serial.print( " # p3: " );
      Serial.print(p3);
      Serial.print( " # p4: " );
      Serial.print(p4);
      Serial.print( " # p5: " );
      Serial.print(p5);
      Serial.print( " # p6: " );
      Serial.print(p6);
      Serial.print( " # p7: " );
      Serial.print(p7);
      Serial.print( " # pA: " );
      Serial.print(pA);
      Serial.print( "  ## " );
      Serial.print( " # p8: " );
      Serial.print(p8);
      Serial.print( " # p9: " );
      Serial.print(p9);
      Serial.print( " # p10: " );
      Serial.print(p10);
      Serial.print( " # p11: " );
      Serial.print(p11);
      Serial.print( " # p12: " );
      Serial.print(p12);
      Serial.print( " # p13: " );
      Serial.print(p13);
      Serial.print( " # p14: " );
      Serial.print(p14);
      Serial.print( " # pB: " );
      Serial.println(pB);
  } else if( (BIJI_YANG_DIGUNAKAN==6) ){
      Serial.print( " p1: " );
      Serial.print(p1);
      Serial.print( " # p2: " );
      Serial.print(p2);
      Serial.print( " # p3: " );
      Serial.print(p3);
      Serial.print( " # p4: " );
      Serial.print(p4);
      Serial.print( " # p5: " );
      Serial.print(p5);
      Serial.print( " # p6: " );
      Serial.print(p6);
      Serial.print( " # pA: " );
      Serial.print(pA);
      Serial.print( "  ## " );
      Serial.print( " # p9: " );
      Serial.print(p9);
      Serial.print( " # p10: " );
      Serial.print(p10);
      Serial.print( " # p11: " );
      Serial.print(p11);
      Serial.print( " # p12: " );
      Serial.print(p12);
      Serial.print( " # p13: " );
      Serial.print(p13);
      Serial.print( " # p14: " );
      Serial.print(p14);
      Serial.print( " # pB: " );
      Serial.println(pB);
  } else if( (BIJI_YANG_DIGUNAKAN==5) ){
      Serial.print( " # p2: " );
      Serial.print(p2);
      Serial.print( " # p3: " );
      Serial.print(p3);
      Serial.print( " # p4: " );
      Serial.print(p4);
      Serial.print( " # p5: " );
      Serial.print(p5);
      Serial.print( " # p6: " );
      Serial.print(p6);
      Serial.print( " # pA: " );
      Serial.print(pA);
      Serial.print( "  ## " );
      Serial.print( " # p9: " );
      Serial.print(p9);
      Serial.print( " # p10: " );
      Serial.print(p10);
      Serial.print( " # p11: " );
      Serial.print(p11);
      Serial.print( " # p12: " );
      Serial.print(p12);
      Serial.print( " # p13: " );
      Serial.print(p13);
      Serial.print( " # pB: " );
      Serial.println(pB);
  } else if( (BIJI_YANG_DIGUNAKAN==4) ){
      Serial.print( " # p2: " );
      Serial.print(p2);
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
      Serial.print( " # p13: " );
      Serial.print(p13);
      Serial.print( " # pB: " );
      Serial.println(pB);
  } else if( (BIJI_YANG_DIGUNAKAN==3) ){
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

  if( (BIJI_YANG_DIGUNAKAN==7) ){
      Serial.print( " n1: " );
      Serial.print(n1);
      Serial.print( " # n2: " );
      Serial.print(n2);
      Serial.print( " # n3: " );
      Serial.print(n3);
      Serial.print( " # n4: " );
      Serial.print(n4);
      Serial.print( " # n5: " );
      Serial.print(n5);
      Serial.print( " # n6: " );
      Serial.print(n6);
      Serial.print( " # n7: " );
      Serial.print(n7);
      Serial.print( " #  A: " );
      Serial.print(A);
      Serial.print( "  ## " );
      Serial.print( " # n8: " );
      Serial.print(n8);
      Serial.print( " # n9: " );
      Serial.print(n9);
      Serial.print( " # n10: " );
      Serial.print(n10);
      Serial.print( " # n11: " );
      Serial.print(n11);
      Serial.print( " # n12: " );
      Serial.print(n12);
      Serial.print( " # n13: " );
      Serial.print(n13);
      Serial.print( " # n14: " );
      Serial.print(n14);
      Serial.print( " #  B: " );
      Serial.println(B);
  } else if( (BIJI_YANG_DIGUNAKAN==6) ){
      Serial.print( " n1: " );
      Serial.print(n1);
      Serial.print( " # n2: " );
      Serial.print(n2);
      Serial.print( " # n3: " );
      Serial.print(n3);
      Serial.print( " # n4: " );
      Serial.print(n4);
      Serial.print( " # n5: " );
      Serial.print(n5);
      Serial.print( " # n6: " );
      Serial.print(n6);
      Serial.print( " #  A: " );
      Serial.print(A);
      Serial.print( "  ## " );
      Serial.print( " # n9: " );
      Serial.print(n9);
      Serial.print( " # n10: " );
      Serial.print(n10);
      Serial.print( " # n11: " );
      Serial.print(n11);
      Serial.print( " # n12: " );
      Serial.print(n12);
      Serial.print( " # n13: " );
      Serial.print(n13);
      Serial.print( " # n14: " );
      Serial.print(n14);
      Serial.print( " #  B: " );
      Serial.println(B);
  } else if( (BIJI_YANG_DIGUNAKAN==5) ){
      Serial.print( " # n2: " );
      Serial.print(n2);
      Serial.print( " # n3: " );
      Serial.print(n3);
      Serial.print( " # n4: " );
      Serial.print(n4);
      Serial.print( " # n5: " );
      Serial.print(n5);
      Serial.print( " # n6: " );
      Serial.print(n6);
      Serial.print( " #  A: " );
      Serial.print(A);
      Serial.print( "  ## " );
      Serial.print( " # n9: " );
      Serial.print(n9);
      Serial.print( " # n10: " );
      Serial.print(n10);
      Serial.print( " # n11: " );
      Serial.print(n11);
      Serial.print( " # n12: " );
      Serial.print(n12);
      Serial.print( " # n13: " );
      Serial.print(n13);
      Serial.print( " #  B: " );
      Serial.println(B);
  } else if( (BIJI_YANG_DIGUNAKAN==4) ){
      Serial.print( " # n2: " );
      Serial.print(n2);
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
      Serial.print( " # n13: " );
      Serial.print(n13);
      Serial.print( " #  B: " );
      Serial.println(B);
  } else if( (BIJI_YANG_DIGUNAKAN==3) ){
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

void detect1() {
  cell1.set_scale(wc1);

  r1 = cell1.get_units(), 10;
  if (r1 < 0) {
    r1 = 0.00;
  }
  n1 = round(r1) / AVG_BIJI;
}

void detect2() {
  cell2.set_scale(wc2);

  r2 = cell2.get_units(), 10;
  if (r2 < 0) {
    r2 = 0.00;
  }
  n2 = round(r2) / AVG_BIJI;
}

void detect3() {
  cell3.set_scale(wc3);

  r3 = cell3.get_units(), 10;
  if (r3 < 0) {
    r3 = 0.00;
  }
  n3 = round(r3) / AVG_BIJI;
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

void detect6() {
  cell6.set_scale(wc6);

  r6 = cell6.get_units(), 10;
  if (r6 < 0) {
    r6 = 0.00;
  }
  n6 = round(r6) / AVG_BIJI;
}

void detect7() {
  cell7.set_scale(wc7);

  r7 = cell7.get_units(), 10;
  if (r7 < 0) {
    r7 = 0.00;
  }
  n7 = round(r7) / AVG_BIJI;
}

void detect8() {
  cell8.set_scale(wc8);

  r8 = cell8.get_units(), 10;
  if (r8 < 0) {
    r8 = 0.00;
  }
  n8 = round(r8) / AVG_BIJI;
}

void detect9() {
  cell9.set_scale(wc9);

  r9 = cell9.get_units(), 10;
  if (r9 < 0) {
    r9 = 0.00;
  }
  n9 = round(r9) / AVG_BIJI;
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

void detect13() {
  cell13.set_scale(wc13);

  r13 = cell13.get_units(), 10;
  if (r13 < 0) {
    r13 = 0.00;
  }
  n13 = round(r13) / AVG_BIJI;
}

void detect14() {
  cell14.set_scale(wc14);

  r14 = cell14.get_units(), 10;
  if (r14 < 0) {
    r14 = 0.00;
  }
  n14 = round(r14) / AVG_BIJI;
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

