#include "database.h"                   // Header untuk fungsi database
#include "patient.h"                    // Header untuk definisi dan fungsi pasien
#include <stdio.h>                       // Library standar untuk fungsi input/output
#include <string.h>                      // Library fungsi string
#include <stdlib.h>                      // Library fungsi utilitas 
#include <ctype.h>                       // Library fungsi karakter

struct Patient patients[MAX];           // Array untuk menyimpan data pasien
int patientCount;                       // Jumlah pasien yang terdaftar
int nextID;                            // ID berikutnya yang akan digunakan untuk pasien baru (Integer)

/*
Cara kerja fungsi loadData:
- Fungsi ini memuat data pasien dari file CSV yang diberikan.
- Jika file tidak ditemukan, fungsi akan menginformasikan dan memulai dengan database kosong.
 */
void loadData(const char* filename) {
    FILE* file = fopen(filename, "r");  // fopen --> membuka file untuk dibaca
    if (!file) {
        printf("Info: File '%s' tidak ditemukan. Memulai dengan database kosong.\n", filename);
        return;
    }
    char lineBuffer[512];                // Buffer untuk menyimpan setiap baris dari file
    int maxID = 0;                       // Variabel untuk menyimpan ID maksimum yang ditemukan
    if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) { // Membaca header
        printf("Info: File '%s' kosong atau header tidak bisa dibaca.\n", filename);
        fclose(file); // fclose --> menutup file
        return;
    }

    printf("Memuat data dari '%s'...\n", filename); //Mencetak Informasi
    while (fgets(lineBuffer, sizeof(lineBuffer), file) != NULL && patientCount < MAX) {
        lineBuffer[strcspn(lineBuffer, "\n")] = 0; // Menghapus newline dari akhir baris

        struct Patient temp_p;              // Variabel sementara untuk menyimpan data pasien
        char temp_name[sizeof(temp_p.name)]; // Buffer untuk nama pasien
        char temp_gender[sizeof(temp_p.gender)]; // Buffer untuk jenis kelamin
        char temp_smoker[sizeof(temp_p.smokerStatus)]; // Buffer untuk status merokok
        char temp_date[sizeof(temp_p.date)]; // Buffer untuk tanggal

        float bmi_csv;                       // Variabel untuk menyimpan nilai BMI dari CSV
        char status_csv[50];                 // Buffer untuk status risiko dari CSV
        
        int itemsScanned = sscanf(lineBuffer, "%d,\"%49[^\"]\",%d,%9[^,],%f,%f,%f,%f,%f,%f,%f,%f,%d,%5[^,],%10[^,],%49[^\n]", 
               &temp_p.id, temp_name, &temp_p.age, temp_gender,
               &temp_p.height, &temp_p.weight, &bmi_csv,
               &temp_p.glucose, &temp_p.cholesterol,
               &temp_p.systolic, &temp_p.diastolic,
               &temp_p.temperature, &temp_p.heartRate,
               temp_smoker, temp_date, status_csv); // Membaca data dari file CSV aebelumnya ke variabel sementara
 
        if (itemsScanned >= 15) {           // Pastikan data CSV sesuai jumlah yang diharapkan, ada >= 15 data per pasien
            strncpy(temp_p.name, temp_name, sizeof(temp_p.name) - 1); // Salin nama pasien ke struct
            temp_p.name[sizeof(temp_p.name) - 1] = '\0';              // Tambahkan null terminator
            strncpy(temp_p.gender, temp_gender, sizeof(temp_p.gender) - 1); // Salin jenis kelamin ke struct
            temp_p.gender[sizeof(temp_p.gender) - 1] = '\0';          // Tambahkan null terminator
            strncpy(temp_p.smokerStatus, temp_smoker, sizeof(temp_p.smokerStatus) - 1); // Salin status merokok ke struct
            temp_p.smokerStatus[sizeof(temp_p.smokerStatus) - 1] = '\0'; // Tambahkan null terminator
            strncpy(temp_p.date, temp_date, sizeof(temp_p.date) - 1);  // Salin tanggal ke struct
            temp_p.date[sizeof(temp_p.date) - 1] = '\0';               // Tambahkan null terminator

            int conflictID = 0;          // Flag untuk deteksi ID yang sudah ada
            for(int i=0; i < patientCount; ++i) { // Loop memeriksa duplikat ID
                if(patients[i].id == temp_p.id) { // Jika ID sudah ada
                    conflictID = 1;      // Tandai sebagai konflik
                    printf("Peringatan: ID Pasien %d dari CSV sudah ada, data dilewati.\n", temp_p.id); // Beri tahu user tentang duplikat
                    break;               // Berhenti cek lebih lanjut
                }
            }

            if (!conflictID) {                // Jika tidak ada konflik
                patients[patientCount++] = temp_p; // Tambahkan pasien ke array
                if (temp_p.id > maxID) {      // Update maxID jika ID lebih besar
                    maxID = temp_p.id;
                }
            }
        } else {
            printf("Peringatan: Format baris CSV tidak sesuai, dilewati: %s (item discan: %d)\n", lineBuffer, itemsScanned); // Beri tahu kesalahan format
        }
    }
    fclose(file);                          // Tutup file setelah selesai

    nextID = maxID + 1;                   // Set ID berikutnya
    printf("Data berhasil dimuat. %d pasien ditambahkan. ID berikutnya akan dimulai dari %d.\n", patientCount, nextID); // Laporkan hasil memuat data
}

/*
Cara kerja fungsi initDB:
- Fungsi ini menginisialisasi database dengan mengatur jumlah pasien ke 0 dan memuat data dari file CSV.
 */
void initDB(void) {
    patientCount = 0;                     // Reset jumlah pasien
    nextID = 1;                           // Set ID berikutnya ke 1
    loadData("laporan_pasien.csv");      // Memuat data dari file CSV
}

/*
Cara kerja fungsi searchPatient:
- Fungsi ini mencari pasien berdasarkan ID yang diberikan.
- Jika pasien ditemukan, fungsi mengembalikan pointer ke data pasien.
 */
struct Patient* searchPatient(int id) {
    for (int i = 0; i < patientCount; i++) { // Loop melalui pasien yang ada
        if (patients[i].id == id) {         // Cek kecocokan ID
            return &patients[i];            // Kembalikan pointer ke data pasien
        }
    }
    return NULL;                           // Jika tidak ditemukan kembalikan NULL
}

/*
Cara kerja fungsi addPatient:
- Fungsi ini menambahkan pasien baru ke database.
- Jika database penuh, fungsi akan mengembalikan -1.
- Jika ID pasien sudah ada, fungsi akan mencari ID baru yang tersedia.
 */
int addPatient(struct Patient* p) {
    if(patientCount >= MAX) {          // Cek bila database penuh
        printf("Database penuh, tidak bisa menambah pasien baru.\n"); // Beri tahu user
        return -1;                    // Kembalikan kode gagal
    }
    p->id = nextID;                   // Tetapkan ID baru untuk pasien
    if(searchPatient(p->id) != NULL) {  // Cek jika ID sudah digunakan
        int temp_id = p->id;            // Mulai dari ID sekarang
        while(searchPatient(temp_id) != NULL) { // Cari ID kosong terdekat
            temp_id++;                  // Tambah ID sementara
        }
        
        p->id = temp_id;                  // Set ID baru
        nextID = temp_id;                 // Update nextID
    }
    
    patients[patientCount++] = *p;       // Tambahkan pasien ke array
    nextID++;                             // Increment nextID
    printf("Pasien %s (ID: %d) berhasil ditambahkan.\n", p->name, p->id);
    return 0;                             // Kode sukses
}

/*
Cara kerja fungsi displayPatients:
- Fungsi ini menampilkan semua data pasien yang ada dalam database.
- Jika tidak ada pasien, fungsi akan memberi tahu pengguna.
 */
void displayPatients(void) {
    if (patientCount == 0) {                                // Cek apakah tidak ada pasien
        printf("Tidak ada data pasien dalam database.\n"); // Info kosong
        return;                                            // Kembalikan tanpa menampilkan pasien 
    }

    printf("\n=== Data Pasien ===\n");      // Header tampilan data pasien
    for (int i = 0; i < patientCount; i++) { // Loop semua pasien
        struct Patient* p = &patients[i]; // Ambil pointer ke pasien

        float bmi = calculateBMI(p->weight, p->height); // Hitung BMI
        int score = calculateTotalRiskScore(p);          // Hitung skor risiko
        const char* status = overallRiskLevel(score);     // Dapatkan status risiko

        // Cetak data pasien 
        printf("\n%d. %s (ID: %d)\n", i + 1, p->name, p->id);
        printf("Usia               : %d tahun\n", p->age);
        printf("Jenis Kelamin      : %s\n", p->gender);
        printf("Tinggi Badan       : %.2f meter\n", p->height);
        printf("Berat Badan        : %.2f kg\n", p->weight);
        printf("BMI                : %.2f\n", bmi);
        printf("Glukosa            : %.2f mmol/L\n", p->glucose);
        printf("Tekanan Darah      : %.0f/%.0f mmHg\n", p->systolic, p->diastolic);
        printf("Kolesterol         : %.2f mmol/L\n", p->cholesterol);
        printf("Status Merokok     : %s\n", p->smokerStatus);
        printf("Status Risiko      : %s\n", status);
    }
}

/*
Cara kerja fungsi deletePatient:
- Fungsi ini menghapus pasien dari database berdasarkan ID yang diberikan.
- Jika pasien ditemukan, data pasien akan dihapus dan fungsi mengembalikan 0.
- Jika tidak ditemukan, fungsi mengembalikan -1.
 */
int deletePatient(int id) {
    int found = -1;                       // Variabel untuk menyimpan indeks pasien yang ditemukan
    for (int i = 0; i < patientCount; i++) { // Loop cek seluruh pasien
        if (patients[i].id == id) { // Jika cocok ID
            found = i;              // Simpan indeksnya jika cocok
            break;                 // Berhenti cari
        }
    }

    if (found != -1) {                    // Jika pasien ditemukan
        for (int j = found; j < patientCount - 1; j++) { // Geser array ke kiri untuk hapus pasien
            patients[j] = patients[j + 1]; // Pindahkan pasien selanjutnya ke indeks sebelumnya
        }
        patientCount--;                    // Kurangi jumlah pasien
        printf("Pasien dengan ID %d berhasil dihapus.\n", id); // Info sukses 
        return 0;                         // Kode sukses
    }
    
    printf("Pasien dengan ID %d tidak ditemukan.\n", id); // Info pasien tidak ditemukan
    return -1;                            // Kode gagal
}

/*
Cara kerja fungsi updatePatient:
- Fungsi ini memperbarui data pasien yang sudah ada berdasarkan ID.
- Jika pasien ditemukan, data pasien akan diperbarui dengan data baru.
 */
int updatePatient(int id, const struct Patient* p) {
    struct Patient* found = searchPatient(id); // Cari pasien berdasarkan ID
    if (!found) {
        printf("Pasien dengan ID %d tidak ditemukan.\n", id);
        return -1;                          // Kode gagal
    }
    *found = *p;                             // Perbarui data pasien
    printf("Data pasien berhasil diperbarui.\n");
    return 0;                                // Kode sukses
}

/*
Cara kerja fungsi saveToFile:
- Fungsi ini menyimpan data pasien ke dalam file teks.
- Jika file tidak dapat dibuka, fungsi akan memberi tahu pengguna.
 */
void saveToFile(void) {
    FILE* file = fopen("laporan_pasien.txt", "w"); // Buka file untuk tulis
    if (!file) {                  // Jika gagal buka file
        printf("Gagal membuka file output!\n"); // Info kegagalan
        return;                  // Kembali tanpa simpan
    }
    for (int i = 0; i < patientCount; i++) { // Loop semua pasien
        struct Patient* p = &patients[i];   // Pointer ke pasien
        float bmi = calculateBMI(p->weight, p->height); // Hitung BMI pasien
        int score = calculateTotalRiskScore(p);          // Hitung skor risiko
        const char* status = overallRiskLevel(score);     // Status risiko
        
        // Tulis laporan pasien ke file
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
    fclose(file);              // Tutup file
    printf("Data berhasil disimpan ke 'laporan_pasien.txt'.\n"); // Info simpan sukses
}

/*
Cara kerja fungsi exportToCSV:
- Fungsi ini mengekspor data pasien ke dalam file CSV.
- Jika file tidak dapat dibuka, fungsi akan memberi tahu pengguna.
 */
void exportToCSV(void) {
    FILE* file = fopen("laporan_pasien.csv", "w"); // Buka file CSV untuk tulis
    if (!file) {              // Jika gagal buka file
        printf("Gagal membuka file CSV 'laporan_pasien.csv'!\n"); // Info kegagalan
        return;              // Kembali tanpa ekspor
    }
    // Tulis header kolom CSV
    fprintf(file, "ID,Nama,Usia,Jenis Kelamin,Tinggi(m),Berat(kg),BMI,Glukosa(mmol/L),Kolesterol(mmol/L),TD Sistolik(mmHg),TD Diastolik(mmHg),Suhu(C),Denyut(bpm),Merokok,Tanggal,Status Risiko\n");
    for (int i = 0; i < patientCount; i++) { // Loop semua pasien
        struct Patient* p = &patients[i];     // Pointer ke pasien
        float bmi = calculateBMI(p->weight, p->height); // Hitung BMI
        int score = calculateTotalRiskScore(p);          // Hitung skor risiko
        const char* status = overallRiskLevel(score);     // Status risiko

        // Tulis data pasien dalam format CSV
        fprintf(file, "%d,\"%s\",%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.0f,%.0f,%.1f,%d,%s,%s,%s\n",
            p->id, p->name, p->age, p->gender,
            p->height, p->weight, bmi, p->glucose,
            p->cholesterol, p->systolic, p->diastolic,
            p->temperature, p->heartRate,
            p->smokerStatus, p->date, status);
    }
    fclose(file);              // Tutup file CSV
    printf("Data berhasil diekspor ke 'laporan_pasien.csv'\n"); // Info ekspor sukses
}

/*
Cara kerja fungsi newDatabase:
- Fungsi ini membuat database baru dengan menghapus data yang ada.
- Jumlah pasien direset dan file laporan dihapus.
 */
void newDatabase(void) {
    patientCount = 0;                      // Reset jumlah pasien
    nextID = 1;                            // Set ID berikutnya ke 1
    const char* txt = "laporan_pasien.txt"; // Nama file teks
    const char* csv = "laporan_pasien.csv"; // Nama file CSV

    remove(txt);                           // Hapus file teks jika ada
    remove(csv);                           // Hapus file CSV jika ada
    printf("Database baru telah dibuat.\n"); // Pesan sukses
}

/*
Cara kerja fungsi searchAndDisplayPatientDetails:
- Fungsi ini mencari pasien berdasarkan ID dan menampilkan detailnya.
- Jika pasien ditemukan, detail akan ditampilkan, jika tidak, akan memberi tahu pengguna.
 */
void searchAndDisplayPatientDetails(int id) {
    struct Patient* p = searchPatient(id); // Cari pasien berdasarkan ID
    if (p) {
        printf("\n--- Detail Pasien ID: %d ---\n", id);
        float bmi = calculateBMI(p->weight, p->height); // Hitung BMI
        int score = calculateTotalRiskScore(p);          // Hitung skor risiko
        const char* status= overallRiskLevel(score);     // Dapatkan status risiko

        // Tampilkan detail pasien
        printf("Nama               : %s\n", p->name);
        printf("Usia               : %d tahun\n", p->age);
        printf("Jenis Kelamin      : %s\n", p->gender);
        printf("Tinggi Badan       : %.2f meter\n", p->height);
        printf("Berat Badan        : %.2f kg\n", p->weight);
        printf("BMI                : %.2f\n", bmi);
        printf("Tekanan Darah      : %.0f/%.0f mmHg\n", p->systolic, p->diastolic);
        printf("Suhu Tubuh         : %.1f C\n", p->temperature);
        printf("Denyut Jantung     : %d bpm\n", p->heartRate);
        printf("Kadar Glukosa      : %.2f mmol/L\n", p->glucose);
        printf("Kolesterol         : %.2f mmol/L\n", p->cholesterol);
        printf("Status Merokok     : %s\n", p->smokerStatus);
        printf("Tanggal Pemeriksaan: %s\n", p->date);
        printf("Status Risiko      : %s\n", status);
        printf("Skor Risiko Total  : %d\n", score);
        printf("-----------------------------------\n");
    } else {
        printf("Pasien dengan ID %d tidak ditemukan.\n", id); // Jika tidak ditemukan
    }
}

/*
Cara kerja fungsi saveSinglePatient:
- Fungsi ini menyimpan laporan untuk pasien tertentu ke dalam file.
- Jika pasien tidak ditemukan, fungsi akan memberi tahu pengguna.
 */
int saveSinglePatient(int id) {
    struct Patient* p = searchPatient(id); // Cari pasien berdasarkan ID
    if (!p) {
        printf("Pasien dengan ID %d tidak ditemukan untuk disimpan laporannya.\n", id);
        return -1;                        // Kode gagal
    }

    char filename[100];                   // Buffer untuk nama file
    sprintf(filename, "laporan_pasien_%d.txt", id); // Format nama file

    FILE* file = fopen(filename, "w");    // Membuka file untuk ditulis
    if (!file) {
        printf("Gagal membuka file '%s' untuk pasien ID %d!\n", filename, id); // Pesan gagal
        return -1;                       // Kembalikan -1 jika gagal buka file
    }

    float bmi = calculateBMI(p->weight, p->height); // Hitung BMI
    int score = calculateTotalRiskScore(p);          // Hitung skor risiko
    const char* status = overallRiskLevel(score);     // Dapatkan status risiko

    // Tulis isi laporan ke file
    fprintf(file, "=== Laporan Evaluasi Pasien %s (ID: %d) ===\n", p->name, p->id);
    fprintf(file, "Tanggal Pemeriksaan: %s\n", p->date);
    fprintf(file, "Usia               : %d tahun\n", p->age);
    fprintf(file, "Jenis Kelamin      : %s\n", p->gender);
    fprintf(file, "Tinggi Badan       : %.2f meter\n", p->height);
    fprintf(file, "Berat Badan        : %.2f kg\n", p->weight);
    fprintf(file, "BMI                : %.2f\n", bmi);
    fprintf(file, "Glukosa            : %.2f mmol/L\n", p->glucose);
    fprintf(file, "Tekanan Darah      : %.0f/%.0f mmHg\n", p->systolic, p->diastolic);
    fprintf(file, "Suhu Tubuh         : %.1f C\n", p->temperature);
    fprintf(file, "Denyut Jantung     : %d bpm\n", p->heartRate);
    fprintf(file, "Kolesterol         : %.2f mmol/L\n", p->cholesterol);
    fprintf(file, "Status Merokok     : %s\n", p->smokerStatus);
    fprintf(file, "Status Risiko      : %s\n", status);
    fprintf(file, "Skor Risiko Total  : %d\n", score);
    fprintf(file, "==========================================\n");

    fclose(file);                          // Tutup file setelah selesai
    printf("Laporan untuk pasien ID %d berhasil disimpan ke '%s'.\n", id, filename);
    return 0;                             // Kode sukses
}

/*
Cara kerja fungsi viewPatientStatus:
- Fungsi ini menampilkan status risiko pasien berdasarkan ID.
- Jika pasien tidak ditemukan, fungsi akan memberi tahu pengguna.
 */
int viewPatientStatus(int id) {
    struct Patient* p = searchPatient(id); // Cari pasien berdasarkan ID
    if (!p) {                       // Jika pasien tidak ditemukan
        printf("Pasien dengan ID %d tidak ditemukan untuk melihat status.\n", id);
        return -1;                 // Kode gagal
    }
    printf("\n--- Status untuk Pasien ID %d ---\n", id); // Header status pasien
    evaluatePatient(p);            // Evaluasi dan tampilkan status pasien
    printf("--------------------------------\n");
    return 0;                     // Kode sukses
}
