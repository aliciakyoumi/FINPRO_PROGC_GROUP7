#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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
    char smokerStatus[6]; 
    char date[20]; 
};

char currentRole[10];

struct Patient patients [MAX];
int n = 0;  

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
void lifestyleRecommendation(int score) {
    printf("\nRekomendasi Gaya Hidup:\n");
    if (score <= 2) {
        printf("- Pertahankan pola hidup sehat.\n");
    } else if (score <= 5) {
        printf("- Kurangi konsumsi gula dan garam.\n");
        printf("- Rutin olahraga ringan.\n");
    } else {
        printf("- Konsultasi rutin dengan dokter.\n");
        printf("- Perubahan gaya hidup menyeluruh diperlukan.\n");
    }
}

void asciiDiagram(int score) {
    printf("\nDiagram Risiko (0-6):\n");
    printf("[ ");
    for (int i = 0; i < 7; i++) {
        if (i == score)
            printf("*");
        else
            printf("-");
    }
    printf(" ]\n");
}

void inputPatient(struct Patient* p) {
    printf("ID Pasien               : ");
    scanf("%d", &p->id);
    printf("Nama                    : ");
    scanf(" %[^\n]", p->name);
    for (int i = 0; p->name[i]; i++) {
        if (i == 0 || p->name[i - 1] == ' ') {
            p->name[i] = toupper(p->name[i]);
        } else {
            p->name[i] = tolower(p->name[i]);
        }
    }
    printf("Usia (tahun)            : ");
    scanf("%d", &p->age);
    printf("Jenis Kelamin (pria/wanita): ");
    scanf("%s", p->gender);
    
    for (int i = 0; p->gender[i]; i++) {
        p->gender[i] = tolower(p->gender[i]);
    }
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
    
    for (int i = 0; p->smokerStatus[i]; i++) {
        p->smokerStatus[i] = tolower(p->smokerStatus[i]);
    }
    printf("Tanggal (DD/MM/YYYY)    : ");
    scanf("%s", p->date); 
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
    asciiDiagram(score);
    lifestyleRecommendation(score);
}

void saveToFile() {
    if (strcmp(currentRole, "Admin") != 0 && strcmp(currentRole, "Dokter") != 0) {
        printf("Anda tidak memiliki izin untuk menyimpan data.\n");
        return;
    }
    
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

    fclose(file);
    printf("\nData berhasil disimpan ke 'laporan_pasien.txt'\n");
}

void exportToCSV() {
    if (strcmp(currentRole, "Admin") != 0 && strcmp(currentRole, "Dokter") != 0) {
        printf("Anda tidak memiliki izin untuk mengekspor data.\n");
        return;
    }

    FILE* file = fopen("laporan_pasien.csv", "w");
    if (!file) {
        printf("Gagal membuat file CSV!\n");
        return;
    }
    fprintf(file, "ID,Nama,Usia,Jenis Kelamin,Tinggi,Berat,BMI,Glukosa,Kolesterol,TD Sistolik,TD Diastolik,Merokok,Tanggal,Status\n");
    for (int i = 0; i < n; i++) {
        float bmi = calculateBMI(patients[i].weight, patients[i].height);
        int score = 0;
        score += scoreBMI(bmi, patients[i].gender);
        score += scoreGlucose(patients[i].glucose);
        score += scoreBP(patients[i].systolic, patients[i].diastolic);
        score += scoreChol(patients[i].cholesterol);
        score += scoreSmoker(patients[i].smokerStatus);
        const char* status = overallRiskLevel(score);
        fprintf(file, "%d,%s,%d,%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.0f,%.0f,%s,%s,%s\n",
            patients[i].id, patients[i].name, patients[i].age, patients[i].gender,
            patients[i].height, patients[i].weight, bmi, patients[i].glucose,
            patients[i].cholesterol, patients[i].systolic, patients[i].diastolic,
            patients[i].smokerStatus, patients[i].date, status);
    }
    fclose(file);
    printf("Data berhasil diekspor ke 'laporan_pasien.csv'\n");
}

void saveSinglePatient() {
    int id;
    printf("Masukkan ID pasien: ");
    scanf("%d", &id);
    for (int i = 0; i < n; i++) {
        if (patients[i].id == id) {
            char filename[60];
            sprintf(filename, "pasien_%d.txt", id);
            FILE* file = fopen(filename, "w");
            if (!file) {
                printf("Gagal membuat file laporan.\n");
                return;
            }
            float bmi = calculateBMI(patients[i].weight, patients[i].height);
            int score = scoreBMI(bmi, patients[i].gender) + scoreGlucose(patients[i].glucose) + scoreBP(patients[i].systolic, patients[i].diastolic) + scoreChol(patients[i].cholesterol) + scoreSmoker(patients[i].smokerStatus);
            const char* status = overallRiskLevel(score);
            fprintf(file, "Laporan Pasien %s (ID: %d) tanggal %s\n", patients[i].name, patients[i].id, patients[i].date);
            fprintf(file, "Usia               : %d tahun\n", patients[i].age);
            fprintf(file, "Jenis Kelamin      : %s\n", patients[i].gender);
            fprintf(file, "BMI                : %.2f\n", bmi);
            fprintf(file, "Glukosa            : %.2f mmol/L\n", patients[i].glucose);
            fprintf(file, "Tekanan Darah      : %.0f/%.0f mmHg\n", patients[i].systolic, patients[i].diastolic);
            fprintf(file, "Kolesterol         : %.2f mmol/L\n", patients[i].cholesterol);
            fprintf(file, "Status Merokok     : %s\n", patients[i].smokerStatus);
            fprintf(file, "Status Risiko      : %s\n", status);
            fclose(file);
            printf("Laporan berhasil disimpan ke %s\n", filename);
            return;
        }
    }
    printf("Pasien tidak ditemukan.\n");
}

void searchPatient() {
    char keyword[50];
    printf("Masukkan nama atau ID pasien: ");
    scanf(" %[^\n]", keyword);

    int isDigit = 1;
    for (int i = 0; keyword[i]; i++) {
        if (!isdigit(keyword[i])) {
            isDigit = 0;
            break;
        }
    }
    printf("\nHasil Pencarian:\n");
    for (int i = 0; i < n; i++) {
        if ((isDigit && patients[i].id == atoi(keyword)) ||
            (!isDigit && strstr(patients[i].name, keyword))) {
            printf("%d. %s (ID: %d)\n", i+1, patients[i].name, patients[i].id);
        }
    }
}

void displayPatients() {
    printf("\n=== Data Pasien ===\n");
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

        printf("\n%d. %s (ID: %d)\n", i + 1, p->name, p->id);
        printf("Tanggal            : %s \n", p->date);
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

void deletePatient() {
    if (strcmp(currentRole, "Admin") != 0) {
        printf("Anda tidak memiliki izin untuk menghapus data.\n");
        return;
    }
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

void updatePatient() {
    if (strcmp(currentRole, "Admin") != 0) {
        printf("Anda tidak memiliki izin untuk memperbarui data.\n");
        return;
    }
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

void newDatabase() {
    n = 0;
    printf("Database pasien baru telah dibuat (data lama dihapus).\n");
}

void viewPatientStatus() {
    int id;
    printf("Masukkan ID pasien untuk melihat status: ");
    scanf("%d", &id);
    for (int i = 0; i < n; i++) {
        if (patients[i].id == id) {
            evaluatePatient(&patients[i]);
            return;
        }
    }
    printf("Pasien dengan ID %d tidak ditemukan.\n", id);
}

int main() {
    char adminName[50];
    int choice;
    int exitProgram = 0;

    printf("=====================================\n");
    printf("   Sistem Evaluasi Data Pasien\n");
    printf("=====================================\n");

    while (!exitProgram) {
        
        printf("Masukkan nama Anda: ");
        scanf(" %[^\n]", adminName);

        printf("Masukkan peran Anda (Admin/Dokter/Perawat): ");
        scanf("%s", currentRole);
        for (int i = 0; currentRole[i]; i++) {
            currentRole[i] = tolower(currentRole[i]);
        }

        int sessionEnded = 0;
        while (!sessionEnded) {
            printf("\n=====================================\n");
            printf(" Menu:\n");
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
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    if (n >= MAX) {
                        printf("Database penuh! Tidak dapat menambah pasien baru.\n");
                        break;
                    }
                    if (strcmp(currentRole, "admin") == 0 || strcmp(currentRole, "dokter") == 0) {
                        inputPatient(&patients[n]);
                        evaluatePatient(&patients[n]);
                        n++;
                    }
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
                    exportToCSV();
                    break;
                case 7:
                    newDatabase();
                    break;
                case 8:
                    searchPatient();
                    break;
                case 9:
                    saveSinglePatient();
                    break;
                case 10:
                    viewPatientStatus();
                    break;
                case 0: {
                    int subChoice;
                    printf("\nApakah Anda ingin:\n");
                    printf("1. Ganti Nama dan Role\n");
                    printf("2. Keluar Program\n");
                    printf("Pilihan Anda: ");
                    scanf("%d", &subChoice);

                    if (subChoice == 1) {
                        sessionEnded = 1;
                    } else if (subChoice == 2) {
                        printf("Keluar program...\n");
                        sessionEnded = 1;
                        exitProgram = 1;
                    } else {
                        printf("Pilihan tidak valid! Kembali ke menu utama.\n");
                    }
                    break;
                }
                default:
                    printf("Pilihan tidak valid!\n");
            }
        }
    }

    return 0;
}