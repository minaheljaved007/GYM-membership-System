# 🏋️‍♂️ Gym Membership System (GMS)

> A comprehensive **C++ Console-Based Gym Management System** developed for the **Software Engineering (AI-131)** course at **UET Lahore – Spring 2026**.

---

## 📌 Project Overview

The **Gym Membership System (GMS)** is a fully functional, menu-driven C++ application that simulates the operations of a modern fitness center.

It supports three major roles:

- 👨‍💼 **Administrator**
- 🏋️ **Trainer**
- 🙋 **Member**

The system provides complete functionality for:

- Member registration and profile management
- Membership plans and renewals
- Payment processing with discount codes
- Fitness class booking and cancellation
- Attendance tracking (Check-In / Check-Out)
- Trainer management
- Equipment maintenance
- Financial and attendance reports
- Audit trail logging

This project is implemented entirely in a **single C++ file** using:

- Structures (`struct`)
- Inheritance
- Enumerations (`enum class`)
- Vectors (`std::vector`)
- Maps (`std::map`)
- File-like in-memory database simulation
- Modular function-based design

---

# ✨ Features

## 👥 Member Management
- Register new members
- Login securely using hashed passwords
- View and update profiles
- Activate, suspend, or cancel memberships
- Emergency contact management

## 📋 Membership Plan Management
- Monthly, Quarterly, and Annual plans
- Create new plans
- Edit prices
- Activate/Deactivate plans
- Auto-renew memberships

## 💳 Payment Management
- Cash, Card, and Bank Transfer
- Transaction references
- Discount codes:
  - `WELCOME10`
  - `SUMMER20`
  - `STUDENT15`
- Payment history

## 🏃 Fitness Class Scheduling
- Create and cancel classes
- Assign trainers
- Book and cancel classes
- Capacity control
- Enrollment tracking

## 🏋️ Trainer Management
- Register trainers
- View assigned schedules
- View enrolled members
- Update progress notes

## 📅 Attendance Management
- Member check-in/check-out
- Attendance history
- Attendance reports

## 🛠 Equipment Management
- Add equipment
- Track condition
- Maintenance alerts
- Repair notifications

## 📊 Reports & Analytics
- Financial summaries
- Attendance reports
- Audit trail
- Payment statistics

---

# 🧠 Functional Requirement Coverage

This project implements all required modules:

| Module | Functional Requirement IDs |
|------|------|
| Member Management | FR-MM-01 → FR-MM-07 |
| Membership Plan Management | FR-MP-01 → FR-MP-08 |
| Payment Management | FR-PM-01 → FR-PM-08 |
| Class Scheduling | FR-CS-01 → FR-CS-07 |
| Trainer Management | FR-TR-01 → FR-TR-05 |
| Attendance Management | FR-AT-01 → FR-AT-03 |
| Equipment Management | FR-EQ-01 → FR-EQ-04 |

---

# 🏗️ Technologies Used

| Technology | Purpose |
|---------|---------|
| C++17 | Core programming language |
| STL Vectors | Dynamic storage |
| STL Maps | Discount code lookup |
| Structs | Data modeling |
| Enum Classes | Strongly typed statuses |
| Hash Functions | Password hashing |
| Console UI | User interaction |

---

# 📂 Project Structure

```text
GymMembershipSystem.cpp
README.md
