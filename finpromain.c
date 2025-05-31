#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "programMenu.h" //import header file menu
#include "parameterPatient.h" //import header file parameter pasien

int main() {
    char adminName[50];     //menyimpan nama admin
    int choice;             //menyimpan pilihan menu
    int exitProgram = 0;    //keluar dari program

    printf("=====================================\n");
    printf("   Sistem Evaluasi Data Pasien\n");
    printf("=====================================\n");

    while (!exitProgram) {                  //loop utama program
        
        printf("Masukkan nama Anda: ");
        scanf(" %[^\n]", adminName);        //membaca nama admin

        printf("Masukkan peran Anda (Admin/Dokter/Perawat): ");
        scanf("%s", currentRole);           //membaca peran/role user
        for (int i = 0; currentRole[i]; i++) {  //membaca penulisan role disesuaikan dengan huruf kecil
            currentRole[i] = tolower(currentRole[i]);
        }

        int sessionEnded = 0;       //mengakhiri sesi
        while (!sessionEnded) {     //loop dalam sesi
            printf("\n=====================================\n");
            printf(" Menu:\n");     //tampilan menu berdasarkan role yg dipilih
            if (strcmp(currentRole, "admin") == 0 || strcmp(currentRole, "dokter") == 0) {
                printf(" 1. Tambah Pasien Baru\n");
            }
            printf(" 2. Tampilkan Data Pasien\n");
            if (strcmp(currentRole, "admin") == 0) {
                printf(" 3. Hapus Data Pasien\n");
                printf(" 4. Perbarui Data Pasien\n");
            }
            if (strcmp(currentRole, "admin") == 0 || strcmp(currentRole, "dokter") == 0) {
                printf(" 5. Simpan ke File\n");
                printf(" 6. Ekspor ke CSV\n");
                printf(" 7. Buat Database Baru\n");
            }
            printf(" 8. Cari Pasien\n");
            printf(" 9. Simpan Laporan Satu Pasien\n");
            printf(" 10. Lihat Status Pasien\n");
            printf(" 0. Keluar\n");
            printf("=====================================\n");
            printf("Pilihan Anda: ");
            scanf("%d", &choice);       //pilihan menu

            switch (choice) {       //case berdasarkan pilihan menu
                case 1:
                    if (n >= MAX) {    //memeriksa database penuh/engga
                        printf("Database penuh! Tidak dapat menambah pasien baru.\n");
                        break;
                    }
                    if (strcmp(currentRole, "admin") == 0 || strcmp(currentRole, "dokter") == 0) {
                        inputPatient(&patients[n]);        //memasukkan data pasien
                        evaluatePatient(&patients[n]);     //mengevaluasi pasien
                        n++;        //menambah jumlah pasien
                    }
                    break;
                case 2:
                    displayPatients();      //menampilkan data pasien
                    break;
                case 3:
                    deletePatient();        //menghapus data pasien
                    break;
                case 4:
                    updatePatient();        //mengupdate data pasien
                    break;
                case 5:
                    saveToFile();           //menyimpan data ke file .txt
                    break;
                case 6:
                    exportToCSV();          //mengekspor data ke file CSV
                    break;
                case 7:
                    newDatabase();          //membuat database baru
                    break;
                case 8:
                    searchPatient();        //mencari pasien
                    break;
                case 9:
                    saveSinglePatient();    //menyimpan laporan satu pasien
                    break;
                case 10:
                    viewPatientStatus();    //melihat status pasien
                    break;
                case 0: {       //pilihan utk exit program
                    int subChoice;      //menyimpan pilihan sub-menu (keluar beneran/ganti role)
                    printf("\nApakah Anda ingin:\n");
                    printf("1. Ganti Nama dan Role\n");
                    printf("2. Keluar Program\n");
                    printf("Pilihan Anda: ");
                    scanf("%d", &subChoice);        //membaca pilihan sub-menu

                    if (subChoice == 1) {
                        sessionEnded = 1;       //ganti nama dan role
                    } else if (subChoice == 2) {
                        printf("Keluar program...\n");
                        sessionEnded = 1;       //mengakhiri sesi
                        exitProgram = 1;        //exit program
                    } else {
                        printf("Pilihan tidak valid! Kembali ke menu utama.\n");    //muncul klo yg dipilih gada di sub-menu
                    }
                    break;
                }
                default:
                    printf("Pilihan tidak valid!\n");       //muncul klo yg dipilih gada di menu
            }
        }
    }

    return 0;       //program selesai
}
