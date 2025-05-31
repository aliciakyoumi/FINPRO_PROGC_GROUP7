#include <stdio.h>    //import library c standar
#include <string.h>   //import library modifikasi string
#include <stdlib.h>   //import library untuk fungsi utilitas umum
#include <ctype.h>    //import library untuk fungsi karakter
#include "patient.h"  //import header file patient
#include "database.h" //import header file database
#include "utils.h"    //import header file utils

#define ACTION_ADD_PATIENT      "add_patient"               //definisi aksi menambah pasien
#define ACTION_DISPLAY_PATIENTS "display_patients"          //definisi aksi tampilan pasien
#define ACTION_DELETE_PATIENT   "delete_patient"            //definisi aksi hapus pasien
#define ACTION_UPDATE_PATIENT   "update_patient"            //definisi aksi update pasien
#define ACTION_SAVE_FILE        "save_file"                 //definisi aksi simpan ke file .txt
#define ACTION_SAVE_CSV         "save_csv"                  //definisi aksi simpan ke file .csv
#define ACTION_FIND_PATIENT     "find_patient"              //definisi aksi cari pasien
#define ACTION_VIEW_PATIENT_STATUS "view_patient_status"    //definisi aksi ngeliat status pasien
#define ACTION_EXPORT_ONE_PATIENT "export_one_patient"      //definisi aksi simpan satu pasien
#define ACTION_NEW_DATABASE     "new_database"              //definisi aksi membuat database baru

static int checkPermission(const char* role, const char* action) {      //memeriksa izin berdasarkan role
    if (strcmp(action, ACTION_DISPLAY_PATIENTS) == 0||
        strcmp(action, ACTION_VIEW_PATIENT_STATUS) == 0 ||
        strcmp(action, ACTION_EXPORT_ONE_PATIENT) == 0 ||
        strcmp(action, ACTION_FIND_PATIENT) == 0) {
        //admin, dokter, dan perawat yang memiliki izin
        if (strcmp(role, "admin") == 0 || strcmp(role, "dokter") == 0 || strcmp(role, "perawat") == 0) {
            return 1;       //izin diberikan 
        }
    }
    if (strcmp(action, ACTION_ADD_PATIENT) == 0 ||
        strcmp(action, ACTION_SAVE_FILE) == 0 ||
        strcmp(action, ACTION_NEW_DATABASE) == 0||
        strcmp(action, ACTION_SAVE_CSV) == 0) {
        //hanya admin dan dokter yang memiliki izin
        if (strcmp(role, "admin") == 0 || strcmp(role, "dokter") == 0) {
            return 1;       //izin diberikan 
        }
    }
    if (strcmp(action, ACTION_DELETE_PATIENT) == 0 ||
        strcmp(action, ACTION_UPDATE_PATIENT) == 0) {
        //hanya admin yang memiliki izin
        if (strcmp(role, "admin") == 0) {
            return 1;       //izin diberikan 
        }
    }
    return 0;       //izin ditolak 
}

static void displayMenu(const char* currentRole_param) {        //tampilan menu berdasarkan pilihan role
    printf("==============================================================\n");
    printf(" Menu (User: %s):\n", currentRole_param);           //menunjukkan role usernya apa
    //tampilan opsi menu berdasarkan izin/permission
    if (checkPermission(currentRole_param, ACTION_ADD_PATIENT)) {
        printf(" 1. Tambah Pasien Baru\n");     //opsi tambah pasien
    }
    printf(" 2. Tampilkan Data Pasien\n");      //opsi tampilan pasien
    if (checkPermission(currentRole_param, ACTION_DELETE_PATIENT)) {
        printf(" 3. Hapus Data Pasien\n");      //opsi hapus pasien
    }
    if (checkPermission(currentRole_param, ACTION_UPDATE_PATIENT)) {
        printf(" 4. Perbarui Data Pasien\n");   //opsi update pasien
    }
    if (checkPermission(currentRole_param, ACTION_SAVE_FILE)) {
        printf(" 5. Simpan ke File\n");         //opsi simpan file ke .txt
    }
    if (checkPermission(currentRole_param, ACTION_NEW_DATABASE)) {
        printf(" 6. Expor ke CSV\n");           //opsi simpan file ke .csv
    }
    if (checkPermission(currentRole_param, ACTION_NEW_DATABASE)) {
        printf(" 7. Buat Database Baru\n");     //opsi membuat database baru
    }
    printf(" 8. Cari Pasien\n");                //opsi mencari pasien
    printf(" 9. Simpan Laporan Satu Pasien\n"); //opsi simpan laporan satu pasien
    printf(" 10. Lihat Status Pasien\n");       //opsi ngelihat status pasien
    printf(" 0. Keluar\n");                     //opsi keluar
    printf("==============================================================\n");
    printf("Pilihan Anda: ");                   //opsi minta input pilihan user
}

int main() {
    char adminName[50];     //menyimpan nama admin
    char role[20];          //menyimpan pilihan role
    int choice;             //menyimpan pilihan menu
    int exitProgram = 0;    //keluar dari program

    initDB();               //inisialisasi database

    printf("==============================================================\n");
    printf("VITALS: Vital Information Tracking and Assessment Logic System\n");
    printf("==============================================================\n");

    while (!exitProgram) {                  //loop utama program
        printf("Masukkan nama Anda: ");     //minta input nama user
        scanf(" %[^\n]", adminName);        //membaca nama user

        int validRole = 0;          //periksa validitas role
        while(!validRole){          //looping utk memastikan role yg valid
            printf("Masukkan peran Anda (Admin/Dokter/Perawat): ");     //input role user
            scanf("%s", role);                  //membaca input peran/role user
            while(getchar() != '\n');           //menghapus karakter newline dari input (kl ada)
            for (int i = 0; role[i]; i++) {     //membaca penulisan role disesuaikan dengan huruf kecil
                role[i] = tolower(role[i]);
            }
            //memeriksa apakah rolenya udh valid apa blm
            if (strcmp(role, "admin") == 0 || strcmp(role, "dokter") == 0 || strcmp(role, "perawat") == 0) {
                validRole = 1;      //role valid ketika termasuk ke dalam salah satu dari 3 role yg dikasih
            } else {
                //
                printf("Peran tidak valid. Silakan coba lagi.\n"); //role diluar admin, dokter, perawat
            }
        }
    }
        int sessionEnded = 0;       //mengakhiri sesi
        while(!sessionEnded){       //loop sesi user
            displayMenu(role);      //tampilan menu berdasarkan role yg dipilih

            if (scanf("%d", &choice) != 1) {        //input pilihan user
                printf("Input tidak valid. Masukkan angka 0-10.\n");    //kalo pilihan diluar tampilan menu
                while (getchar() != '\n');          //menghapus karakter newline (sisa)
                continue;                           //kembali ke awal loop
            }
            while(getchar() != '\n');               //menghapus karakter newline

            switch(choice){         //pakai switch case untuk proses pilihan user dari tampilan menu
                case 1: {           //pilihan tambah pasien
                    if(checkPermission(role, ACTION_ADD_PATIENT)){
                        createAndAddPatient();}     //memanggil fungsi untuk tambah pasien
                        else {
                            printf("Anda tidak memiliki izin untuk menambah pasien.\n");
                        }
                        break;}
                case 2: {           //pilihan tampilan pasien
                    displayPatients();              //memanggil fungsi untuk menampilkan pasien
                    break;}
                case 3: {           //pilihan hapus pasien
                    if(checkPermission(role, ACTION_DELETE_PATIENT)){
                        deletePatientData();}       //memanggil fungsi untuk hapus pasien
                    else {
                        printf("Anda tidak memiliki izin untuk menghapus pasien.\n");}
                    break;}
                case 4: {           //pilihan update pasien
                    if(checkPermission(role, ACTION_UPDATE_PATIENT)){
                        updatePatientData();}       //memanggil fungsi untuk update pasien
                    else {
                    printf("Anda tidak memiliki izin untuk memperbarui pasien.\n");}
                    break;}
                case 5: {           //pilihan simpan ke file .txt
                    if(checkPermission(role, ACTION_SAVE_FILE)){
                        saveToFile();}              //memanggil fungsi untuk simpan ke file .txt
                    else {
                        printf("Anda tidak memiliki izin untuk menyimpan ke file.\n");
                    }
                    break;}
                case 6: {           //pilihan simpan ke file .csv 
                    if(checkPermission(role, ACTION_NEW_DATABASE)){
                        exportToCSV();}             //memanggil fungsi untuk simpan ke file .csv
                    else {
                        printf("Anda tidak memiliki izin untuk mengekspor ke CSV.\n");
                    }
                    break;}
                case 7: {           //pilihan membuat database baru 
                    if(checkPermission(role, ACTION_NEW_DATABASE)){
                        newDatabase();}             //memanggil fungsi untuk membuat database baru
                    else {
                        printf("Anda tidak memiliki izin untuk membuat database baru.\n");
                    }
                    break;}
                case 8: {           //pilihan cari pasien
                    if(checkPermission(role, ACTION_FIND_PATIENT)){
                        searchPatientData();}       //memanggil fungsi untuk cari pasien
                    else {
                        printf("Anda tidak memiliki izin untuk mencari pasien.\n");
                    }
                    break;}
                case 9: {           //pilihan simpan laporan satu pasien
                    if(checkPermission(role, ACTION_EXPORT_ONE_PATIENT)){
                        saveSinglePatientData();    //memanggil fungsi untuk simpan laporan satu pasien
                    } else {
                        printf("Anda tidak memiliki izin untuk menyimpan laporan satu pasien.\n");
                    }
                    break;} 
                case 10: {          //pilihan lihat status pasien
                    if(checkPermission(role, ACTION_VIEW_PATIENT_STATUS)){
                        checkPatientStatusData();}  //memanggil fungsi untuk lihat status pasien
                    else {
                        printf("Anda tidak memiliki izin untuk melihat status pasien.\n");
                    }
                    break;
                }
                case 0: {           //pilihan keluar dari menu
                    int subChoice;  //variabel pilihan sub-menu dari pilihan keluar
                    printf("\nApakah Anda ingin:\n");       //pilihan keluar atau ganti role
                    printf("1. Ganti Nama dan Role\n");     //opsi ganti nama dan role
                    printf("2. Keluar Program\n");          //opsi keluar program 
                    printf("Pilihan Anda: ");               //input dari user
                    scanf("%d", &subChoice);                //membaca pilihan sub-menu yg dipilih user

                    //proses pilihan sub-menu
                    if (subChoice == 1) {
                        sessionEnded = 1;           //mengakhiri sesi, bisa ganti role dan nama
                    } else if (subChoice == 2) {
                        printf("Keluar program...\n");
                        printf("==============================================================\n");
                        sessionEnded = 1;           //mengakhiri sesi dan program
                        exitProgram = 1;
                    } else {
                        printf("Pilihan tidak valid! Kembali ke menu utama.\n");    //muncul klo yg dipilih gada di sub-menu
                    }
                    break;}
                default: {
                    printf("Pilihan tidak valid! Pilih angka 1-10!\n");     //muncul klo yg dipilih gada di menu
                }      
            }
        }
    return 0;       //program selesai
}
