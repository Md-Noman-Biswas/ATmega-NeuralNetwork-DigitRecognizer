# 🧠 ATmega Neural Network Digit Recognizer

This project implements a simple feedforward **neural network in pure C**, optimized to run on an **ATmega328P microcontroller**. The goal is to recognize binary-encoded digits from a 4x4 keypad input and display the result on an LCD.

> 🔧 **Trained in Python. Implemented in C. Runs on 8-bit ATmega.**

---

## 📌 Project Overview

- 🎓 **Course:** Microprocessor System Design  
- 🎯 **Goal:** Build a digit recognizer that runs independently on an ATmega MCU  
- 💡 **Main Challenge:** Implementing a neural network **without any libraries** in **pure C**

---

## 🚀 Features

- ✨ Pure C neural network inference
- 🔢 Takes 4x4 binary grid input (keypad or simulated)
- 📟 Displays recognized digit on LCD
- 📦 Trained weights stored in EEPROM
- 🧮 Manual matrix multiplication and activation functions
- 💾 Compact and optimized for microcontroller limitations

---

## 🧠 Neural Network Architecture

- **Input layer:** 16 inputs (for 4x4 binary pattern)  
- **Hidden layer:** 1 layer (customizable size)  
- **Output layer:** 10 neurons (digits 0–9)

- **Activation functions:** Step / ReLU / Manual max selection  
- **Forward pass:** Fully written in C with fixed-point style logic (if needed)

---

## 🧪 Workflow

1. **Data Preparation (Python)**  
   Prepare binary digit representations as training data.

2. **Model Training (Python)**  
   Train a simple neural network using NumPy or other basic tools.

3. **Weight Export**  
   Save trained weights as C arrays or EEPROM byte data.

4. **C Implementation (ATmega)**  
   Write all inference logic in C: matrix multiply, activation, max-index prediction.

5. **Deployment**  
   Flash code to ATmega32P, feed binary input via keypad, and show prediction on LCD.

---

## 📸 Demo 

![image](https://github.com/user-attachments/assets/4a3c3b33-0d9c-42f5-a9f1-a53a45659e2b)
![image](https://github.com/user-attachments/assets/5f518879-daef-46db-baa1-51efd7e7bea2)
![image](https://github.com/user-attachments/assets/5ef61d38-8e8a-47c6-bbc2-4532ee1977a2)




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
