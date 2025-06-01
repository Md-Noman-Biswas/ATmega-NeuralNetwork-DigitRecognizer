# ATmega Neural Network Digit Recognizer

This project implements a simple feedforward **neural network in pure C**, optimized to run on an **ATmega328P microcontroller**. The goal is to recognize binary-encoded digits from a 4x4 keypad input and display the result on an LCD.

> 🔧 **Trained in Python. Implemented in C. Runs on 8-bit ATmega.**

---

##  Project Overview

- 🎓 **Course:** Microprocessor 
- 🎯 **Goal:** Build a digit recognizer that runs independently on an ATmega MCU  
- 💡 **Main Challenge:** Implementing a neural network **without any libraries** in **pure C**

---

## Features

- ✨ Pure C neural network inference
- 🔢 Takes 4x4 binary grid input (keypad or simulated)
- 📟 Displays recognized digit on LCD
- 📦 Trained weights stored in EEPROM
- 🧮 Manual matrix multiplication and activation functions
- 💾 Compact and optimized for microcontroller limitations

---

##  Neural Network Architecture

- **Input layer:** 16 inputs (for 4x4 binary pattern)  
- **Hidden layer:** 1 layer with 6 neurons  
- **Output layer:** 10 neurons (digits 0–9)

- **Activation functions:** 
  - Sigmoid for the hidden layer  
  - Softmax for the output layer  
- **Forward pass:** Implemented in Python with NumPy; includes fixed-point weight export for C implementation (scaled by 1000, stored as int16_t)

---

##  Workflow

1. **Data Preparation (Python)**  
   Prepare binary digit representations as training data.

2. **Model Training (Python)**  
   Train a simple neural network using NumPy or other basic tools.

3. **Weight Export**  
   Save trained weights as C arrays or EEPROM byte data.

4. **C Implementation (ATmega)**  
   Write all inference logic in C: matrix multiply, activation, max-index prediction.

5. **Deployment**  
   Flash code to ATmega328P, feed binary input via keypad, and show prediction on LCD.

---

## 📸 Demo

<img src="https://github.com/user-attachments/assets/9543724c-95ba-4bef-9935-d543d10ac0f3" width="300"/>  
<img src="https://github.com/user-attachments/assets/5f518879-daef-46db-baa1-51efd7e7bea2" width="300"/>  
<img src="https://github.com/user-attachments/assets/5ef61d38-8e8a-47c6-bbc2-4532ee1977a2" width="300"/>

---

## 🗂️ Repository Structure

```bash
📦 ATmega-NeuralNetwork-DigitRecognizer
├── Neural Network in C.cpp        # Core neural network inference in C
├── test.cpp                       # Optional testing simulation in C++
├── Binary pattern recognition.ipynb  # Python notebook for training
├── CheapNeurons_*.docx           # Project documentation
├── .vscode/                      # VSCode settings
├── .cph/ / .ipynb_checkpoints/   # Temp files (can be ignored)
