#ifndef PARAMETERPATIENT_H // Penjaga header - mencegah file diinclude berkali-kali
#define PARAMETERPATIENT_H // Mendefinisikan simbol untuk penjaga header

// Library yang dibutuhkan:
#include <stdio.h>   // Untuk input/output seperti printf
#include <string.h>  // Untuk manipulasi string
#include <stdlib.h>  // Untuk alokasi memori dll
#include <ctype.h>   // Untuk fungsi karakter (seperti mengubah ke huruf kecil)

// Batas-batas penyimpanan data:
#define MAX_PATIENTS 100      // Banyaknya pasien maksimal yang bisa disimpan
#define MAX_NAME_LEN 50       // Panjang maksimal nama pasien
#define MAX_GENDER_LEN 10     // Panjang maksimal jenis kelamin
#define MAX_DATE_LEN 20       // Panjang maksimal tanggal
#define MAX_SMOKER_LEN 5      // Panjang maksimal status merokok

// Format penyimpanan data pasien: 
typedef struct {
    int id;             // Nomor ID pasien
    char name[MAX_NAME_LEN]; // Nama lengkap pasien
    int age;            // Usia pasien (dalam tahun)
    char gender[MAX_GENDER_LEN]; // Jenis kelamin ("pria"/"wanita")
    float height;       // dalam meter
    float weight;       // dalam kg
    float systolic;     // tekanan darah (mmHg)
    float diastolic;    // tekanan darah (mmHg)
    float temperature;  // dalam °C
    int heartRate;      // denyut jantung (bpm)
    float glucose;      // kadar glukosa (mmol/L)
    float cholesterol;  // kadar kolesterol (mmol/L)
    char smokerStatus[MAX_SMOKER_LEN]; // Status merokok ("ya" atau "tidak")
    char date[MAX_DATE_LEN]; // anggal pemeriksaan  (format: DD/MM/YYYY)
} Patient;

// Variabel global
// Tempat penyimpanan data pasien: 
Patient patients[MAX_PATIENTS];// Array untuk menyimpan semua data pasien
int patientCount = 0; // Menghitung berapa pasein sudah terdaftar

// Fungsi untuk menambah pasien 
// Return: 1 jika berhasil, 0 jika gagal 
int addPatient(const Patient* p) {
    if (patientCount >= MAX_PATIENTS) {
        printf("[ERROR] Kapasitas pasien penuh!\n");
        return 0; // Memberi tahu kalau gagal
    }
    
    patients[patientCount] = *p; // Menyimpan data pasien
    patientCount++; // Tambah jumlah pasien terdaftar
    return 1;
}

// Menghitung BMI
// input: berat (kg), tinggi (meter)
// Output: nilai BMI atau -1 jika salah input
float calculateBMI(float weight, float height) {
    if (height <= 0 || weight <= 0) { // Cek data masuk akal
        printf("[ERROR] Tinggi/berat tidak valid\n");
        return -1.0f; // Beri kode error 
    }
    return weight / (height * height); // Rumus BMI
}

// Memberi nilai berdasarkan BMI
// Input: nilai BMI
// Output: 0 (normal), 1 (kurang/lebih), 2 (obesitas), -1 (error)
int scoreBMI(float bmi) {
    if (bmi < 0) return -1; // Cek error
    if (bmi >= 18.5f && bmi <= 24.9f) return 0;    // Normal
    if ((bmi >= 25.0f && bmi <= 29.9f) || bmi < 18.5f) return 1; // Over/Under
    return 2;  // Obesitas
}

// Memberi nilai berdasarkan gula darah
// Input: kadar gula (mmol/L)
// Output: 0 (normal), 1 (pra-diabetes), 2 (diabetes), -1 (error)
int scoreGlucose(float glucose) {
    if (glucose < 0) return -1; // Cek error
    if (glucose < 7.8f) return 0;    // Normal
    if (glucose < 11.1f) return 1;   // Prediabetes
    return 2;  // Diabetes
}

//Memberi nilai berdasarkan tekanan darah
// Input: tekanan atas, tekanan bawah (mmHg)
// Output: 0 (normal), 1 (pra-hipertensi), 2 (hipertensi), -1 (error)
int scoreBP(float systolic, float diastolic) {
    if (systolic <= 0 || diastolic <= 0) return -1;
    if (systolic < 120.0f && diastolic < 80.0f) return 0;  // Normal
    if (systolic < 140.0f && diastolic < 90.0f) return 1;  // Prehipertensi
    return 2;  // Hipertensi
}

// Memberi nilai berdasarkan kolesterol
// Input: kadar kolesterol (mmol/L)
// Output: 0 (normal), 1 (batas tinggi), 2 (tinggi), -1 (error)
int scoreChol(float cholesterol) {
    if (cholesterol < 0) return -1;
    if (cholesterol < 5.2f) return 0;    // Normal
    if (cholesterol < 6.2f) return 1;    // Batas tinggi
    return 2;  // Tinggi
}

// Memberi nilai berdasarkan status merokok
// Input: "ya" atau "tidak" (case insensitive)
// Output: 0 (tidak merokok), 2 (merokok), -1 (error)
int scoreSmoker(const char* status) {
    if (!status) return -1;
    
    // Ubah ke huruf kecil untuk memudahkan perbandingan
    char lowerStatus[MAX_SMOKER_LEN];
    strncpy(lowerStatus, status, MAX_SMOKER_LEN);
    for (int i = 0; lowerStatus[i]; i++) {
        lowerStatus[i] = tolower(lowerStatus[i]);
    }
    
    if (strcmp(lowerStatus, "tidak") == 0) return 0; // Tidak Merokok
    if (strcmp(lowerStatus, "ya") == 0) return 2; // Merokok
    
    printf("[WARNING] Status merokok tidak valid: %s\n", status);
    return -1; // Input tidak valid
}

// Menentukan level risiko 
// Input: total nilai dari semua penilaian
// Output: "Sehat", "Perlu Perhatian", atau "Risiko Tinggi" */
const char* overallRiskLevel(int totalScore) {
    if (totalScore < 0) return "ERROR: Skor tidak valid"; // Cek error
    if (totalScore <= 2) return "Sehat";
    if (totalScore <= 5) return "Perlu Perhatian";
    return "Risiko Tinggi";
}

// Memeriksa kelengkapan dan kebenaran data pasien
// Input: data pasien
// Output: 1 jika valid, 0 jika tidak valid
int validatePatient(const Patient* p) {
    if (!p) return 0;
    
    // Memeriksa semua data dalam range yang wajar: 
    return (p->id > 0) &&  // ID harus positif
           (p->age > 0 && p->age <= 120) &&  // Usia wajar
           (p->height > 0.5f && p->height < 2.5f) &&  // Tinggi wajar
           (p->weight > 2.0f && p->weight < 300.0f) &&  // Berat wajar
           (p->systolic > 50.0f && p->systolic < 250.0f) &&  // Tekanan systolic wajar
           (p->diastolic > 30.0f && p->diastolic < 150.0f) && // Tekanan diastolic wajar
           (p->temperature > 30.0f && p->temperature < 45.0f) &&  // Suhu wajar
           (p->heartRate > 30 && p->heartRate < 200) &&  // Denyut jantung wajar
           (p->glucose > 0 && p->glucose < 50.0f) &&  // Gula darah wajar
           (p->cholesterol > 0 && p->cholesterol < 20.0f) &&  // Kolesterol wajar
           (scoreSmoker(p->smokerStatus) != -1);  // Status merokok valid
}

#endif // Akhir dari penjaga header