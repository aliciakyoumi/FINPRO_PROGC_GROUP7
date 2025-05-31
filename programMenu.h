//Header guard untuk mencegah pendefinisian ganda file header ini
#ifndef PROGRAMMENU_H
#define PROGRAMMENU_H

#include <stdio.h> //library untuk standar input/output
#include <string.h> //library untuk manipulasi string
#include <stdlib.h> //library untuk alokasi memori dinamis dan fungsi exit
#include <ctype.h> //library untuk fungsi-fungsi karakter (misalnya isdigit, toupper)
#include "parameterPatient.h"  //Mengimpor header yang berisi struct Patient dan fungsi terkait


void lifestyleRecommendation(int score) { //Fungsi rekomendasi gaya hidup berdasarkan skor kesehatan
    printf("\nRekomendasi Gaya Hidup:\n"); //judul rekomendasi
    if (score <= 2) { //Jika skor sehat
        printf("- Pertahankan pola hidup sehat.\n"); //Rekomendasi jika sehat untuk mempertahankan kesehatan
    } else if (score <= 5) { //Jika skor perlu perhatian
        printf("- Kurangi konsumsi gula dan garam.\n"); //Rekomendasi jika perlu perhatian untuk mengurangi gula dan garam
        printf("- Rutin olahraga ringan.\n"); //Rekomendasi jika perlu perhatian untuk berolahraga secara teratur
    } else { //Jika skor risiko tinggi
        printf("- Konsultasi rutin dengan dokter.\n"); //Rekomendasi jika skor tinggi untuk berkonsultasi dengan dokter
        printf("- Perubahan gaya hidup menyeluruh diperlukan.\n"); //Rekomendasi untuk melakukan perubahan gaya hidup
    }
}

void asciiDiagram(int score) { //Fungsi menampilkan diagram risiko kesehatan
    printf("\nDiagram Risiko (0-6):\n"); //judul diagram risiko
    printf("[ "); //Membuka tampilan diagram
    for (int i = 0; i < 7; i++) { //Mengulangi untuk setiap nilai dari 0 hingga 6
        if (i == score) //Jika indeks sama dengan skor
            printf("*"); //Bintang untuk menunjukkan skor
        else
            printf("-"); //Tanda minus untuk nilai lainnya
    }
    printf(" ]\n"); //Menutup tampilan diagram
}

//Fungsi menginput data pasien
void inputPatient(struct Patient* p) {
    printf("ID Pasien               : "); //Input ID pasien
    scanf("%d", &p->id); //Menyimpan ID pasien ke dalam struct

    printf("Nama                    : "); //Input nama pasien
    scanf(" %[^\n]", p->name); //Menyimpan nama pasien ke dalam struct

    //Mengubah format huruf pada nama
    for (int i = 0; p->name[i]; i++) { //Mengulangi setiap karakter dalam nama
        if (i == 0 || p->name[i - 1] == ' ') { //Jika karakter pertama atau setelah spasi
            p->name[i] = toupper(p->name[i]); //Mengubah menjadi huruf upper case
        } else {//lainnya
            p->name[i] = tolower(p->name[i]); //Mengubah menjadi huruf lower case
        }
    }

    printf("Usia (tahun)            : "); //Input usia pasien
    scanf("%d", &p->age); //Menyimpan usia pasien ke dalam struct

    printf("Jenis Kelamin (pria/wanita): "); //Input jenis kelamin pasien
    scanf("%s", p->gender); // Menyimpan jenis kelamin ke dalam struct

    //Mengubah format huruf pada gender
    for (int i = 0; p->gender[i]; i++) { //Mengulangi setiap karakter dalam gender
        p->gender[i] = tolower(p->gender[i]); //Mengubah menjadi huruf lower case
    }

    printf("Tinggi (meter)          : "); //Input tinggi badan pasien
    scanf("%f", &p->height); //Menyimpan tinggi badan ke dalam struct

    printf("Berat Badan (kg)        : "); //Input berat badan pasien
    scanf("%f", &p->weight); //Menyimpan berat badan ke dalam struct

    printf("Tekanan Darah Sistolik (mmHg): "); //Input tekanan darah sistolik
    scanf("%f", &p->systolic); //Menyimpan tekanan darah sistolik ke dalam struct

    printf("Tekanan Darah Diastolik (mmHg): "); //Input tekanan darah diastolik
    scanf("%f", &p->diastolic); //Menyimpan tekanan darah diastolik ke dalam struct

    printf("Suhu Tubuh (°C)         : "); //Input suhu tubuh pasien
    scanf("%f", &p->temperature); //Menyimpan suhu tubuh ke dalam struct

    printf("Denyut Jantung (bpm)    : "); //Input denyut jantung pasien
    scanf("%d", &p->heartRate); //Menyimpan denyut jantung ke dalam struct

    printf("Kadar Glukosa (mmol/L)  : "); // Input kadar glukosa pasien
    scanf("%f", &p->glucose); //Menyimpan kadar glukosa ke dalam struct

    printf("Kolesterol (mmol/L)     : "); //Input kadar kolesterol pasien
    scanf("%f", &p->cholesterol); //Menyimpan kadar kolesterol ke dalam struct

    printf("Merokok? (ya/tidak)     : "); //Input status merokok pasien
    scanf("%s", p->smokerStatus); //Menyimpan status merokok ke dalam struct

    //Mengubah format huruf pada smoker status
    for (int i = 0; p->smokerStatus[i]; i++) { //Mengulangi setiap karakter dalam smokerStatus
        p->smokerStatus[i] = tolower(p->smokerStatus[i]); //Mengubah menjadi huruf lower case
    }

    printf("Tanggal (DD/MM/YYYY)    : "); //Input tanggal pasien
    scanf("%s", p->date); //Menyimpan tanggal ke dalam struct
}

//Fungsi mengevaluasi status kesehatan pasien
void evaluatePatient(struct Patient* p) {
    float bmi = calculateBMI(p->weight, p->height); //Menghitung BMI pasien
    int score = 0; //Inisialisasi skor

    //Menambahkan skor berdasarkan evaluasi kesehatan
    score += scoreBMI(bmi, p->gender);
    score += scoreGlucose(p->glucose);
    score += scoreBP(p->systolic, p->diastolic);
    score += scoreChol(p->cholesterol);
    score += scoreSmoker(p->smokerStatus);

    const char* status = overallRiskLevel(score); //Menentukan status kesehatan berdasarkan skor

    //Status kesehatan pasien
    printf("\nStatus Kesehatan Pasien %s (ID: %d): %s\n", p->name, p->id, status);
    asciiDiagram(score); //Diagram risiko
    lifestyleRecommendation(score); //Rekomendasi gaya hidup
}

//Fungsi menyimpan data pasien ke dalam file
void saveToFile() {
    //Memeriksa izin pengguna
    if (strcmp(currentRole, "Admin") != 0 && strcmp(currentRole, "Dokter") != 0) {
        printf("Anda tidak memiliki izin untuk menyimpan data.\n");
        return; //Keluar dari fungsi jika tidak memiliki izin
    }
    
    FILE* file = fopen("laporan_pasien.txt", "w"); //Membuka file untuk menulis
    if (!file) { //Periksa file 
        printf("Gagal membuka file output!\n");
        return; //Keluar dari fungsi jika gagal membuka file
    }

    //Mengulangi untuk setiap pasien yang ada
    for (int i = 0; i < n; i++) {
        struct Patient* p = &patients[i]; //Mengambil data pasien
        float bmi = calculateBMI(p->weight, p->height); //Menghitung BMI pasien
        int score = 0; //Inisialisasi skor

        //Menambahkan skor berdasarkan evaluasi kesehatan
        score += scoreBMI(bmi, p->gender);
        score += scoreGlucose(p->glucose);
        score += scoreBP(p->systolic, p->diastolic);
        score += scoreChol(p->cholesterol);
        score += scoreSmoker(p->smokerStatus);
        const char* status = overallRiskLevel(score); //Menentukan status kesehatan berdasarkan skor
        
        //Menyimpan laporan evaluasi pasien ke dalam file
        fprintf(file, "\n--- Laporan Evaluasi Pasien %s (ID: %d) ---\n", p->name, p->id);
        fprintf(file, "Usia               : %d tahun\n", p->age);
        fprintf(file, "Jenis Kelamin      : %s\n", p->gender);
        fprintf(file, "BMI                : %.2f\n", bmi);
        fprintf(file, "Glukosa            : %.2f mmol/L\n", p->glucose);
        fprintf(file, "Tekanan Darah      : %.0f/%.0f mmHg\n", p->systolic, p->diastolic);
        fprintf(file, "Kolesterol         : %.2f mmol/L\n", p->cholesterol);
        fprintf(file, "Status Merokok     : %s\n", p->smokerStatus);
        fprintf(file, "Tanggal            : %s\n", p->date);
        fprintf(file, "Status Risiko      : %s\n", status);
    }

    fclose(file); //Menutup file setelah selesai menulis
    printf("\nData berhasil disimpan ke 'laporan_pasien.txt'\n"); //Pesan sukses
}

//Fungsi mengekspor data pasien ke dalam format CSV
void exportToCSV() {
    //Memeriksa izin pengguna
    if (strcmp(currentRole, "Admin") != 0 && strcmp(currentRole, "Dokter") != 0) {
        printf("Anda tidak memiliki izin untuk mengekspor data.\n");
        return; //Keluar dari fungsi jika tidak memiliki izin
    }

    FILE* file = fopen("laporan_pasien.csv", "w"); //File CSV 
    if (!file) { //Memeriksa file 
        printf("Gagal membuat file CSV!\n");
        return; //Keluar dari fungsi jika gagal membuka file
    }

    //Header kolom untuk file CSV
    fprintf(file, "ID,Nama,Usia,Jenis Kelamin,Tinggi,Berat,BMI,Glukosa,Kolesterol,TD Sistolik,TD Diastolik,Merokok,Tanggal,Status\n");
    
    //Mengulangi untuk setiap pasien yang ada
    for (int i = 0; i < n; i++) {
        float bmi = calculateBMI(patients[i].weight, patients[i].height); //Menghitung BMI pasien
        int score = 0; //Inisialisasi skor

        //Menambahkan skor berdasarkan evaluasi kesehatan
        score += scoreBMI(bmi, patients[i].gender);
        score += scoreGlucose(patients[i].glucose);
        score += scoreBP(patients[i].systolic, patients[i].diastolic);
        score += scoreChol(patients[i].cholesterol);
        score += scoreSmoker(patients[i].smokerStatus);
        const char* status = overallRiskLevel(score); //Menentukan status kesehatan berdasarkan skor
        
        //Menyimpan data pasien ke dalam file CSV
        fprintf(file, "%d,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.0f,%.0f,%s,%s,%s\n",
            patients[i].id, patients[i].name, patients[i].age, patients[i].gender,
            patients[i].height, patients[i].weight, bmi, patients[i].glucose,
            patients[i].cholesterol, patients[i].systolic, patients[i].diastolic,
            patients[i].smokerStatus, patients[i].date, status);
    }

    fclose(file); //Menutup file
    printf("Data berhasil diekspor ke 'laporan_pasien.csv'\n"); //Menampilkan pesan sukses
}

#endif //Penutup header guard
