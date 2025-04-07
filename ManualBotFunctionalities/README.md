# RC-Controlled Manual Robot

This project is an **RC-controlled manual bot** that operates using three separate systems:

1. **Drive System**  
2. **Arm & Gripper System**  
3. **Intake & Conveyor System**

Each module is independently powered and controlled using RC signals from a transmitter (e.g., FlySky i6), read using `pulseIn()` on the Arduino.

---

## 1. 🚗 Drive System

**Microcontroller:** Arduino Mega  
**Motors Used:** 4 x DC Motors with L298N motor driver  
**Control Inputs:**  
- **Throttle (CH2)** — forward/reverse  
- **Steering (CH1)** — left/right  
- **Relay (CH7 or CH5)** — switches a high-power relay for additional power mode

**Wiring Summary:**
- CH1 (A8): Steering input
- CH2 (A9): Throttle input
- CH7 (A10): Relay control
- L298N Motor Pins:
  - Left Motor: `EN1`, `IN1`, `IN2`
  - Right Motor: `EN2`, `IN3`, `IN4`

**Functionality:**
- Smooth analog-like driving via RC stick inputs.
- Relay toggles a different motor mode when required.

---

## 2. 🤖 Arm & Gripper System

**Microcontroller:** Arduino Mega  
**Motors Used:**
- 4 x Stepper Motors (for arm movement)
- 2 x Servo (for grippers)

**Control Inputs:**  
- **CH9 (A9):** Vertical movement  
- **CH10 (A8):** Horizontal movement  
- **CH7 (A10):** Gripper open/close control

**Libraries Used:**
- `MobaTools` — for precise stepper control
- `Servo` — for gripper movement

**Stepper Specs:**
- 800 steps/rev
- Horizontal and vertical speeds set via `MoToStepper`

**Functionality:**
- Smooth stepper-based 2-axis arm
- Servo gripper with 3 states: open, mid, closed (0°, 90°, 180°)

---

## 3. 🎯 Intake & Conveyor System

**Microcontroller:** Arduino Mega  
**Motors Used:**
- 1 x Stepper (for ball intake)
- 1 x DC Motor (for secondary intake)
- 2 x DC Motor (for belt conveyor and throw)

**Control Inputs:**
- **CH8 (A13):** Belt/relay toggle
- **CH5 (A11):** Main intake control
- **CH6 (A12):** Secondary DC intake motor

**Functionality:**
- Intake stepper runs in different directions based on switch position.
- DC motor speeds adjusted using PWM for conveyor/intake.
- Relay engaged when belt is off.

---

## 🛠️ Setup Instructions

1. **Connect each Arduino to its respective motor drivers and RC channels.**
2. **Upload the corresponding sketch:**
   - `DriveCodes (1).ino` → Drive controller
   - `mainMech.ino` → Arm and gripper controller + Intake and conveyor controller
3. **Power each module appropriately.**
4. **Use an RC transmitter to control the bot manually.**

---

## 📦 Required Libraries

- [MobaTools](https://github.com/MicroBahner/MobaTools)
- Servo (included with Arduino IDE)

Install them via **Arduino Library Manager**.

---

## 🎮 Transmitter Channel Mapping

| Function         | Channel | Pin on Arduino | Notes                 |
|------------------|---------|----------------|------------------------|
| Throttle         | CH2     | A9             | Forward/backward       |
| Steering         | CH1     | A8             | Left/right             |
| Relay toggle     | CH7     | A10            | For belt or high-power |
| Arm Vertical     | CH9     | A9             | Up/down arm            |
| Arm Horizontal   | CH10    | A8             | Horizontal arm move    |
| Gripper          | CH7     | A10            | Servo control          |
| Belt Relay       | CH8     | A13            | Belt activation        |
| Intake Stepper   | CH5     | A11            | Main intake control    |
| Intake DC Motor  | CH6     | A12            | Second motor for intake|

---

## 👤 Author

Designed and built by Aman Shaikh
With support from Kevin Jimmy, Shubham Prasad, Sanket Jain, Nayan Bhoyar, Yash Bhosale.

---
