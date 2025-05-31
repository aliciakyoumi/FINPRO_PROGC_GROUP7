#ifndef PATIENT_H // Jika PATIENT_H belum didefinisikan
#define PATIENT_H // Definisikan PATIENT_H agar file ini tidak di-include berulang

//Struct ini nyimpen data-data seorang pasien dan kesehatannya.
struct Patient {
    int id;                   // ID unik buat pasien
    char name[50];            // Nama pasiennya
    int age;                  // Umur pasien
    char gender[10];          // Jenis kelamin ("pria" atau "wanita")
    float height;             // Tinggi badan dalam cm
    float weight;             // Berat badan dalam kg
    float systolic;           // Tekanan darah sistolik (atas) dalam mmHg
    float diastolic;          // Tekanan darah diastolik (bawah) dalam mmHg
    float temperature;        // Suhu tubuh dalam °C
    int heartRate;            // Denyut jantung per menit (BPM)
    float glucose;            // Kadar gula darah (mg/dL)
    float cholesterol;        // Kadar kolesterol (mg/dL)
    char smokerStatus[6];     // Status merokok ("ya" atau "tidak")
    char date[20];            // Tanggal data ini dicatat (misal: "31/05/2025")
};

//Fungsi buat ngitung Indeks Massa Tubuh (BMI) dari berat & tinggi.
float calculateBMI(float weight, float height);// Deklarasi fungsi: Hitung BMI

//Fungsi buat ngitung skor BMI berdasarkan nilai BMI & jenis kelamin.
int scoreBMI(float bmi, const char* gender);// Deklarasi fungsi: Kasih skor buat BMI

//Fungsi buat ngitung skor glukosa dari kadar glukosa darah.
int scoreGlucose(float glucose);// Deklarasi fungsi: Kasih skor buat glukosa

//Fungsi buat ngitung skor tekanan darah dari nilai sistolik & diastolik.
int scoreBP(float systolic, float diastolic);// Deklarasi fungsi: Kasih skor buat tekanan darah

//Fungsi buat ngitung skor kolesterol dari kadar kolesterol.
int scoreChol(float cholesterol);// Deklarasi fungsi: Kasih skor buat kolesterol

//Fungsi buat ngitung skor perokok dari status merokoknya.
int scoreSmoker(const char* status);// Deklarasi fungsi: Kasih skor buat status merokok

//Fungsi buat ngitung total skor risiko kesehatan dari semua skor individu.
int calculateTotalRiskScore(const struct Patient* p);// Deklarasi fungsi: Hitung total semua skor risiko

//Fungsi buat nentuin level risiko keseluruhan dari total skor.
const char* overallRiskLevel(int totalScore); // Deklarasi fungsi: Tentukan level risiko (misal: Rendah, Sedang, Tinggi)

//Fungsi buat minta pengguna masukin data-data pasien.
void inputPatient(struct Patient* p);// Deklarasi fungsi: Minta input data pasien

//Fungsi buat nampilin laporan evaluasi dari data kesehatan pasien.
void evaluatePatient(const struct Patient* p);// Deklarasi fungsi: Tampilkan evaluasi kesehatan pasien

//Fungsi buat nampilin saran gaya hidup berdasarkan total skor risiko.
void lifestyleRecommendation(int score);// Deklarasi fungsi: Kasih saran gaya hidup

//Fungsi buat nampilin diagram risiko pake teks ASCII berdasarkan skor.
void asciiDiagram(int score);// Deklarasi fungsi: Tampilkan diagram risiko sederhana

#endif // Akhir dari header