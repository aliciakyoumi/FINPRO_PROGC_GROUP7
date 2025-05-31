// Header guard
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h> //library untuk standar input/output

void clearInputBuffer(void); // Deklarasi fungsi untuk membersihkan buffer input

void createAndAddPatient(void); // Deklarasi fungsi untuk membuat dan menambahkan pasien baru

void deletePatientData(void); // Deklarasi fungsi untuk menghapus data pasien berdasarkan ID

void updatePatientData(void);  // Deklarasi fungsi untuk memperbarui data pasien yang ada

void searchPatientData(void); // Deklarasi fungsi untuk mencari data pasien berdasarkan ID

void saveSinglePatientData(void); // Deklarasi fungsi untuk menyimpan laporan pasien

void checkPatientStatusData(void);  // Deklarasi fungsi untuk memeriksa status pasien

#endif // Penutup header guard