#include "utils.h" // Mengimpor header utilitas umum
#include "database.h" // Mengimpor header fungsi database
#include "patient.h" // Mengimpor header definisi dan fungsi pasien

/*
 * Cara kerja fungsi clearInputBuffer:
 * Fungsi ini membaca karakter dari input standar menggunakan getchar() hingga menemukan newline ('\n') atau end-of-file (EOF).
 * Ini akan membersihkan buffer input dan memastikan bahwa input yang tersisa tidak mengganggu operasi input berikutnya.
 */
void clearInputBuffer(void) { // Variabel untuk penampung karakter sementara
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Baca karakter sampai menemukan newline atau sampai akhir file/input
    }
/*
 * Cara kerja fungsi createAndAddPatient:
 * Fungsi ini menambah pasien baru dengan meminta detail pasien dari pengguna,
 * memvalidasi input data dan menambahkan pasien tersebut ke database jika data valid.
 * Setelah itu, fungsi akan mengevaluasi data pasien yang baru dibuat.
 */
void createAndAddPatient(void) {
    struct Patient newPatient; // Mendeklarasikan variabel struct pasien baru
    inputPatient(&newPatient); // Memanggil fungsi input untuk mengisi data pasien
    if (addPatient(&newPatient) == 0) { // Menambahkan pasien ke database, cek keberhasilan
        evaluatePatient(&newPatient);} // Evaluasi data pasien jika penambahan berhasil
    }
/*
 * Cara kerja fungsi deletePatientData:
 * Fungsi ini menghapus data pasien berdasarkan ID yang dimasukkan pengguna.
 * Fungsi meminta ID pasien, memvalidasi input, kemudian menghapus pasien dari database.
 */        
void deletePatientData(void) {
    int id; // Variabel untuk menampung ID pasien
    printf("Masukkan ID Pasien yang ingin dihapus: "); // Minta input ID pasien yang akan dihapus
    if (scanf("%d", &id) != 1) { // Baca input ID dan cek validitas input
        clearInputBuffer(); // Bersihkan buffer input jika input tidak valid
        printf("Input ID tidak valid.\n"); // Informasi kesalahan input
        return;} // Menghentikan fungsi jika input salah
    clearInputBuffer(); // Bersihkan buffer input agar tidak mengganggu input berikutnya
    deletePatient(id); // Panggil fungsi untuk menghapus pasien berdasarkan ID
}

/*
 * Cara kerja fungsi updatePatientData:
 * Fungsi ini memperbarui data pasien yang sudah ada berdasarkan ID.
 * Fungsi meminta ID pasien, mengecek apakah pasien ada, lalu meminta input 
 * data baru dan mengganti data lama dengan data yang baru.
 * Fungsi juga menjaga agar ID pasien tidak berubah.
 */
void updatePatientData(void) {
    int id; // Variabel untuk menampung ID pasien yang akan diperbarui
    printf("Masukkan ID Pasien yang ingin diperbarui: "); // Minta input ID pasien
    if (scanf("%d", &id) != 1) { // Baca input dan validasi
        clearInputBuffer(); // Bersihkan buffer jika input tidak valid
        printf("Input ID tidak valid.\n"); // Informasi kesalahan input
        return;} // Hentikan fungsi jika input tidak valid
    clearInputBuffer(); // Bersihkan buffer input sebelum operasi selanjutnya

    if (searchPatient(id) == NULL) {  // Cari pasien berdasarkan ID, cek apakah ada
        printf("Pasien dengan ID %d tidak ditemukan. Tidak dapat memperbarui.\n", id); // Jika tidak ditemukan, beri info
        return;                       // Hentikan fungsi karena pasien tidak ditemukan
    }
    struct Patient p;                  // Variabel struct untuk data pasien baru
    printf("--- Memasukkan detail baru untuk pasien ID %d ---\n", id);  // Informasi proses update data
    printf("Catatan: ID Pasien tidak dapat diubah.\n");  // Peringatan bahwa ID tidak boleh diubah
    inputPatient(&p);                  // Minta input data pasien baru yang akan menggantikan
    p.id = id;                        // ID tetap dipertahankan agar tidak berubah
    if (updatePatient(id, &p) == 0) { // Perbarui data pasien dengan data baru, cek hasilnya
        evaluatePatient(&p);           // Evaluasi data yang telah diperbarui jika berhasil
    }
}

/*
 * Cara kerja fungsi searchPatientData:
 * Fungsi ini mencari data pasien berdasarkan ID yang dimasukkan pengguna
 * dan menampilkan detail pasien tersebut jika ditemukan.
 */
void searchPatientData(void) {
    int id;                            // Variabel untuk menampung ID pasien yang dicari
    printf("Masukkan ID Pasien yang ingin dicari: ");  // Minta input ID pasien
    if (scanf("%d", &id) != 1) {      // Baca dan validasi input ID pasien
        clearInputBuffer();            // Bersihkan buffer jika input tidak valid
        printf("Input ID tidak valid.\n");  // Informasi kesalahan input
        return;                       // Hentikan fungsi jika input salah
    }
    clearInputBuffer();                // Bersihkan buffer setelah input valid
    searchAndDisplayPatientDetails(id);  // Cari pasien dan tampilkan detailnya
}

/*
 * Cara kerja fungsi saveSinglePatientData:
 * Fungsi ini menyimpan laporan data pasien tunggal berdasarkan ID.
 * Fungsi meminta ID pasien, memvalidasi input lalu menyimpan laporan
 * jika pasien ditemukan dan proses simpan berhasil.
 */
void saveSinglePatientData(void) {
    int id;                            // Variabel untuk menampung ID pasien yang laporannya akan disimpan
    printf("Masukkan ID Pasien yang ingin disimpan laporannya: ");  // Minta input ID pasien
    if (scanf("%d", &id) != 1) {      // Baca input dan validasi
        clearInputBuffer();            // Bersihkan buffer jika input salah
        printf("Input ID tidak valid.\n");  // Informasi kesalahan input
        return;                       // Hentikan fungsi jika salah input
    }
    clearInputBuffer();                // Bersihkan buffer input
    if (saveSinglePatient(id) == 0) { // Simpan laporan pasien jika berhasil
        printf("Laporan pasien dengan ID %d berhasil disimpan.\n", id); // Konfirmasi keberhasilan penyimpanan
    }
}

/*
 * Cara kerja fungsi checkPatientStatusData:
 * Fungsi ini memeriksa dan menampilkan status pasien berdasarkan ID yang dimasukkan pengguna.
 */
void checkPatientStatusData(void) {
    int id;                            // Variabel untuk menampung ID pasien yang statusnya ingin dicek
    printf("Masukkan ID Pasien yang ingin diperiksa statusnya: ");  // Minta input ID pasien
    if (scanf("%d", &id) != 1) {      // Baca dan validasi input ID pasien
        clearInputBuffer();            // Bersihkan buffer jika input salah
        printf("Input ID tidak valid.\n");  // Pesan error input tidak valid
        return;                       // Hentikan fungsi jika input salah
    }
    clearInputBuffer();                // Bersihkan buffer input agar siap untuk operasi berikutnya
    viewPatientStatus(id);             // Tampilkan status pasien berdasarkan ID yang dimasukkan
}