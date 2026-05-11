# 🏋️‍♂️ Gym Membership System (GMS) with Qt GUI

> A comprehensive **C++ desktop application with a modern Qt-based GUI** for managing gym memberships, payments, classes, trainers, attendance, and equipment.

---

## 📌 Project Overview

The **Gym Membership System (GMS)** is a semester project developed for the **Software Engineering (AI-131)** course at UET Lahore.

This project is built entirely in **C++** and uses the **Qt Framework** to provide a professional graphical user interface (GUI). It automates the core operations of a fitness center, including:

- Member registration and profile management
- Membership plans and renewals
- Payment processing with discount codes
- Fitness class scheduling and booking
- Attendance tracking
- Trainer management
- Equipment maintenance
- Financial and attendance reports
- Audit trail logging

---

# 🖥️ Technology Stack

| Technology | Purpose |
|----------|----------|
| C++17 | Core programming language |
| Qt Widgets | Graphical User Interface (GUI) |
| Qt Designer | Drag-and-drop UI design |
| MinGW / MSVC | C++ Compiler |
| STL (`vector`, `map`) | In-memory data storage |
| `std::hash` | Password hashing |
| Qt Network *(optional)* | Web requests / scraping |

---

# ✨ Key Features

## 👥 Member Management
- Register new members
- Secure login
- View and update profiles
- Cancel memberships
- Emergency contact management

## 📋 Membership Plan Management
- Monthly, Quarterly, and Annual plans
- Create and edit plans
- Activate/Deactivate plans
- Auto-renew support

## 💳 Payment Management
- Cash, Card, and Bank Transfer
- Transaction references
- Discount codes:
  - `WELCOME10`
  - `SUMMER20`
  - `STUDENT15`
- Payment history

## 🏃 Fitness Class Scheduling
- Create classes
- Assign trainers
- Book and cancel classes
- Capacity control

## 🏋️ Trainer Management
- Register trainers
- View schedules
- View enrolled members
- Update progress notes

## 📅 Attendance Management
- Check-In / Check-Out
- Attendance history
- Attendance reports

## 🛠 Equipment Management
- Add equipment
- Track condition
- Maintenance alerts

## 📊 Reports & Analytics
- Financial reports
- Attendance reports
- Audit trail
- Enrollment summaries

---

# 👤 User Roles

## 👨‍💼 Administrator
- Manage members
- Manage plans
- Manage trainers
- Manage classes
- Manage equipment
- Generate reports
- View audit trail

## 🏋️ Trainer
- View assigned schedule
- View enrolled members
- Update progress notes

## 🙋 Member
- Register and login
- Enroll in plans
- Renew memberships
- View payments
- Book classes
- Track attendance

---

# 🏗️ System Architecture

```
Qt GUI (Windows & Forms)
        ↓
C++ Business Logic
        ↓
STL Data Structures
        ↓
Reports & Audit Logs
```

---

# 🧱 Core Data Structures

The project models real-world entities using custom `struct`s:

- `User`
- `Admin`
- `Member`
- `Trainer`
- `MembershipPlan`
- `Membership`
- `Payment`
- `FitnessClass`
- `Booking`
- `Attendance`
- `Equipment`
- `AuditEntry`

---

# 📂 Project Structure

```text
GymMembershipSystem/
├── main.cpp
├── GymMembershipSystem.cpp
├── GymMembershipSystem.h
├── loginwindow.cpp
├── loginwindow.h
├── loginwindow.ui
├── admindashboard.cpp
├── admindashboard.h
├── admindashboard.ui
├── memberdashboard.cpp
├── memberdashboard.h
├── memberdashboard.ui
├── trainerdashboard.cpp
├── trainerdashboard.h
├── trainerdashboard.ui
├── resources/
│   ├── icons/
│   └── images/
└── README.md
```

---

# 🧠 Functional Requirement Coverage

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

# 🎨 GUI Screens

- 🔐 Login Window
- 📝 Registration Form
- 👨‍💼 Admin Dashboard
- 🙋 Member Dashboard
- 🏋️ Trainer Dashboard
- 💳 Payment History
- 📋 Membership Plans
- 📅 Class Booking
- 📊 Reports
- 🛠 Equipment Management

---

# 🔐 Demo Login Credentials

| Role | Email | Password |
|------|------|------|
| Admin | `admin@gym.com` | `Admin@123` |
| Trainer | `sara@gym.com` | `Sara@123` |
| Member | `usman@email.com` | `Usman@123` |

---

# 🎁 Supported Discount Codes

| Code | Discount |
|------|----------|
| `WELCOME10` | 10% |
| `SUMMER20` | 20% |
| `STUDENT15` | 15% |

---

# 🌐 Web Scraping / API Integration (Optional)

To satisfy advanced course requirements, the system can be extended using:

- **Qt Network**
- **libcurl**
- **JSON parsing**

Possible integrations:
- Currency exchange rates
- Weather data
- Fitness tips from public APIs

---

# 🛠️ Build Instructions

## Using Qt Creator
1. Install Qt and Qt Creator.
2. Open the project (`.pro` or `CMakeLists.txt`).
3. Select MinGW or MSVC.
4. Click **Build** and **Run**.

## Using CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

# 📊 Reports Available

- 💰 Revenue Summary
- 📅 Attendance Report
- 👥 Member Statistics
- 🏃 Class Enrollment Report
- 🛠 Maintenance Alerts
- 📜 Audit Trail

---

# 🛡 Security Features

- Password hashing using `std::hash`
- Role-based access control
- Input validation
- Session management

> ⚠️ Password hashing is included for educational purposes and is not intended for production use.

---

# 🧪 Test Scenarios

1. Register a new member
2. Login as member
3. Enroll in a membership plan
4. Apply a discount code
5. Make payment
6. Book a class
7. Check in and check out
8. Login as admin and generate reports
9. Login as trainer and update notes

---

# 🚀 Future Enhancements

- Persistent storage with files or SQLite
- Email notifications
- Charts and analytics
- PDF invoice generation
- Dark mode theme
- Online membership portal

---

# 📸 Suggested Screenshots

Create a `screenshots/` folder and add images such as:

```markdown
![Login Screen](screenshots/login.png)
![Admin Dashboard](screenshots/admin-dashboard.png)
![Member Portal](screenshots/member-dashboard.png)
```

---

# 🎓 Academic Information

- **Course:** Software Engineering (AI-131)
- **Department:** Artificial Intelligence
- **University:** University of Engineering and Technology (UET) Lahore
- **Semester:** Spring 2026

---

# 👨‍💻 Author

**Minahel Javed**  
BS Artificial Intelligence Student

---

# ⭐ Project Highlights

- ✅ Fully developed in C++
- ✅ Professional Qt GUI
- ✅ Role-based dashboards
- ✅ Covers all functional requirements
- ✅ Modular and scalable design
- ✅ Web/API integration ready
- ✅ Strong portfolio project

---

# 📜 License

This project is developed for academic and educational purposes.

---

# 🌟 Support

If you found this project helpful, please give it a ⭐ on GitHub!

---

> “Transforming raw C++ logic into a complete desktop application with Qt.” 
 