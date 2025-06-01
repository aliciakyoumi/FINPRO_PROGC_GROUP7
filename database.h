// Header guard
#ifndef DATABASE_H // Cek apakah DATABASE_H belum didefinisikan
#define DATABASE_H // Definisikan DATABASE_H

#include "patient.h" // Mengimpor header untuk definisi struct Patient
#define MAX 100     // Mendefinisikan maksimum jumlah pasien

extern struct Patient patients[MAX];   // Deklarasi array eksternal untuk menyimpan data pasien
extern int patientCount;               // Deklarasi variabel eksternal untuk menghitung jumlah pasien
extern int nextID;                     // Deklarasi variabel eksternal untuk ID berikutnya

void initDB(void); // Menginisialisasi database pasien dengan mengatur jumlah pasien ke 0 dan memuat data awal dari file CSV
int addPatient(struct Patient* pDetails); // Menambahkan pasien baru ke database
struct Patient* searchPatient(int id); // Mencari pasien berdasarkan ID yang diberikan
int deletePatient(int id); // Menghapus pasien dari database berdasarkan ID
int updatePatient(int id, const struct Patient* pDetails); // Memperbarui data pasien yang sudah ada berdasarkan ID
void displayPatients(void); // Menampilkan semua data pasien yang ada dalam database
void saveToFile(void); // Menyimpan seluruh data pasien ke dalam file teks
void exportToCSV(void); // Mengekspor semua data pasien ke dalam file CSV
int saveSinglePatient(int id); // Menyimpan laporan untuk pasien tertentu ke dalam file
void newDatabase(void); // Menghapus semua pasien dan mereset database
void searchAndDisplayPatientDetails(int id); // Memilih pasien berdasarkan ID dan menampilkan detailnya
int viewPatientStatus(int id); // Melihat status dan nama pasien berdasarkan ID

#endif // Penutup header guard