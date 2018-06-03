#include <iostream>
#include <cstdlib>
#include <ctime>
// #include <map>
// #include <iterator>
using namespace std;

// map<int, int> rumah;
int rumah[16] = { };
int lubang_dipilih;
int biji_ditangan;
int giliran_pemain;
bool isPlaying = true;

void next_rumah(){
    int lubang_selanjutnya = lubang_dipilih+1;
    if((lubang_selanjutnya == 8)and(giliran_pemain == 2)){
        lubang_selanjutnya = lubang_selanjutnya+1;
    }
    else if((lubang_selanjutnya == 16)and(giliran_pemain == 1)){
        lubang_selanjutnya = 1;
    }
    else if(lubang_selanjutnya >= 17){
        lubang_selanjutnya = 1;
    }

    int valid;
    cout<<"masukkan biji ke lubang "<< lubang_selanjutnya <<" : ";
    cin >> valid;
    
    biji_ditangan = biji_ditangan - 1;

    rumah[lubang_selanjutnya-1]=rumah[lubang_selanjutnya-1]+1;

    lubang_dipilih = lubang_selanjutnya;
}

void print_biji_rumah(){
    for( int a = 0; a < 16; a = a + 1 ) {
        cout<< a+1 <<" : "<< rumah[a] <<endl;
    }
}

bool isEmpty_allrumah(){
    int count = 0;
    for( int a = 0; a < 7; a = a + 1 ) {
        if(rumah[a]>0){
            count += 1;
        }
    }
    for( int a = 8; a < 15; a = a + 1 ) {
        if(rumah[a]>0){
            count += 1;
        }
    }
    if(count == 0){
        return true;
    }else{
        return false;
    }
}

bool isEmpty_rumah1(){
    int count = 0;
    for( int a = 0; a < 7; a = a + 1 ) {
        if(rumah[a]>0){
            count += 1;
        }
    }
    if(count == 0){
        return true;
    }else{
        return false;
    }
}

bool isEmpty_rumah2(){
    int count = 0;
    for( int a = 8; a < 15; a = a + 1 ) {
        if(rumah[a]>0){
            count += 1;
        }
    }
    if(count == 0){
        return true;
    }else{
        return false;
    }
}

void print_pilih_rumah1(){
    for( int a = 0; a < 7; a = a + 1 ) {
        if(rumah[a]>0){
            cout<< a+1 <<", ";
        }
    }
}

void print_pilih_rumah2(){
    for( int a = 8; a < 15; a = a + 1 ) {
        if(rumah[a]>0){
            cout<< a+1 <<", ";
        }
    }
}

void masukkan_biji(){
    biji_ditangan = rumah[lubang_dipilih-1];
    rumah[lubang_dipilih-1]=0;
    
    while(biji_ditangan!=0){
        next_rumah();
    }
}

int main()
{
    // mulai permainan
    rumah[0]=7;
    rumah[1]=7;
    rumah[2]=7;
    rumah[3]=7;
    rumah[4]=7;
    rumah[5]=7;
    rumah[6]=7;
    rumah[7]=0;

    rumah[8]=7;
    rumah[9]=7;
    rumah[10]=7;
    rumah[11]=7;
    rumah[12]=7;
    rumah[13]=7;
    rumah[14]=7;
    rumah[15]=0;

    print_biji_rumah();

    //giliran pemain
    srand(time(NULL));
    int r = rand() % 100 + 1;
    cout<< "hasil random : " << r <<endl;
    if(r<=50){
        giliran_pemain = 1;
        cout<< "giliran pemain 1" <<endl;
        cout<<"pilih lubang (1-7): ";
        cin >> lubang_dipilih;
        while((lubang_dipilih < 1)or(lubang_dipilih > 7)){
            cout<<"pilih lubang (1-7): ";
            cin >> lubang_dipilih;
        }
    }else{
        giliran_pemain = 2;
        cout<< "giliran pemain 2" <<endl;
        cout<<"pilih lubang (9-15): ";
        cin >> lubang_dipilih;
        while((lubang_dipilih < 9)or(lubang_dipilih > 15)){
            cout<<"pilih lubang (9-15): ";
            cin >> lubang_dipilih;
        }
    }

    while(isPlaying){
        masukkan_biji();

        print_biji_rumah();

        cout<< "lubang sekarang: "<< lubang_dipilih <<endl;

        //if semua rumah bijinya kosong maka permainan berakhir
        if(isEmpty_allrumah()){
            isPlaying = false;
            cout<< "permainan berakhir" <<endl;
            cout<< "Skor pemain 1: "<< rumah[8] <<endl;
            cout<< "Skor pemain 2: "<< rumah[16] <<endl;
        }

        //if masuk rumah besar 8 atau 16 pilih lumbung yg ada bijinya sesuai pemainnya
        else if((lubang_dipilih == 8)and(giliran_pemain == 1)){
            //ganti giliran jika biji di rumah habis
            if(isEmpty_rumah1()){
                giliran_pemain = 2;
                cout<< "giliran pemain 2" <<endl;
                cout<<"pilih lubang ";
                print_pilih_rumah2();
                cout<<" : ";
                cin >> lubang_dipilih;
            }else{
                cout<<"pilih lubang ";
                print_pilih_rumah1();
                cout<<" : ";
                cin >> lubang_dipilih;
            }
        }
        else if((lubang_dipilih == 16)and(giliran_pemain == 2)){
            //ganti giliran jika biji di rumah habis
            if(isEmpty_rumah2()){
                giliran_pemain = 1;
                cout<< "giliran pemain 1" <<endl;
                cout<<"pilih lubang ";
                print_pilih_rumah1();
                cout<<" : ";
                cin >> lubang_dipilih;   
            }else{
                cout<<"pilih lubang ";
                print_pilih_rumah2();
                cout<<" : ";
                cin >> lubang_dipilih;
            }
        }

        // //[OPSIONAL] tukar giliran jika masuk ke lumbung lawan
        // else if((giliran_pemain == 1)and(lubang_dipilih > 8)){
        //     giliran_pemain = 2;
        //     cout<< "giliran pemain 2" <<endl;
        //     cout<<"pilih lubang ";
        //     print_pilih_rumah2();
        //     cout<<" : ";
        //     cin >> lubang_dipilih;
        // }
        // else if((giliran_pemain == 2)and(lubang_dipilih < 9)){
        //     giliran_pemain = 1;
        //     cout<< "giliran pemain 1" <<endl;
        //     cout<<"pilih lubang ";
        //     print_pilih_rumah1();
        //     cout<<" : ";
        //     cin >> lubang_dipilih;
        // }

        //tembak lumbung lawan
        else if((giliran_pemain == 1)and(lubang_dipilih < 8)and(rumah[lubang_dipilih-1] == 1)){
            int total = 0;
            if(lubang_dipilih == 1){
                total = rumah[0]+rumah[14];
                rumah[0] = 0;
                rumah[14] = 0;
                rumah[7] += total;
            }else if(lubang_dipilih == 2){
                total = rumah[1]+rumah[13];
                rumah[1] = 0;
                rumah[13] = 0;
                rumah[7] += total;
            }else if(lubang_dipilih == 3){
                total = rumah[2]+rumah[12];
                rumah[2] = 0;
                rumah[12] = 0;
                rumah[7] += total;
            }else if(lubang_dipilih == 4){
                total = rumah[3]+rumah[11];
                rumah[3] = 0;
                rumah[11] = 0;
                rumah[7] += total;
            }else if(lubang_dipilih == 5){
                total = rumah[4]+rumah[10];
                rumah[4] = 0;
                rumah[10] = 0;
                rumah[7] += total;
            }else if(lubang_dipilih == 6){
                total = rumah[5]+rumah[9];
                rumah[5] = 0;
                rumah[9] = 0;
                rumah[7] += total;
            }else if(lubang_dipilih == 7){
                total = rumah[6]+rumah[8];
                rumah[6] = 0;
                rumah[8] = 0;
                rumah[7] += total;
            }
            giliran_pemain = 2;
            cout<< "giliran pemain 2" <<endl;
            cout<<"pilih lubang ";
            print_pilih_rumah2();
            cout<<" : ";
            cin >> lubang_dipilih;
        }
        else if((giliran_pemain == 2)and(lubang_dipilih > 8)and(rumah[lubang_dipilih-1] == 1)){         
            int total = 0;
            if(lubang_dipilih == 15){
                total = rumah[0]+rumah[14];
                rumah[0] = 0;
                rumah[14] = 0;
                rumah[15] += total;
            }else if(lubang_dipilih == 14){
                total = rumah[1]+rumah[13];
                rumah[1] = 0;
                rumah[13] = 0;
                rumah[15] += total;
            }else if(lubang_dipilih == 13){
                total = rumah[2]+rumah[12];
                rumah[2] = 0;
                rumah[12] = 0;
                rumah[15] += total;
            }else if(lubang_dipilih == 12){
                total = rumah[3]+rumah[11];
                rumah[3] = 0;
                rumah[11] = 0;
                rumah[15] += total;
            }else if(lubang_dipilih == 11){
                total = rumah[4]+rumah[10];
                rumah[4] = 0;
                rumah[10] = 0;
                rumah[15] += total;
            }else if(lubang_dipilih == 10){
                total = rumah[5]+rumah[9];
                rumah[5] = 0;
                rumah[9] = 0;
                rumah[15] += total;
            }else if(lubang_dipilih == 9){
                total = rumah[6]+rumah[8];
                rumah[6] = 0;
                rumah[8] = 0;
                rumah[15] += total;
            }

            giliran_pemain = 1;
            cout<< "giliran pemain 1" <<endl;
            cout<<"pilih lubang ";
            print_pilih_rumah1();
            cout<<" : ";
            cin >> lubang_dipilih;  
        }
    }
}