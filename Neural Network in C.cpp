#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define INPUT_SIZE   16
#define HIDDEN_SIZE  6
#define OUTPUT_SIZE 10

// Neural Network weights and biases (fixed-point scaled by 1)
int16_t W1[INPUT_SIZE][HIDDEN_SIZE] = {
    {-798, 7337, 6692, -824, 7020, -9256},
    {-409, 4245, 5279, 8433, 3413, 6538},
    {-1215, -3169, -4901, 6761, 696, 2547},
    {-1068, 1986, 5221, -342, -4134, 2066},
    {-2253, -2263, -5679, -1014, -4266, 1752},
    {-1146, 5536, -3638, -11704, -4898, -3984},
    {-268, -924, -5055, -1617, -8, 2581},
    {-1584, 715, 4913, -99, -4279, 1933},
    {-966, 3202, 4933, -66, -4191, 1952},
    {554, 1183, -2840, 974, 2310, -6884},
    {-1218, -3028, 1152, -16205, 909, 2206},
    {-1454, 253, 5001, -1313, -4113, 1970},
    {-33, -3179, -4959, 3550, -4250, -7060},
    {-379, -3342, 5329, 936, 2710, 2308},
    {-1352, -3302, -4707, -2191, 3118, 2346},
    {-1125, -7584, 363, 607, 1430, -5068},
};

int16_t b1[HIDDEN_SIZE] = { -1813, 1344, -5368, -2691, 1943, 432 };

int16_t W2[HIDDEN_SIZE][OUTPUT_SIZE] = {
    {631, 21, -440, 95, -1265, -126, 470, 291, 562, -483},
    {872, -1960, -8355, -9462, 188, -3115, -9325, 16809, -3231, 17873},
    {10884, -8155, -3704, 13257, -8657, 11099, -2075, -4607, -12705, 5000},
    {6177, -3151, 8294, -8465, -5126, -3662, 8920, 4215, -3714, -3827},
    {-2652, 11535, 8543, -11274, -9636, 13595, -14566, 655, 10199, -6437},
    {-2882, 9218, -6415, -9428, 10410, 2192, 8096, -1001, -11939, 1548},
};

int16_t b2[OUTPUT_SIZE] = { -2131, -4652, 1128, 9809, 4118, -8396, -1816, -4066, 7848, -1843 };

// Sigmoid activation with overflow clipping
static float sigmoid(float x) {
    if (x > 10.0f) return 1.0f;
    if (x < -10.0f) return 0.0f;
    return 1.0f / (1.0f + expf(-x));
}

// Softmax (numerically stable)
static void softmax(float in[OUTPUT_SIZE], float out[OUTPUT_SIZE]) {
    float m = in[0];
    for (int i = 1; i < OUTPUT_SIZE; ++i)
        if (in[i] > m) m = in[i];
    float sum = 0.0f;
    for (int i = 0; i < OUTPUT_SIZE; ++i) {
        float v = expf(in[i] - m);
        out[i] = v;
        sum += v;
    }
    if (sum == 0.0f) sum = 1e-6f;
    for (int i = 0; i < OUTPUT_SIZE; ++i)
        out[i] /= sum;
}

// Predict digit from a 16-length binary pattern
int predict(const int input_pattern[INPUT_SIZE]) {
    float x[INPUT_SIZE];
    for (int i = 0; i < INPUT_SIZE; ++i) x[i] = (float)input_pattern[i];

    float z1[HIDDEN_SIZE], a1[HIDDEN_SIZE];
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        // First layer accumulator (fixed-point scaled by 1000)
        float acc_fp = (float)b1[i];
        for (int j = 0; j < INPUT_SIZE; ++j)
            acc_fp += x[j] * (float)W1[j][i];
        // Convert fixed-point to real value
        float z = acc_fp / 1000.0f;
        z1[i] = z;
        a1[i] = sigmoid(z);
    }

    float z2[OUTPUT_SIZE], a2[OUTPUT_SIZE];
    for (int i = 0; i < OUTPUT_SIZE; ++i) {
        // Second layer accumulator (fixed-point scaled by 1000)
        float acc_fp = (float)b2[i];
        for (int j = 0; j < HIDDEN_SIZE; ++j)
            acc_fp += a1[j] * (float)W2[j][i];
        // Convert fixed-point to real value
        z2[i] = acc_fp / 1000.0f;
    }
    softmax(z2, a2);(z2, a2);

    int best = 0;
    for (int i = 1; i < OUTPUT_SIZE; ++i)
        if (a2[i] > a2[best]) best = i;
    return best;
}

int main(void) {
    int pattern[INPUT_SIZE];
    printf("Enter 16 binary values (0 or 1) separated by spaces:\n");
    for (int i = 0; i < INPUT_SIZE; ++i) {
        if (scanf("%d", &pattern[i]) != 1 || (pattern[i] != 0 && pattern[i] != 1)) {
            fprintf(stderr, "Invalid input at position %d.\n", i);
            return EXIT_FAILURE;
        }
    }
    int result = predict(pattern);
    printf("Predicted digit: %d\n", result);
    return EXIT_SUCCESS;
}
