
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

void evaluateAndSave(struct Patient* p, FILE* file) {
    float bmi = calculateBMI(p->weight, p->height);
    int score = 0;
    score += scoreBMI(bmi, p->gender);
    score += scoreGlucose(p->glucose);
    score += scoreBP(p->systolic, p->diastolic);
    score += scoreChol(p->cholesterol);
    score += scoreSmoker(p->smokerStatus);

    fprintf(file, "\n=== Laporan Evaluasi Pasien %s (%s) ===\n", p->name, p->id);
    fprintf(file, "Usia: %d | Jenis Kelamin: %s\n", p->age, p->gender);
    fprintf(file, "BMI: %.2f | Skor: %d\n", bmi, scoreBMI(bmi, p->gender));
    fprintf(file, "Glukosa: %.2f mmol/L | Skor: %d\n", p->glucose, scoreGlucose(p->glucose));
    fprintf(file, "Tekanan Darah: %.0f/%.0f mmHg | Skor: %d\n", p->systolic, p->diastolic, scoreBP(p->systolic, p->diastolic));
    fprintf(file, "Kolesterol: %.2f mmol/L | Skor: %d\n", p->cholesterol, scoreChol(p->cholesterol));
    fprintf(file, "Status Merokok: %s | Skor: %d\n", p->smokerStatus, scoreSmoker(p->smokerStatus));
    fprintf(file, "Total Skor Risiko: %d -> %s\n", score, overallRiskLevel(score));
}


int main() {
    struct Patient patients[MAX];
    int n, i;

    printf("Masukkan jumlah pasien: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("\n--- Input Data Pasien ke-%d ---\n", i + 1);
        printf("ID Pasien: ");
        scanf("%s", patients[i].id);
        printf("Nama: ");
        scanf(" %[^\n]", patients[i].name);
        printf("Usia: ");
        scanf("%d", &patients[i].age);
        printf("Jenis Kelamin (pria/wanita): ");
        scanf("%s", patients[i].gender);
        printf("Tinggi (meter): ");
        scanf("%f", &patients[i].height);
        printf("Berat Badan (kg): ");
        scanf("%f", &patients[i].weight);
        printf("Tekanan Darah Sistolik (mmHg): ");
        scanf("%f", &patients[i].systolic);
        printf("Tekanan Darah Diastolik (mmHg): ");
        scanf("%f", &patients[i].diastolic);
        printf("Suhu Tubuh (°C): ");
        scanf("%f", &patients[i].temperature);
        printf("Denyut Jantung (bpm): ");
        scanf("%d", &patients[i].heartRate);
        printf("Kadar Glukosa (mmol/L): ");
        scanf("%f", &patients[i].glucose);
        printf("Kolesterol (mmol/L): ");
        scanf("%f", &patients[i].cholesterol);
        printf("Merokok? (ya/tidak): ");
        scanf("%s", patients[i].smokerStatus);
    }

    FILE* file = fopen("laporan_pasien.txt", "w");
    if (!file) {
        printf("Gagal membuka file output!\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        evaluateAndSave(&patients[i], file);
    }

    fclose(file);
    printf("\n Evaluasi selesai. Laporan tersimpan di 'laporan_pasien.txt'\n");

    return 0;
}
