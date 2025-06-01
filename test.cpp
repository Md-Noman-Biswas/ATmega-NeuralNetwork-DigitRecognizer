#include <stdio.h>
#include <stdint.h>

#define SCALE 1000

// Put the actual arrays here
int16_t W1[9][6] = {
    {-1291, 299, 772, -325, -3219, 4242},
    {1490, 2015, -1354, 345, 108, -1018},
    {630, 856, -44, -1376, -1313, 1958},
    {-2965, 483, -714, 1850, 678, -797},
    {226, -2564, -644, -429, 5739, -1403},
    {718, -1269, 4148, -4368, 1482, -660},
    {-331, -5077, 2219, 4121, -3562, 451},
    {-4774, 317, 305, 2715, -1331, 1555},
    {946, -557, 520, -594, 25, 60},
};

int16_t b1[6] = { 1333, 1878, -1309, 217, 198, -1025 };

int16_t W2[6][10] = {
    {-4174, -1259, 2629, -3360, 2469, -1548, -1638, 4575, -3389, -5977},
    {-5705, 835, -6677, -215, 12, 1432, -6040, 3414, -187, 2196},
    {1681, -5799, 1467, -182, -1077, -4587, 923, -1449, -126, 2567},
    {1727, 1902, -4112, -3457, -4799, 2933, 1223, -5385, -3465, -4507},
    {-3723, 1943, -1325, 3045, 3204, -5532, 3145, -5505, 3039, -5836},
    {3311, -3748, 312, 1198, -4289, 1318, -3865, -526, 1144, 604},
};

int16_t b2[10] = { -2718, -1279, -776, -3117, -1563, -1955, -1950, -3136, -3129, -1720 };

int16_t sigmoid(int32_t x) {
    if (x < -5000) return 0;
    if (x > 5000) return SCALE;
    return SCALE / 2 + x / 10;
}

void forward(uint8_t input[9], uint8_t* output_digit) {
    int16_t hidden[6];
    int16_t output[10];

    for (int i = 0; i < 6; i++) {
        int32_t sum = b1[i];
        for (int j = 0; j < 9; j++) {
            sum += input[j] * W1[j][i];
        }
        hidden[i] = sigmoid(sum / SCALE);
    }

    for (int i = 0; i < 10; i++) {
        int32_t sum = b2[i];
        for (int j = 0; j < 6; j++) {
            sum += hidden[j] * W2[j][i];
        }
        output[i] = sigmoid(sum / SCALE);
    }

    *output_digit = 0;
    int16_t max_val = output[0];
    for (int i = 1; i < 10; i++) {
        if (output[i] > max_val) {
            max_val = output[i];
            *output_digit = i;
        }
    }
}

int main() {
    uint8_t input[9];
    uint8_t digit;

    printf("Enter 9 binary values (0 or 1) separated by spaces:\n");
    for (int i = 0; i < 9; i++) {
        scanf("%hhu", &input[i]);  // %hhu = unsigned 8-bit int
    }

    forward(input, &digit);

    printf("Predicted digit: %d\n", digit);
    return 0;
}

