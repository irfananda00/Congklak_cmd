#include <HX711.h>

float GAMEREADY_BATASBAWAH = 3.5;
float GAMEREADY_BATASATAS = 4.9;

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
float p1 = 0;
bool isn1_dilanggar = false;

float wc2 = -2191; //-2191
float n2 = 0;
float p2 = 0;
bool isn2_dilanggar = false;

float wc3 = -440; //-440
float n3 = 0;
float p3 = 0;
bool isn3_dilanggar = false;

float wc4 = -2061; //-2061
float n4 = 0;
float p4 = 0;
bool isn4_dilanggar = false;

float wc5 = -1911; //-1911
float n5 = 0;
float p5 = 0;
bool isn5_dilanggar = false;

float wc6 = -2080; //-1911
float n6 = 0;
float p6 = 0;
bool isn6_dilanggar = false;

float wcA = -446; //-446
float A = 0;
float pA = 0;

float wcB = -447; //-447
float B = 0;
float pB = 0;

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

            //init p values 
            if( (p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) ){
                p1 = n1;
                p2 = n2;
                p3 = n3;
                p4 = n4;
                p5 = n5;
                p6 = n6;
            }
        }else{
            isGameReady = false;
        }
      break;
    //PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
    case 1:
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
        detect_if_biji_dimasukkan();
        break;
    //PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
    case 3:
        detect_if_tembak_diambil();
        break;
    //GANTI GILIRAN
    case 4:
        ganti_giliran();
        break;
    //PERMAINAN SELESAI
    case 5:
        permainan_selesai();
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
        if( (n1<=0.1) && (n6<=0.1) && ((pA + (p1+p6) <= A+0.7) || (pA + (p1+p6) >= A-0.7)) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==2 ){
        if( (n2<=0.1) && (n5<=0.1) && ((pA + (p2+p5) <= A+0.7) || (pA + (p2+p5) >= A-0.7)) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==3 ){
        if( (n3<=0.1) && (n4<=0.1) && ((pA + (p3+p4) <= A+0.7) || (pA + (p3+p4) >= A-0.7)) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==4 ){
        if( (n4<=0.1) && (n3<=0.1) && ((pB + (p3+p4) <= B+0.7) || (pB + (p3+p4) >= B-0.7)) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==5 ){
        if( (n5<=0.1) && (n2<=0.1) && ((pB + (p2+p5) <= B+0.7) || (pB + (p2+p5) >= B-0.7)) ){
            GAME_STATE = 4;
        }
    }else if( lumbung_sekarang==6 ){
        if( (n6<=0.1) && (n1<=0.1) && ((pB + (p1+p6) <= B+0.7) || (pB + (p1+p6) >= B-0.7)) ){
            GAME_STATE = 4;
        }
    }
}

void ganti_giliran(){
    if(giliran_pemain == 1){
        giliran_pemain == 2;
    }else if(giliran_pemain == 2){
        giliran_pemain == 1;
    }
    if( (n1<=0.4) && (n2<=0.4) && (n3<=0.4) && (n4<=0.4) && (n5<=0.4) && (n6<=0.4) ){
        //permainan selesai karena biji pada lumbung sudah habis
        GAME_STATE = 5;
    }else{
        GAME_STATE = 1;
    }
}

void detect_lumbung_dipilih(){
  if(isGameReady){
    //Mendeteksi lumbung yang dipilih pemain
    if( (n1<=0.1) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_sekarang = 1;
        biji_ditangan = p1;
        p1 = n1;
        GAME_STATE = 2;
    }else if( (n2<=0.1) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_sekarang = 2;
        biji_ditangan = p2;
        p2 = n2;
        GAME_STATE = 2;
    }else if( (n3<=0.1) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_sekarang = 3;
        biji_ditangan = p3;
        p3 = n3;
        GAME_STATE = 2;
    }else if( (n4<=0.1) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_sekarang = 4;
        biji_ditangan = p4;
        p4 = n4;
        GAME_STATE = 2;
    }else if( (n5<=0.1) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n6>=GAMEREADY_BATASBAWAH)&&(n6<=GAMEREADY_BATASATAS)) ){
        lumbung_sekarang = 5;
        biji_ditangan = p5;
        p5 = n5;
        GAME_STATE = 2;
    }else if( (n6<=0.1) && ((n1>=GAMEREADY_BATASBAWAH)&&(n1<=GAMEREADY_BATASATAS)) && ((n2>=GAMEREADY_BATASBAWAH)&&(n2<=GAMEREADY_BATASATAS)) && ((n3>=GAMEREADY_BATASBAWAH)&&(n3<=GAMEREADY_BATASATAS)) && ((n4>=GAMEREADY_BATASBAWAH)&&(n4<=GAMEREADY_BATASATAS)) && ((n5>=GAMEREADY_BATASBAWAH)&&(n5<=GAMEREADY_BATASATAS)) ){
        lumbung_sekarang = 6;
        biji_ditangan = p6;
        p6 = n6;
        GAME_STATE = 2;
    }
  }
}

void detect_if_biji_dimasukkan(){
    //deteksi jika 1 biji dimasukkan ke lumbung selanjutnya
    if(biji_ditangan < 0.9){
        biji_ditangan = 0;
        GAME_STATE = 1;
    }else{
        if( ((p1 <= n1) || (n1 < p1+2.6)) && (lumbung_sekarang == 6) && giliran_pemain==1 ){
            if( (giliran_pemain==1) && (p1<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 3;
            }else if( (p1<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 1;
            p1 = n1;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p2 <= n2) || (n2 < p2+2.6)) && (lumbung_sekarang == 1) ){
            if( (giliran_pemain==1) && (p2<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 3;
            }else if( (p2<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 2;
            p2 = n2;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p3 <= n3) || (n3 < p3+2.6)) && (lumbung_sekarang == 2) ){
            if( (giliran_pemain==1) && (p3<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 3;
            }else if( (p3<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 3;
            p3 = n3;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p4 <= n4) || (n4 < p4+2.6)) && (lumbung_sekarang == 3) && giliran_pemain==2){
            if( (giliran_pemain==2) && (p4<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 3;
            }else if( (p4<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 4;
            p4 = n4;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p5 <= n5) || (n5 < p5+2.6)) && (lumbung_sekarang == 4) ){
            if( (giliran_pemain==2) && (p5<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 3;
            }else if( (p5<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 5;
            p5 = n5;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p6 <= n6) || (n6 < p6+2.6)) && (lumbung_sekarang == 5) ){
            if( (giliran_pemain==2) && (p6<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 3;
            }else if( (p6<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 6;
            p6 = n6;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((pA <= A) || (A < pA+2.6)) && (lumbung_sekarang == 3) && giliran_pemain==1 ){
            lumbung_sekarang = 7;
            pA = A;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((pB <= B) || (B < pB+2.6)) && (lumbung_sekarang == 6) && giliran_pemain==2 ){
            lumbung_sekarang = 8;
            pB = B;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p4 <= n4) || (n4 < p4+2.6)) && (lumbung_sekarang == 7) && giliran_pemain==1 ){
            if( (giliran_pemain==1) && (p4<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 4;
            p4 = n4;
            biji_ditangan = biji_ditangan - 1;
        }else if( ((p1 <= n1) || (n1 < p1+2.6)) && (lumbung_sekarang == 8) && giliran_pemain==2 ){
            if( (giliran_pemain==2) && (p1<=0.8) && (biji_ditangan<=1.4) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 1;
            p1 = n1;
            biji_ditangan = biji_ditangan - 1;
        }
    }
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



