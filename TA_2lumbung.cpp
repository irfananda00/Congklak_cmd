#include <HX711.h>
#include <math.h> 

float GAMEREADY_BATAS = 3;

float AVG_BIJI = 5;

int lumbung_sekarang = 0;
int lumbung_lanjut = 0;

int giliran_pemain = 0;

int biji_ditangan = 0;

int GAME_STATE = 0;
// 0 = PERSIAPAN PERMAINAN, memasukkan biji ke lumbung 3 x 6
// 1 = PEMAIN MEMILIH LUMBUNG, pemain mengambil semua biji dari 1 lumbung
// 2 = PEMAIN MEMASUKKAN BIJI KE LUMBUNG, pemain memasukkan biji di tangan ke lumbung selanjutnya searah jarum jam
// 3 = PEMAIN MENEMBAK LUMBUNG LAWAN, pemain mengambil biji di lumbungnya dan yang ditembak dipindahkan ke rumahnya
// 4 = GANTI GILIRAN
// 5 = PERMAINAN SELESAI

HX711 cell4(22, 23);//D A
HX711 cell3(24, 25);//D B
HX711 cell5(27, 26);//D 1
HX711 cell2(28, 29);//N 2
HX711 cell1(30, 31);//D 3 
HX711 cell6(33, 32);//D 4
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

//1-6 / n1-n6 = lumbung
//7 / A = Rumah pemain 1
//8 / B = Rumah pemain 2

int wc1 = -2191; //-2191
int n1 = 0;
float r1 = 0;
int p1 = 0;

int wc2 = -2191; //-2191
int n2 = 0;
float r2 = 0;
int p2 = 0;

int wc3 = -438; //-438
int n3 = 0;
float r3 = 0;
int p3 = 0;

int wc4 = -1991; //-1991
int n4 = 0;
float r4 = 0;
int p4 = 0;

int wc5 = -2059; //-2059
int n5 = 0;
float r5 = 0;
int p5 = 0;

int wc6 = -2125; //-2125
int n6 = 0;
float r6 = 0;
int p6 = 0;

int wcA = -440; //-440
int A = 0;
float rA = 0;
int pA = 0;

int wcB = -441; //-443
int B = 0;
float rB = 0;
int pB = 0;

void loop() {
  // put your main code here, to run repeatedly:

  detect_lumbung();  

  printValue();

  congklak_game();

  delay(200);
}

void congklak_game(){
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
        if( (lumbung_sekarang == 7 && giliran_pemain == 1 && (n1!=0 || n3!=0)) || (lumbung_sekarang == 8 && giliran_pemain == 2 && (n4!=0 || n5!=0 || n6!=0)) || (lumbung_sekarang == 0 && giliran_pemain == 1) || (lumbung_sekarang == 0 && giliran_pemain == 2) ){
            if(giliran_pemain==1){
                if(n1!=0){
                    Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 1 -------------");
                }
                if(n2!=0){
                    Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 2 -------------");
                }
                if(n3!=0){
                    Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 3 -------------");
                }
            }else{
                if(n4!=0){
                    Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 4 -------------");
                }
                if(n5!=0){
                    Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 5 -------------");
                }
                if(n6!=0){
                    Serial.println("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : 6 -------------");
                }
            }
            //TODO: tampilkan yang boleh dipilih lumbung 1 2 3
            detect_lumbung_dipilih_dari_rumah();    
        }else{
            //TODO: tampilkan yang boleh dipilih lumbung lumbung_sekarang
            Serial.print("---------- SILAHKAN AMBIL BIJI DARI LUMBUNG : ");
            Serial.print(lumbung_sekarang);
            Serial.println(" -------------");
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

void detect_lumbung(){
    detectA();
    detectB();
    detect6();
    detect5();
    detect4();
    detect3();
    detect2();
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

void show_next_lumbung(){
    if( (lumbung_sekarang == 6) && (giliran_pemain == 1) ){
        lumbung_lanjut = 1;
    }else if( (lumbung_sekarang == 6) && (giliran_pemain == 2) ){
        lumbung_lanjut = 8;
    }else if( (lumbung_sekarang == 3) && (giliran_pemain == 1) ){
        lumbung_lanjut = 7;
    }else if( (lumbung_sekarang == 3) && (giliran_pemain == 2) ){
        lumbung_lanjut = 4;
    }else if( lumbung_sekarang == 7 ){
        lumbung_lanjut = 4;
    }else if( lumbung_sekarang == 8 ){
        lumbung_lanjut = 1;
    }else{
        lumbung_lanjut = lumbung_sekarang + 1;
    }
    Serial.print("---------- MASUKKAN BIJI KE LUMBUNG : ");
    Serial.print(lumbung_lanjut);
    Serial.println(" -------------");
}

void detect_if_tembak_diambil(){
    if( lumbung_sekarang==1 ){
        Serial.println("---------- AMBIL BIJI PADA LUMBUNG 1 dan 6 -------------");
        if( (n1==0) && (n6==0) ){
            Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
            if ( (pA + (p1+p6) == A) ){
                p1 = n1;
                p6 = n6;
                pA = A;
                GAME_STATE = 4;
            }
        }
    }else if( lumbung_sekarang==2 ){
        Serial.println("---------- AMBIL BIJI PADA LUMBUNG 2 dan 5 -------------");
        if( (n5==0) && (n2==0) ){
            Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
            if( (pA + (p2+p5) == A) ){
                p2 = n2;
                p5 = n5;
                pA = A;
                GAME_STATE = 4;
            }
        }
    }else if( lumbung_sekarang==3 ){
        Serial.println("---------- AMBIL BIJI PADA LUMBUNG 3 dan 4 -------------");
        if( (n3==0) && (n4==0) ){
            Serial.println("---------- MASUKKAN BIJI KE RUMAH A -------------");
            if( (pA + (p3+p4) == A) ){
                p3 = n3;
                p4 = n4;
                pA = A;
                GAME_STATE = 4;
            }
        }
    }else if( lumbung_sekarang==4 ){
        Serial.println("---------- AMBIL BIJI PADA LUMBUNG 4 dan 3 -------------");
        if( (n4==0) && (n3==0) ){
            Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
            if( (pB + (p3+p4) == B) ){
                p3 = n3;
                p4 = n4;
                pB = B;
                GAME_STATE = 4;
            }
        }
    }else if( lumbung_sekarang==5 ){
        Serial.println("---------- AMBIL BIJI PADA LUMBUNG 5 dan 2 -------------");
        if( (n5==0) && (n2==0) ){
            Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
            if( (pB + (p2+p5) == B) ){
                p2 = n2;
                p5 = n5;
                pB = B;
                GAME_STATE = 4;
            }
        }
    }else if( lumbung_sekarang==6 ){
        Serial.println("---------- AMBIL BIJI PADA LUMBUNG 6 dan 1 -------------");
        if( (n6==0) && (n1==0)){
            Serial.println("---------- MASUKKAN BIJI KE RUMAH B -------------");
            if( (pB + (p1+p6) == B) ){
                p6 = n6;
                p1 = n1;
                pB = B;
                GAME_STATE = 4;
            }
        }
    }else if( ( (lumbung_sekarang==1) && ((n1==0) || (n6==0)) ) || ( (lumbung_sekarang==2) && ((n2==0) || (n5==0)) ) || ( (lumbung_sekarang==3) && ((n3==0) || (n4==0)) ) || ( (lumbung_sekarang==4) && ((n4==0) || (n3==0)) ) || ( (lumbung_sekarang==5) && ((n2==0) || (n5==0)) ) || ( (lumbung_sekarang==6) && ((n1==0) || (n6==0)) ) ){
        check_aturan();
    }
}

void ganti_giliran(){
    if(giliran_pemain == 1){
        giliran_pemain = 2;
        lumbung_sekarang = 0;
        Serial.println("---------- GILIRAN PEMAIN 2 !!! -------------");
    }else if(giliran_pemain == 2){
        giliran_pemain = 1;
        lumbung_sekarang = 0;
        Serial.println("---------- GILIRAN PEMAIN 1 !!! -------------");
    }
    check_game_selesai();
}

void check_game_selesai(){
    if( (n1==0) && (n2==0) && (n3==0) && (n4==0) && (n5==0) && (n6==0) ){
        //permainan selesai karena biji pada lumbung sudah habis
        GAME_STATE = 5;
    }else{
        GAME_STATE = 1;
    }
}

void detect_lumbung_dipilih_dari_rumah(){
    //Mendeteksi lumbung yang dipilih pemain 1 = 1 2 3 || pemain 2 = 4 5 6 
    if( (p1!=0) && (n1==0) && (n2==p2) && (n3==p3) && (n4==p4) && (n5==p5) && (n6==p6) && ((lumbung_sekarang==7) || (lumbung_sekarang==0)) && (giliran_pemain==1) ){
        lumbung_sekarang = 1;
        biji_ditangan = p1;
        p1 = n1;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 1 DIPILIH !!! -------------");
    }else if( (p2!=0) && (n2==0) && (n1==p1) && (n3==p3) && (n4==p4) && (n5==p5) && (n6==p6) && ((lumbung_sekarang==7) || (lumbung_sekarang==0)) && (giliran_pemain==1) ){
        lumbung_sekarang = 2;
        biji_ditangan = p2;
        p2 = n2;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 2 DIPILIH !!! -------------");
    }else if( (p3!=0) && (n3==0) && (n1==p1) && (n2==p2) && (n4==p4) && (n5==p5) && (n6==p6) && ((lumbung_sekarang==7) || (lumbung_sekarang==0)) && (giliran_pemain==1) ){
        lumbung_sekarang = 3;
        biji_ditangan = p3;
        p3 = n3;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    }else if( (p4!=0) && (n4==0) && (n1==p1) && (n2==p2) && (n3==p3) && (n5==p5) && (n6==p6) && ((lumbung_sekarang==8) || (lumbung_sekarang==0)) && (giliran_pemain==2) ){
        lumbung_sekarang = 4;
        biji_ditangan = p4;
        p4 = n4;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    }else if( (p5!=0) && (n5==0) && (n1==p1) && (n2==p2) && (n3==p3) && (n4==p4) && (n6==p6) && ((lumbung_sekarang==8) || (lumbung_sekarang==0)) && (giliran_pemain==2) ){
        lumbung_sekarang = 5;
        biji_ditangan = p5;
        p5 = n5;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    }else if( (p6!=0) && (n6==0) && (n1==p1) && (n2==p2) && (n3==p3) && (n4==p4) && (n5==p5) && ((lumbung_sekarang==8) || (lumbung_sekarang==0)) && (giliran_pemain==2) ){
        lumbung_sekarang = 6;
        biji_ditangan = p6;
        p6 = n6;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 6 DIPILIH !!! -------------");
    }else{
        check_aturan();
    }
}

void detect_lumbung_dipilih(){
    //Mendeteksi lumbung yang dipilih pemain
    if( (p1!=0) && (n1==0) && (n2==p2) && (n3==p3) && (n4==p4) && (n5==p5) && (n6==p6) && (lumbung_sekarang==1) ){
        lumbung_sekarang = 1;
        biji_ditangan = p1;
        p1 = n1;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 1 DIPILIH !!! -------------");
    }else if( (p2!=0) && (n2==0) && (n1==p1) && (n3==p3) && (n4==p4) && (n5==p5) && (n6==p6) && (lumbung_sekarang==2) ){
        lumbung_sekarang = 2;
        biji_ditangan = p2;
        p2 = n2;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 2 DIPILIH !!! -------------");
    }else if( (p3!=0) && (n3==0) && (n1==p1) && (n2==p2) && (n4==p4) && (n5==p5) && (n6==p6) && (lumbung_sekarang==3) ){
        lumbung_sekarang = 3;
        biji_ditangan = p3;
        p3 = n3;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 3 DIPILIH !!! -------------");
    }else if( (p4!=0) && (n4==0) && (n1==p1) && (n2==p2) && (n3==p3) && (n5==p5) && (n6==p6) && (lumbung_sekarang==4) ){
        lumbung_sekarang = 4;
        biji_ditangan = p4;
        p4 = n4;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 4 DIPILIH !!! -------------");
    }else if( (p5!=0) && (n5==0) && (n1==p1) && (n2==p2) && (n3==p3) && (n4==p4) && (n6==p6) && (lumbung_sekarang==5) ){
        lumbung_sekarang = 5;
        biji_ditangan = p5;
        p5 = n5;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 5 DIPILIH !!! -------------");
    }else if( (p6!=0) && (n6==0) && (n1==p1) && (n2==p2) && (n3==p3) && (n4==p4) && (n5==p5) && (lumbung_sekarang==6) ){
        lumbung_sekarang = 6;
        biji_ditangan = p6;
        p6 = n6;
        GAME_STATE = 2;
        Serial.println("---------- LUMBUNG 6 DIPILIH !!! -------------");
    }else{
        check_aturan();
    }
}

void check_aturan(){
    if( n1<p1 ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 1 KURANG BIJI !!! -------------");
    }else if( n1==p1 ){
        //TODO: lampu mati
    }else if( n1>p1 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 1 LEBIH BIJI !!! -------------");
    }
    if( n2<p2 ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 2 KURANG BIJI !!! -------------");
    }else if( n2==p2 ){
        //TODO: lampu mati
    }else if( n2>p2 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 2 LEBIH BIJI !!! -------------");
    }
    if( n3<p3 ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
    }else if( n3==p3 ){
        //TODO: lampu mati
    }else if( n3>p3 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
    }
    if( n4<p4 ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
    }else if( n4==p4 ){
        //TODO: lampu mati
    }else if( n4>p4 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
    }
    if( n5<p5 ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
    }else if( n5==p5 ){
        //TODO: lampu mati
    }else if( n5>p5 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
    }
    if( n6<p6 ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 6 KURANG BIJI !!! -------------");
    }else if( n6==p6 ){
        //TODO: lampu mati
    }else if( n6>p6 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 6 LEBIH BIJI !!! -------------");
    }
    if( A<pA ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG A KURANG BIJI !!! -------------");
    }else if( A==pA ){
        //TODO: lampu mati
    }else if( A>pA ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
    }
    if( B<pB ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG B KURANG BIJI !!! -------------");
    }else if( B==pB ){
        //TODO: lampu mati
    }else if( B>pB ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
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
        }else if( (p2+1 == n2) && (lumbung_sekarang == 1) ){
            if( (giliran_pemain==1) && (p2==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p2==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 2;
            p2 = n2;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 2 !!! -------------");
        }else if( (p3+1 == n3) && (lumbung_sekarang == 2) ){
            if( (giliran_pemain==1) && (p3==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p3==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 3;
            p3 = n3;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 3 !!! -------------");
        }else if( (p4+1 == n4) && (lumbung_sekarang == 3) && giliran_pemain==2 ){
            if( (giliran_pemain==2) && (p4==0) && (biji_ditangan==1) ){
                GAME_STATE = 3;
            }else if( (p4==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 4;
            p4 = n4;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 4 !!! -------------");
        }else if( (p5+1 == n5) && (lumbung_sekarang == 4) ){
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
        }else if( (pA+1 == A) && (lumbung_sekarang == 3) && (giliran_pemain==1) ){
            lumbung_sekarang = 7;
            pA = A;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = A !!! -------------");
        }else if( (pB+1 == B) && (lumbung_sekarang == 6) && (giliran_pemain==2) ){
            lumbung_sekarang = 8;
            pB = B;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = B !!! -------------");
        }else if( (p4+1 == n4) && (lumbung_sekarang == 7) && (giliran_pemain==1) ){
            if( (giliran_pemain==1) && (p4==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 4;
            p4 = n4;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 4 !!! -------------");
        }else if( (p1+1 == n1) && (lumbung_sekarang == 8) && (giliran_pemain==2) ){
            if( (giliran_pemain==2) && (p1==0) && (biji_ditangan==1) ){
                GAME_STATE = 4;
            }
            lumbung_sekarang = 1;
            p1 = n1;
            biji_ditangan = biji_ditangan - 1;
            Serial.println("---------- LUMBUNG SEKARANG = 1 !!! -------------");
        }else{
            check_aturan();
        }
    }
}

bool isSudahSiapMain(){
    if( n1<GAMEREADY_BATAS ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 1 KURANG BIJI !!! -------------");
    }else if( n1==GAMEREADY_BATAS ){
        //TODO: lampu mati
    }else if( n1>GAMEREADY_BATAS ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 1 LEBIH BIJI !!! -------------");
    }
    else if( n2<GAMEREADY_BATAS ){
        //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 2 KURANG BIJI !!! -------------");
    }else if( n2==GAMEREADY_BATAS ){
        //TODO: lampu mati
    }else if( n2>GAMEREADY_BATAS ){
        //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 2 LEBIH BIJI !!! -------------");
    }
    if( n3<GAMEREADY_BATAS ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 3 KURANG BIJI !!! -------------");
    }else if( n3==GAMEREADY_BATAS ){
        //TODO: lampu mati
    }else if( n3>GAMEREADY_BATAS ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 3 LEBIH BIJI !!! -------------");
    }
    if( n4<GAMEREADY_BATAS ){
        //TODO: lampu kuning (bijinya kurang)
    Serial.println("---------- LUMBUNG 4 KURANG BIJI !!! -------------");
    }else if( n4==GAMEREADY_BATAS ){
        //TODO: lampu mati
    }else if( n4>GAMEREADY_BATAS ){
        //TODO: lampu merah (bijinya lebih)
    Serial.println("---------- LUMBUNG 4 LEBIH BIJI !!! -------------");
    }
    if( n5<GAMEREADY_BATAS ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 5 KURANG BIJI !!! -------------");
    }else if( n5==GAMEREADY_BATAS ){
        //TODO: lampu mati
    }else if( n5>GAMEREADY_BATAS ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 5 LEBIH BIJI !!! -------------");
    }
    if( n6<GAMEREADY_BATAS ){
        //TODO: lampu kuning (bijinya kurang)
        Serial.println("---------- LUMBUNG 6 KURANG BIJI !!! -------------");
    }else if( n6==GAMEREADY_BATAS ){
        //TODO: lampu mati
    }else if( n6>GAMEREADY_BATAS ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG 6 LEBIH BIJI !!! -------------");
    }
    if( A>0 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG A LEBIH BIJI !!! -------------");
    }
    if( B>0 ){
        //TODO: lampu merah (bijinya lebih)
        Serial.println("---------- LUMBUNG B LEBIH BIJI !!! -------------");
    }else if( (n1==GAMEREADY_BATAS) && (n3==GAMEREADY_BATAS) && (n5==GAMEREADY_BATAS) && (n6==GAMEREADY_BATAS) && (A==0) && (B==0) ){
        return true;
    }else{
        return false;
    }
}

void printValue(){
  Serial.print( "Biji ditangan: " );
  Serial.print(biji_ditangan);
  Serial.print( " # Lumbung sekarang: " );
  Serial.print(lumbung_sekarang);
  Serial.print( " # giliran: " );
  Serial.print(giliran_pemain);
  Serial.print( " # state: " );
  Serial.println(GAME_STATE);
  
  Serial.print( " r1: " );
  Serial.print(r1);
  Serial.print( " # r2: " );
  Serial.print(r2);
  Serial.print( " # r3: " );
  Serial.print(r3);
  Serial.print( " # rA: " );
  Serial.print(rA);
  Serial.print( "  ## " );
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
  Serial.print( "  ## " );
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
  Serial.print( "  ## " );
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
  B = round(rB)/AVG_BIJI;
}

void detectA() {
  cell4.set_scale(wcA);

  rA = cell4.get_units(), 10;  
  if (rA < 0) {
    rA = 0.00;
  }
  A = round(rA)/AVG_BIJI;
}

void detect6() {
  cell8.set_scale(wc6);

  r6 = cell8.get_units(), 10;
  if (r6 < 0) {
    r6 = 0.00;
  }
  n6 = round(r6)/AVG_BIJI;
}

void detect5() {
  cell7.set_scale(wc5);

  r5 = cell7.get_units(), 10;
  if (r5 < 0) {
    r5 = 0.00;
  }
  n5 = round(r5)/AVG_BIJI;
}

void detect4() {
  cell6.set_scale(wc4);

  r4 = cell6.get_units(), 10;  
  if (r4 < 0) {
    r4 = 0.00;
  }
  n4 = round(r4)/AVG_BIJI;
}

void detect3() {
  cell1.set_scale(wc3);

  r3 = cell1.get_units(), 10;
  if (r3 < 0) {
    r3 = 0.00;
  }
  n3 = round(r3)/AVG_BIJI;
}

void detect2() {
  cell2.set_scale(wc2);

  r2 = cell2.get_units(), 10;
  if (r2 < 0) {
    r2 = 0.00;
  }
  n2 = round(r2)/AVG_BIJI;
}

void detect1() {
  cell5.set_scale(wc1);

  r1 = cell5.get_units(), 10;
  if (r1 < 0) {
    r1 = 0.00;
  }
  n1 = round(r1)/AVG_BIJI;
}

//  Serial.print(n1);
//  Serial.print(" grams"); 
//  Serial.print(" calibration_factor: ");
//  Serial.print(wc1);
//  Serial.println();



