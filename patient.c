#include "patient.h"     // Definisi struktur data pasien
#include "utils.h"       // Fungsi-fungsi utilitas pendukung (misalnya, clearInputBuffer)
#include <string.h>     // Untuk operasi string (misalnya, strcmp, strlen)
#include <ctype.h>      // Untuk fungsi manipulasi karakter (misalnya, toupper, tolower)
#include <stdio.h>      // Untuk fungsi input/output standar (misalnya, printf, scanf)

// Fungsi untuk menghitung Indeks Massa Tubuh (BMI)
float calculateBMI(float weight, float height) {
    if (height <= 0) return 0.0f; // Jika tinggi tidak valid (<= 0), kembalikan BMI 0
    return weight / (height * height); // Rumus standar BMI: berat / (tinggi * tinggi)
}

// Fungsi untuk menentukan skor berdasarkan nilai BMI
int scoreBMI(float bmi, const char* gender) {
    (void)gender; // Parameter gender saat ini tidak digunakan, ditandai untuk menghindari peringatan kompilator
    if (bmi >= 18.5 && bmi <= 24.9) return 0; // BMI ideal, skor 0
    else if ((bmi >= 25 && bmi <= 29.9) || (bmi < 18.5)) return 1; // BMI lebih atau kurang dari ideal, skor 1
    else if (bmi >= 30) return 2; // Kondisi obesitas, skor 2
    return -1; // Nilai default jika tidak ada kategori yang cocok (seharusnya tidak tercapai dengan input valid)
}

// Fungsi untuk menentukan skor berdasarkan kadar glukosa
int scoreGlucose(float glucose) {
    if (glucose < 7.8) return 0; // Kadar glukosa normal, skor 0
    else if (glucose < 11.1) return 1; // Kadar glukosa pra-diabetes, skor 1
    else return 2; // Kadar glukosa diabetes, skor 2
}

// Fungsi untuk menentukan skor berdasarkan tekanan darah
int scoreBP(float systolic, float diastolic) {
    if (systolic < 130 && diastolic < 80) return 0; // Tekanan darah normal, skor 0
    else if ((systolic < 160) || (diastolic < 100)) return 1; // Tekanan darah prahipertensi/hipertensi tahap 1, skor 1
    else return 2; // Tekanan darah hipertensi tahap 2 atau lebih, skor 2
}

// Fungsi untuk menentukan skor berdasarkan kadar kolesterol
int scoreChol(float cholesterol) {
    if (cholesterol < 5.0) return 0; // Kadar kolesterol normal, skor 0
    else if (cholesterol < 7.5) return 1; // Kadar kolesterol batas tinggi, skor 1
    else return 2; // Kadar kolesterol tinggi, skor 2
}

// Fungsi untuk menentukan skor berdasarkan status merokok pasien
int scoreSmoker(const char* status) {
    if (strcmp(status, "tidak") == 0) return 0; // Jika tidak merokok, skor 0
    else return 2; // Jika merokok ("ya"), skor 2
}

// Fungsi untuk menghitung total skor risiko kesehatan seorang pasien
int calculateTotalRiskScore(const struct Patient* p) {
    if (!p) return -1; // Jika pointer pasien null, kembalikan -1 sebagai indikasi error
    float bmi = calculateBMI(p->weight, p->height); // Hitung nilai BMI pasien
    int totalScore = 0; // Inisialisasi total skor dengan 0
    totalScore += scoreBMI(bmi, p->gender); // Akumulasi skor dari BMI
    totalScore += scoreGlucose(p->glucose); // Akumulasi skor dari glukosa
    totalScore += scoreBP(p->systolic, p->diastolic); // Akumulasi skor dari tekanan darah
    totalScore += scoreChol(p->cholesterol); // Akumulasi skor dari kolesterol
    totalScore += scoreSmoker(p->smokerStatus); // Akumulasi skor dari status merokok
    return totalScore; // Kembalikan total skor risiko yang terhitung
}

// Fungsi untuk menentukan level risiko keseluruhan berdasarkan total skor
const char* overallRiskLevel(int totalScore) {
    if (totalScore <= 2) return "Sehat"; // Skor rendah mengindikasikan level "Sehat"
    else if (totalScore <= 5) return "Perlu Perhatian"; // Skor sedang mengindikasikan "Perlu Perhatian"
    else return "Risiko Tinggi"; // Skor tinggi mengindikasikan "Risiko Tinggi"
}

// Fungsi untuk menampilkan rekomendasi gaya hidup sesuai skor risiko
void lifestyleRecommendation(int score) {
    printf("\nRekomendasi Gaya Hidup:\n"); // Judul bagian rekomendasi
    if (score <= 2) { // Untuk skor rendah
        printf("- Pertahankan pola hidup sehat.\n"); // Rekomendasi bagi yang sudah sehat
    } else if (score <= 5) { // Untuk skor sedang
        printf("- Kurangi konsumsi gula dan garam.\n"); // Saran pertama untuk skor sedang
        printf("- Rutin olahraga ringan.\n"); // Saran kedua untuk skor sedang
    } else { // Untuk skor tinggi
        printf("- Konsultasi rutin dengan dokter.\n"); // Saran pertama untuk skor tinggi
        printf("- Perubahan gaya hidup menyeluruh diperlukan.\n"); // Saran kedua untuk skor tinggi
    }
}

// Fungsi untuk menampilkan diagram risiko sederhana berbasis teks (ASCII)
void asciiDiagram(int score) {
    printf("\nDiagram Risiko (0-6):\n"); // Judul untuk diagram
    printf("[ "); // Awalan representasi diagram
    for (int i = 0; i <= 6; i++) { // Iterasi untuk membuat bar diagram (skala 0-6)
        if (i == (score > 6 ? 6 : score)) // Jika iterasi 'i' sama dengan nilai skor (dibatasi hingga 6)
            printf("*"); // Tandai posisi skor dengan karakter '*'
        else
            printf("-"); // Bagian lain dari bar ditandai dengan '-'
    }
    printf(" ]\n"); // Akhiran representasi diagram
}

// Fungsi untuk menerima input data pasien dengan validasi setiap field
void inputPatient(struct Patient* p) {
    // Input nama: validasi panjang, tidak kosong, format kapitalisasi.
    do {
        printf("Nama                         : "); // Meminta input nama pasien
        scanf(" %49[^\n]", p->name); // Membaca input nama (maks. 49 karakter, mengabaikan spasi awal, hingga newline)
        clearInputBuffer(); // Membersihkan sisa karakter dalam buffer input

        if (strlen(p->name) == 0 || strlen(p->name) > 49) { // Validasi: apakah nama kosong atau melebihi panjang maksimal?
            printf("Nama tidak boleh kosong atau lebih dari 49 karakter. Silakan coba lagi.\n"); // Pesan kesalahan panjang nama
            continue; // Melanjutkan iterasi untuk meminta input nama kembali
        }

        // Proses ini bertujuan untuk format nama: huruf pertama tiap kata kapital, sisanya huruf kecil.
        for (int i = 0; p->name[i]; i++) { // Iterasi untuk setiap karakter dalam nama
            if (i == 0 || p->name[i - 1] == ' ') { // Jika karakter pertama atau setelah spasi
                p->name[i] = toupper(p->name[i]); // Mengubah karakter menjadi kapital
            } else { // Untuk karakter lainnya
                p->name[i] = tolower(p->name[i]); // Mengubah karakter menjadi huruf kecil
            }
        }
        break; // Jika valid, keluar dari loop input nama
    } while (1); // Loop akan berlanjut hingga input nama valid

    // Input usia: validasi harus numerik dan positif.
    do {
        printf("Usia (tahun)             : "); // Meminta input usia pasien
        if (scanf("%d", &p->age) != 1) { // Membaca input usia dan memeriksa apakah input adalah numerik
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan jika input bukan angka
            clearInputBuffer(); // Membersihkan buffer dari input yang salah
            p->age = -1; // Menetapkan nilai usia tidak valid untuk mengulang loop
        } else if (p->age < 0) { // Memeriksa apakah usia bernilai negatif
            printf("Usia tidak boleh negatif. Silakan masukkan usia yang valid.\n"); // Pesan kesalahan untuk usia negatif
            // Nilai p->age yang negatif akan menyebabkan loop berlanjut
        } else { // Jika input usia valid (numerik dan tidak negatif)
            clearInputBuffer(); // Membersihkan buffer setelah input valid
            break; // Keluar dari loop input usia
        }
    } while (p->age < 0); // Loop berlanjut jika usia masih negatif (atau input awal non-numerik)

    // Input jenis kelamin: validasi harus "pria" atau "wanita".
    do {
        printf("Jenis Kelamin (pria/wanita): "); // Meminta input jenis kelamin
        scanf("%9s", p->gender); // Membaca input jenis kelamin (maks. 9 karakter)
        clearInputBuffer(); // Membersihkan buffer

        for (int i = 0; p->gender[i]; i++) { // Iterasi pada setiap karakter input
            p->gender[i] = tolower(p->gender[i]); // Mengonversi semua karakter ke huruf kecil untuk konsistensi perbandingan
        }

        if (strcmp(p->gender, "pria") != 0 && strcmp(p->gender, "wanita") != 0) { // Memeriksa validitas input
            printf("Jenis kelamin tidak valid. Silakan masukkan 'pria' atau 'wanita'.\n"); // Pesan kesalahan
        }
    } while (strcmp(p->gender, "pria") != 0 && strcmp(p->gender, "wanita") != 0); // Loop hingga input valid

    // Input tinggi: validasi numerik float, rentang 0.4 - 3.0 meter.
    do {
        printf("Tinggi (meter)             : "); // Meminta input tinggi pasien
        if (scanf("%f", &p->height) != 1) { // Memeriksa apakah input adalah float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan jika bukan float
            clearInputBuffer(); // Membersihkan buffer
            p->height = -1.0f; // Menetapkan nilai tinggi tidak valid
        } else if (p->height < 0.4f || p->height > 3.0f) { // Memeriksa apakah tinggi berada dalam rentang yang valid
            printf("Tinggi harus antara 0.4 dan 3.0 meter. Silakan coba lagi.\n"); // Pesan mengenai rentang nilai yang valid
            p->height = -1.0f; // Menetapkan nilai tinggi tidak valid
        } else { // Jika input tinggi valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->height < 0.4f || p->height > 3.0f); // Loop jika tinggi di luar rentang (atau input non-numerik)

    // Input berat badan: validasi numerik float, rentang 5.0 - 500.0 kg.
    do {
        printf("Berat Badan (kg)           : "); // Meminta input berat badan
        if (scanf("%f", &p->weight) != 1) { // Memeriksa apakah input adalah float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->weight = -1.0f; // Menetapkan nilai berat tidak valid
        } else if (p->weight < 5.0f || p->weight > 500.0f) { // Memeriksa rentang berat badan
            printf("Berat badan harus antara 5.0 dan 500.0 kg. Silakan coba lagi.\n"); // Pesan rentang valid
            p->weight = -1.0f; // Menetapkan nilai berat tidak valid
        } else { // Jika input berat valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->weight < 5.0f || p->weight > 500.0f); // Loop jika berat di luar rentang

    // Input tekanan darah sistolik: validasi numerik float, rentang 50 - 250 mmHg.
    do {
        printf("Tekanan Darah Sistolik (mmHg): "); // Meminta input sistolik
        if (scanf("%f", &p->systolic) != 1) { // Cek tipe data float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->systolic = -1.0f; // Nilai tidak valid
        } else if (p->systolic < 50 || p->systolic > 250) { // Cek rentang valid
            printf("Tekanan darah sistolik harus antara 50 dan 250 mmHg. Silakan coba lagi.\n"); // Pesan rentang
            p->systolic = -1.0f; // Nilai tidak valid
        } else { // Jika valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->systolic < 50 || p->systolic > 250); // Loop jika di luar rentang

    // Input tekanan darah diastolik: validasi numerik float, rentang 30 - 150 mmHg.
    do {
        printf("Tekanan Darah Diastolik (mmHg): "); // Meminta input diastolik
        if (scanf("%f", &p->diastolic) != 1) { // Cek tipe data float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->diastolic = -1.0f; // Nilai tidak valid
        } else if (p->diastolic < 30 || p->diastolic > 150) { // Cek rentang valid
            printf("Tekanan darah diastolik harus antara 30 dan 150 mmHg. Silakan coba lagi.\n"); // Pesan rentang
            p->diastolic = -1.0f; // Nilai tidak valid
        } else { // Jika valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->diastolic < 30 || p->diastolic > 150); // Loop jika di luar rentang
    
    // Input suhu tubuh: validasi numerik float, rentang 35.0 - 40.0 Celsius.
    do {
        printf("Suhu Tubuh (Celsius)       : "); // Meminta input suhu
        if (scanf("%f", &p->temperature) != 1) { // Cek tipe data float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->temperature = -1.0f; // Nilai tidak valid
        } else if (p->temperature < 35.0f || p->temperature > 40.0f) { // Cek rentang valid
            printf("Suhu tubuh harus antara 35.0 dan 40.0 derajat Celsius. Silakan coba lagi.\n"); // Pesan rentang
            p->temperature = -1.0f; // Nilai tidak valid
        } else { // Jika valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->temperature < 35.0f || p->temperature > 40.0f); // Loop jika di luar rentang

    // Input denyut jantung: validasi numerik integer, rentang 30 - 200 bpm.
    do {
        printf("Denyut Jantung (bpm)       : "); // Meminta input denyut jantung
        if (scanf("%d", &p->heartRate) != 1) { // Cek tipe data integer
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->heartRate = -1; // Nilai tidak valid
        } else if (p->heartRate < 30 || p->heartRate > 200) { // Cek rentang valid
            printf("Denyut jantung harus antara 30 dan 200 bpm. Silakan coba lagi.\n"); // Pesan rentang
            p->heartRate = -1; // Nilai tidak valid
        } else { // Jika valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->heartRate < 30 || p->heartRate > 200); // Loop jika di luar rentang

    // Input glukosa: validasi numerik float, rentang 6.0 - 15.0 mmol/L.
    do {
        printf("Glukosa (mmol/L)           : "); // Meminta input glukosa
        if (scanf("%f", &p->glucose) != 1) { // Cek tipe data float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->glucose = -1.0f; // Nilai tidak valid
        } else if (p->glucose < 6.0f || p->glucose > 15.0f) { // Cek rentang valid
            printf("Glukosa harus antara 6.0 dan 15.0 mmol/L. Silakan coba lagi.\n"); // Pesan rentang
            p->glucose = -1.0f; // Nilai tidak valid
        } else { // Jika valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->glucose < 6.0f || p->glucose > 15.0f); // Loop jika di luar rentang

    // Input kolesterol: validasi numerik float, rentang 3.0 - 10.0 mmol/L.
    do {
        printf("Kolesterol (mmol/L)        : "); // Meminta input kolesterol
        if (scanf("%f", &p->cholesterol) != 1) { // Cek tipe data float
            printf("Input tidak valid. Silakan masukkan angka.\n"); // Pesan kesalahan
            clearInputBuffer(); // Membersihkan buffer
            p->cholesterol = -1.0f; // Nilai tidak valid
        } else if (p->cholesterol < 3.0f || p->cholesterol > 10.0f) { // Cek rentang valid
            printf("Kolesterol harus antara 3.0 dan 10.0 mmol/L. Silakan coba lagi.\n"); // Pesan rentang
            p->cholesterol = -1.0f; // Nilai tidak valid
        } else { // Jika valid
            clearInputBuffer(); // Membersihkan buffer
            break; // Keluar loop
        }
    } while (p->cholesterol < 3.0f || p->cholesterol > 10.0f); // Loop jika di luar rentang

    // Input status merokok: validasi harus "ya" atau "tidak".
    do {
        printf("Status Merokok (ya/tidak)  : "); // Meminta input status merokok
        scanf(" %5s", p->smokerStatus); // Membaca input (maks. 5 karakter), mengabaikan spasi awal
        clearInputBuffer(); // Membersihkan buffer

        for (int i = 0; p->smokerStatus[i]; i++) { // Iterasi pada karakter input
            p->smokerStatus[i] = tolower(p->smokerStatus[i]); // Konversi ke huruf kecil
        }

        if (strcmp(p->smokerStatus, "ya") != 0 && strcmp(p->smokerStatus, "tidak") != 0) { // Cek validitas
            printf("Status merokok tidak valid. Silakan masukkan 'ya' atau 'tidak'.\n"); // Pesan kesalahan
        }
    } while (strcmp(p->smokerStatus, "ya") != 0 && strcmp(p->smokerStatus, "tidak") != 0); // Loop hingga valid

    // Input tanggal: validasi format DD/MM/YYYY dan validitas logis dasar.
    do {
        printf("Tanggal (DD/MM/YYYY)       : "); // Meminta input tanggal
        scanf(" %10s", p->date); // Membaca input (maks. 10 karakter), mengabaikan spasi awal
        clearInputBuffer(); // Membersihkan buffer

        if (strlen(p->date) != 10 || p->date[2] != '/' || p->date[5] != '/') { // Memeriksa format dasar tanggal
            printf("Format tanggal tidak valid. Silakan masukkan dalam format DD/MM/YYYY.\n"); // Pesan kesalahan format
        } else {
            int day, month, year; // Variabel untuk komponen tanggal
            sscanf(p->date, "%d/%d/%d", &day, &month, &year); // Mem-parsing string tanggal

            if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) { // Memeriksa validitas logis tanggal
                printf("Tanggal tidak valid. Silakan coba lagi.\n"); // Pesan kesalahan tanggal
            } else { // Jika format dan nilai logis valid
                break; // Keluar loop
            }
        }
    } while (1); // Loop hingga input tanggal valid
}

// Fungsi untuk mengevaluasi kondisi kesehatan pasien dan menampilkan hasilnya
void evaluatePatient(const struct Patient* p) {
    float bmi = calculateBMI(p->weight, p->height); // Menghitung BMI pasien
    int score = 0; // Inisialisasi skor total risiko
    score += scoreBMI(bmi, p->gender); // Menambahkan skor berdasarkan BMI
    score += scoreGlucose(p->glucose); // Menambahkan skor berdasarkan glukosa
    score += scoreBP(p->systolic, p->diastolic); // Menambahkan skor berdasarkan tekanan darah
    score += scoreChol(p->cholesterol); // Menambahkan skor berdasarkan kolesterol
    score += scoreSmoker(p->smokerStatus); // Menambahkan skor berdasarkan status merokok

    const char* status = overallRiskLevel(score); // Menentukan level risiko keseluruhan

    printf("\nStatus Kesehatan Pasien %s (ID: %d): %s\n", p->name, p->id, status); // Menampilkan status kesehatan pasien
    asciiDiagram(score); // Menampilkan diagram risiko
    lifestyleRecommendation(score); // Menampilkan rekomendasi gaya hidup
}