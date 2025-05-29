#include <stdio.h>
#include <string.h>

#define MAX 100

struct Patient {
    int id;
    char name[50];
    int age;
    char gender[10];
    float height;
    float weight;
    float systolic;
    float diastolic;
    float temperature;
    int heartRate;
    float glucose;
    float cholesterol;
    char smokerStatus[5]; // ya / tidak
};

struct Patient patients [MAX];
int n = 0;  // jumlah data pasien saat ini

float calculateBMI(float weight, float height) {
    return weight / (height * height);
}

int scoreBMI(float bmi, const char* gender) {
    if (bmi >= 18.5 && bmi <= 24.9) return 0;
    else if ((bmi >= 25 && bmi <= 29.9) || (bmi < 18.5)) return 1;
    else if (bmi >= 30) return 2;
    return -1;
}

int scoreGlucose(float glucose) {
    if (glucose < 7.8) return 0;
    else if (glucose < 11.1) return 1;
    else return 2;
}

int scoreBP(float systolic, float diastolic) {
    if (systolic < 130 && diastolic < 80) return 0;
    else if ((systolic < 160) || (diastolic < 100)) return 1;
    else return 2;
}

int scoreChol(float cholesterol) {
    if (cholesterol < 5.0) return 0;
    else if (cholesterol < 7.5) return 1;
    else return 2;
}

int scoreSmoker(const char* status) {
    if (strcmp(status, "tidak") == 0) return 0;
    else return 2;
}

const char* overallRiskLevel(int totalScore) {
    if (totalScore <= 2) return "Sehat";
    else if (totalScore <= 5) return "Perlu Perhatian";
    else return "Risiko Tinggi";
}

//Menu yang dijalankan 
//Menu 1--> Memasukkan data pasien
void inputPatient(struct Patient* p) {
    printf("ID Pasien               : ");
    scanf("%d", &p->id);
    printf("Nama                    : ");
    scanf(" %[^\n]", p->name);
    printf("Usia (tahun)            : ");
    scanf("%d", &p->age);
    printf("Jenis Kelamin (pria/wanita): ");
    scanf("%s", p->gender);
    printf("Tinggi (meter)          : ");
    scanf("%f", &p->height);
    printf("Berat Badan (kg)        : ");
    scanf("%f", &p->weight);
    printf("Tekanan Darah Sistolik (mmHg): ");
    scanf("%f", &p->systolic);
    printf("Tekanan Darah Diastolik (mmHg): ");
    scanf("%f", &p->diastolic);
    printf("Suhu Tubuh (°C)         : ");
    scanf("%f", &p->temperature);
    printf("Denyut Jantung (bpm)    : ");
    scanf("%d", &p->heartRate);
    printf("Kadar Glukosa (mmol/L)  : ");
    scanf("%f", &p->glucose);
    printf("Kolesterol (mmol/L)     : ");
    scanf("%f", &p->cholesterol);
    printf("Merokok? (ya/tidak)     : ");
    scanf("%s", p->smokerStatus);
}

void evaluatePatient(struct Patient* p) {
    float bmi = calculateBMI(p->weight, p->height);
    int score = 0;
    score += scoreBMI(bmi, p->gender);
    score += scoreGlucose(p->glucose);
    score += scoreBP(p->systolic, p->diastolic);
    score += scoreChol(p->cholesterol);
    score += scoreSmoker(p->smokerStatus);


    const char* status = overallRiskLevel(score);

    printf("\nStatus Kesehatan Pasien %s (ID: %d): %s\n", p->name, p->id, status);
}


//Menu 2--> Menampilkan data pasien
void displayPatients() {
    printf("\n=== Data Pasien ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s (ID: %d)\n", i+1, patients[i].name, patients[i].id);
    }
}

//Menu 3--> Menghapus data pasien yang telah ada
void deletePatient() {
    int id, found = 0;
    printf("Masukkan ID pasien yang ingin dihapus: ");
    scanf("%d", &id);
    for (int i = 0; i < n; i++) {
        if (patients[i].id == id) {
            for (int j = i; j < n-1; j++) {
                patients[j] = patients[j+1];
            }
            n--;
            found = 1;
            printf("Pasien dengan ID %d berhasil dihapus.\n", id);
            break;
        }
    }
    if (!found) printf("Pasien dengan ID %d tidak ditemukan.\n", id);
}

//Menu 4--> Memperbarui data pasien yang telah ada
void updatePatient() {
    int id, found = 0;
    printf("Masukkan ID pasien yang ingin diperbarui: ");
    scanf("%d", &id);
    for (int i = 0; i < n; i++) {
        if (patients[i].id == id) {
            printf("Input ulang data pasien:\n");
            inputPatient(&patients[i]);
            evaluatePatient(&patients[i]);
            printf("Data pasien berhasil diperbarui.\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Pasien dengan ID %d tidak ditemukan.\n", id);
}

//Menu 5--> Menyimpan data pasien ke file txt
void saveToFile() {
    FILE* file = fopen("laporan_pasien.txt", "w");
    if (!file) {
        printf("Gagal membuka file output!\n");
        return;
    }


    for (int i = 0; i < n; i++) {
        struct Patient* p = &patients[i];
        float bmi = calculateBMI(p->weight, p->height);
        int score = 0;
        score += scoreBMI(bmi, p->gender);
        score += scoreGlucose(p->glucose);
        score += scoreBP(p->systolic, p->diastolic);
        score += scoreChol(p->cholesterol);
        score += scoreSmoker(p->smokerStatus);
        const char* status = overallRiskLevel(score);


        fprintf(file, "\n=== Laporan Evaluasi Pasien %s (ID: %d) ===\n", p->name, p->id);
        fprintf(file, "Usia               : %d tahun\n", p->age);
        fprintf(file, "Jenis Kelamin      : %s\n", p->gender);
        fprintf(file, "BMI                : %.2f\n", bmi);
        fprintf(file, "Glukosa            : %.2f mmol/L\n", p->glucose);
        fprintf(file, "Tekanan Darah      : %.0f/%.0f mmHg\n", p->systolic, p->diastolic);
        fprintf(file, "Kolesterol         : %.2f mmol/L\n", p->cholesterol);
        fprintf(file, "Status Merokok     : %s\n", p->smokerStatus);
        fprintf(file, "Status Risiko      : %s\n", status);
    }
    fclose(file);
    printf("\nData berhasil disimpan ke 'laporan_pasien.txt'\n");
}


//Menu 6--> Membuat Database Baru
void newDatabase() {
    n = 0;
    printf("Database pasien baru telah dibuat (data lama dihapus).\n");
}


int main() {
    char adminName[50], accessKey[10];
    int choice;

    printf("=====================================\n");
    printf("   Sistem Evaluasi Data Pasien\n");
    printf("=====================================\n");
    printf("Masukkan nama Anda: ");
    scanf(" %[^\n]", adminName);

    printf("Apakah Anda admin? (ya/tidak): ");
    scanf("%s", accessKey);

    if (strcmp(accessKey, "ya") != 0) {
        printf("\nMaaf %s, Anda tidak dapat mengakses database pasien.\n", adminName);
        return 0;
    }
    do {
        printf("\n=====================================\n");
        printf(" Menu:\n");
        printf(" 1. Tambah Pasien Baru\n");
        printf(" 2. Tampilkan Data Pasien\n");
        printf(" 3. Hapus Data Pasien\n");
        printf(" 4. Perbarui Data Pasien\n");
        printf(" 5. Simpan ke File\n");
        printf(" 6. Buat Database Baru\n");
        printf(" 7. Keluar\n");
        printf("=====================================\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (n >= MAX) {
                    printf("Database penuh!\n");
                    break;
                }
                inputPatient(&patients[n]);
                evaluatePatient(&patients[n]);
                n++;
                break;
            case 2:
                displayPatients();
                break;
            case 3:
                deletePatient();
                break;
            case 4:
                updatePatient();
                break;
            case 5:
                saveToFile();
                break;
            case 6:
                newDatabase();
                break;
            case 7:
                printf("Keluar program...\n");
                break;
            default:
                printf("Pilihan tidak valid! Pilih angka 1-7!\n");
        }
    } while (choice != 0);

    return 0;
}


