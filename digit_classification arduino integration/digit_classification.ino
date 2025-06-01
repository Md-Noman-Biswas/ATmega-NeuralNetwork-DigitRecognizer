#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <math.h>
#include <avr/pgmspace.h>

// Keypad configuration (4x4 for 16 inputs)
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Neural Network dimensions (16 inputs now)
#define INPUT_SIZE   16
#define HIDDEN_SIZE  6
#define OUTPUT_SIZE 10

// PROGMEM weights and biases (scaled by 1000 fixed-point)
const int16_t W1[INPUT_SIZE][HIDDEN_SIZE] PROGMEM = {
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
  {-1125, -7584, 363, 607, 1430, -5068}
};
const int16_t b1[HIDDEN_SIZE] PROGMEM = {-1813, 1344, -5368, -2691, 1943, 432};

const int16_t W2[HIDDEN_SIZE][OUTPUT_SIZE] PROGMEM = {
  {631, 21, -440, 95, -1265, -126, 470, 291, 562, -483},
  {872, -1960, -8355, -9462, 188, -3115, -9325, 16809, -3231, 17873},
  {10884, -8155, -3704, 13257, -8657, 11099, -2075, -4607, -12705, 5000},
  {6177, -3151, 8294, -8465, -5126, -3662, 8920, 4215, -3714, -3827},
  {-2652, 11535, 8543, -11274, -9636, 13595, -14566, 655, 10199, -6437},
  {-2882, 9218, -6415, -9428, 10410, 2192, 8096, -1001, -11939, 1548}
};
const int16_t b2[OUTPUT_SIZE] PROGMEM = {-2131, -4652, 1128, 9809, 4118, -8396, -1816, -4066, 7848, -1843};

// Activation functions
float sigmoid(float x) {
  if (x > 10.0f) return 1.0f;
  if (x < -10.0f) return 0.0f;
  return 1.0f / (1.0f + expf(-x));
}

void softmax(float inArr[OUTPUT_SIZE], float outArr[OUTPUT_SIZE]) {
  float maxv = inArr[0];
  for (int i = 1; i < OUTPUT_SIZE; i++) if (inArr[i] > maxv) maxv = inArr[i];
  float sum = 0.0f;
  for (int i = 0; i < OUTPUT_SIZE; i++) {
    outArr[i] = expf(inArr[i] - maxv);
    sum += outArr[i];
  }
  for (int i = 0; i < OUTPUT_SIZE; i++) outArr[i] /= sum;
}

// PROGMEM read helpers
inline int16_t getW1(int i, int j) { return pgm_read_word_near(&W1[i][j]); }
inline int16_t getB1(int i)      { return pgm_read_word_near(&b1[i]); }
inline int16_t getW2(int i, int j) { return pgm_read_word_near(&W2[i][j]); }
inline int16_t getB2(int i)      { return pgm_read_word_near(&b2[i]); }

// Predict from 16-bit input
int predict(const int inputs[INPUT_SIZE]) {
  float x[INPUT_SIZE];
  for (int i = 0; i < INPUT_SIZE; i++) x[i] = inputs[i];

  float a1[HIDDEN_SIZE];
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float acc = (float)getB1(i);
    for (int j = 0; j < INPUT_SIZE; j++) acc += x[j] * getW1(j,i);
    a1[i] = sigmoid(acc / 1000.0f);
  }

  float out[OUTPUT_SIZE];
  for (int i = 0; i < OUTPUT_SIZE; i++) {
    float acc = (float)getB2(i);
    for (int j = 0; j < HIDDEN_SIZE; j++) acc += a1[j] * getW2(j,i);
    out[i] = acc / 1000.0f;
  }

  float probs[OUTPUT_SIZE];
  softmax(out, probs);
  int best = 0;
  for (int i = 1; i < OUTPUT_SIZE; i++) if (probs[i] > probs[best]) best = i;
  return best;
}

int inputsArr[INPUT_SIZE];
int idx = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Enter 16 bits");
  display.display();
}

void loop() {
  char k = customKeypad.getKey();

  // 1) Handle insertion or deletion
  if (k == '0' || k == '1') {
    if (idx < INPUT_SIZE) inputsArr[idx++] = k - '0';
  }
  else if (k == '#') {
    if (idx > 0) idx--;        // step back one
  }
  else {
    return;                    // ignore everything else
  }

  // 2) Redraw on any 0/1/D
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      int i = row*4 + col;
      if (i < idx) display.print(inputsArr[i]);
      else display.print(' ');
      display.print(' ');
    }
    display.println();
  }

  // 3) If full, predict
  if (idx == INPUT_SIZE) {
    int p = predict(inputsArr);
    display.setTextSize(1);
    display.setCursor(60, 0);
    display.print("Prediction:");
    display.setCursor(80, 15);
    display.setTextSize(2);
    display.print(p);
    idx = 0;
  }

  display.display();
}


