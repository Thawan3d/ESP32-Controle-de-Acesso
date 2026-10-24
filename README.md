# 🔐 ESP32 Access Control System

An educational embedded systems project developed using the **ESP32, Wokwi simulator, and Arduino IDE**.

This project simulates an electronic access control system with password verification, LCD messages, LED indicators, temporary lockouts, and permanent lockout logic.

## 🚀 Features

- 🔑 Password verification using four buttons.
- 📟 LCD display for system status and feedback.
- 🟢 LED indicator for simulated door access.
- 🔴 LED indicator for incorrect passwords and lockout status.
- ⏱️ Temporary lockout after three incorrect passwords.
- 🚨 Permanent lockout after three temporary lockouts.
- 🔓 Manual reset of the permanent lockout using an authorization button.
- ⏳ Automatic closing of the simulated door after five seconds.
- 🔒 Password input feedback using asterisks on the LCD.

## 🧰 Technologies

- ESP32
- Wokwi Simulator
- Arduino IDE
- C/C++
- Digital electronics
- GPIO input and output control
- Embedded systems and control logic

## 🧪 Simulation and Testing

The circuit was developed and tested using the **Wokwi online simulator** because the physical components required for the complete assembly are not currently available.

The simulation allows the program logic, button inputs, LCD interface, LED indicators, and access-control states to be tested in a virtual environment.

**[▶ Run the ESP32 Access Control Simulation on Wokwi](https://wokwi.com/projects/478782224874054657)**

## ⚙️ How It Works

1. The user enters a four-button password.
2. If the password is correct, the access indicator turns on for five seconds.
3. If the password is incorrect, the system counts the failed password entries.
4. After three incorrect passwords, the system enters a temporary lockout.
5. After three temporary lockouts, the system enters a permanent lockout.
6. During the permanent lockout, the red LED flashes until the authorization button is pressed.

## 🎯 Learning Objectives

- Practice embedded C/C++ programming.
- Implement password verification and error counters.
- Control digital inputs and outputs.
- Use timers with the `millis()` function.
- Implement temporary and permanent lockout states.
- Interface an LCD with a microcontroller.
- Develop basic access-control logic.

## ⚠️ Disclaimer

This project is an educational simulation and has not yet been validated as a complete physical prototype. It is intended for learning and experimentation, not for real-world security applications.

## 👨‍💻 Author

**Thawan Eduardo**

- GitHub: [@Thawan3d](https://github.com/Thawan3d)
- LinkedIn: [Thawan Eduardo](https://www.linkedin.com/in/thawan-eduardo-a860523b2/)

---

*Developed as part of my learning journey in Electrical Engineering, Electronics, and Embedded Systems.* ⚡
