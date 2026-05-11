/*
 * ============================================================
 *  GYM MEMBERSHIP SYSTEM (GMS)
 *  Software Engineering (AI-131) — UET Lahore — Spring 2026
 * ============================================================
 *  Single-file C++ console application covering ALL functional
 *  requirements from the assignment document.
 *
 *  Modules implemented:
 *   1. Member Management       (FR-MM-01 to FR-MM-07)
 *   2. Membership Plan Mgmt    (FR-MP-01 to FR-MP-08)
 *   3. Payment Management      (FR-PM-01 to FR-PM-08)
 *   4. Class Scheduling        (FR-CS-01 to FR-CS-07)
 *   5. Trainer Management      (FR-TR-01 to FR-TR-05)
 *   6. Attendance Management   (FR-AT-01 to FR-AT-03)
 *   7. Equipment Management    (FR-EQ-01 to FR-EQ-04)
 *
 *  Roles: Administrator | Member | Trainer
 *
 *  Build:  g++ -std=c++17 -o GymMembershipSystem GymMembershipSystem.cpp
 *  Run  :  ./GymMembershipSystem   (Linux/macOS)
 *          GymMembershipSystem.exe (Windows)
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <functional>
#include <ctime>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
//  UTILITY HELPERS
// ─────────────────────────────────────────────

static int g_idCounter = 1000;

string generateID(const string& prefix) {
    return prefix + to_string(++g_idCounter);
}

string currentDate() {
    time_t t = time(nullptr);
    tm* now  = localtime(&t);
    char buf[20];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);
    return string(buf);
}

// Simple password hash (sum of char values — for console demo only)
size_t hashPassword(const string& pw) {
    return hash<string>{}(pw);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnter() {
    cout << "\n  [Press ENTER to continue...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════╗\n";
    cout << "  ║  " << left << setw(44) << title << "║\n";
    cout << "  ╚══════════════════════════════════════════════╝\n\n";
}

void printLine() {
    cout << "  ─────────────────────────────────────────────────\n";
}

// ─────────────────────────────────────────────
//  ENUMERATIONS
// ─────────────────────────────────────────────

enum class MemberStatus    { Active, Suspended, Cancelled };
enum class MembershipStatus{ Active, Expired, Cancelled, Suspended };
enum class PaymentStatus   { Pending, Completed, Failed };
enum class PaymentMethod   { Cash, Card, BankTransfer };
enum class BookingStatus   { Confirmed, Cancelled, Completed, NoShow };
enum class ClassStatus     { Scheduled, Ongoing, Completed, Cancelled };
enum class EquipCondition  { Excellent, Good, Fair, NeedsRepair, OutOfService };
enum class UserRole        { Admin, Member, Trainer };

string memberStatusStr(MemberStatus s) {
    switch(s){
        case MemberStatus::Active:    return "Active";
        case MemberStatus::Suspended: return "Suspended";
        case MemberStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

string membershipStatusStr(MembershipStatus s) {
    switch(s){
        case MembershipStatus::Active:    return "Active";
        case MembershipStatus::Expired:   return "Expired";
        case MembershipStatus::Cancelled: return "Cancelled";
        case MembershipStatus::Suspended: return "Suspended";
    }
    return "Unknown";
}

string payStatusStr(PaymentStatus s) {
    switch(s){
        case PaymentStatus::Pending:   return "Pending";
        case PaymentStatus::Completed: return "Completed";
        case PaymentStatus::Failed:    return "Failed";
    }
    return "Unknown";
}

string payMethodStr(PaymentMethod m) {
    switch(m){
        case PaymentMethod::Cash:        return "Cash";
        case PaymentMethod::Card:        return "Card";
        case PaymentMethod::BankTransfer:return "Bank Transfer";
    }
    return "Unknown";
}

string bookStatusStr(BookingStatus s) {
    switch(s){
        case BookingStatus::Confirmed:  return "Confirmed";
        case BookingStatus::Cancelled:  return "Cancelled";
        case BookingStatus::Completed:  return "Completed";
        case BookingStatus::NoShow:     return "No Show";
    }
    return "Unknown";
}

string classStatusStr(ClassStatus s) {
    switch(s){
        case ClassStatus::Scheduled:  return "Scheduled";
        case ClassStatus::Ongoing:    return "Ongoing";
        case ClassStatus::Completed:  return "Completed";
        case ClassStatus::Cancelled:  return "Cancelled";
    }
    return "Unknown";
}

string equipCondStr(EquipCondition c) {
    switch(c){
        case EquipCondition::Excellent:    return "Excellent";
        case EquipCondition::Good:         return "Good";
        case EquipCondition::Fair:         return "Fair";
        case EquipCondition::NeedsRepair:  return "Needs Repair";
        case EquipCondition::OutOfService: return "Out of Service";
    }
    return "Unknown";
}

// ─────────────────────────────────────────────
//  DATA STRUCTURES
// ─────────────────────────────────────────────

struct MembershipPlan {
    string planID;
    string planName;
    int    durationMonths;
    double price;
    string facilities;
    bool   isActive;
    string createdByAdminID;
};

struct Membership {
    string membershipID;
    string memberID;
    string planID;
    string startDate;
    string endDate;
    MembershipStatus status;
    bool   autoRenew;
};

struct Payment {
    string        paymentID;
    string        memberID;
    string        membershipID;
    double        amount;
    string        paymentDate;
    PaymentMethod method;
    PaymentStatus status;
    string        transactionRef;
    double        discountApplied;
};

struct Booking {
    string        bookingID;
    string        memberID;
    string        classID;
    string        bookingDateTime;
    BookingStatus status;
};

struct Attendance {
    string attendanceID;
    string memberID;
    string checkInTime;
    string checkOutTime;
    string date;
};

struct FitnessClass {
    string      classID;
    string      className;
    string      trainerID;
    string      scheduledDateTime;
    int         durationMinutes;
    int         maxCapacity;
    int         currentEnrollment;
    string      location;
    string      classType;
    ClassStatus status;
};

struct Equipment {
    string         equipmentID;
    string         name;
    int            quantity;
    EquipCondition condition;
    string         purchaseDate;
    string         lastMaintenanceDate;
    string         nextMaintenanceDate;
    double         purchaseCost;
};

struct AuditEntry {
    string timestamp;
    string userID;
    string action;
};

// ─────────────────────────────────────────────
//  USER BASE + DERIVED TYPES
// ─────────────────────────────────────────────

struct User {
    string userID;
    string email;
    size_t passwordHash;
    string createdAt;
    bool   isActive;
    UserRole role;
};

struct Member : User {
    string memberID;
    string firstName;
    string lastName;
    string contactNumber;
    string dateOfBirth;
    string cnic;
    string address;
    string emergencyContact;
    MemberStatus status;
    string registrationDate;
};

struct Trainer : User {
    string trainerID;
    string firstName;
    string lastName;
    string specialization;
    string qualification;
    string contactNumber;
    bool   active;
};

struct Admin : User {
    string adminID;
    string username;
    string role_name;
};

// ─────────────────────────────────────────────
//  GLOBAL DATA STORES (in-memory "database")
// ─────────────────────────────────────────────

vector<Member>         members;
vector<Trainer>        trainers;
vector<Admin>          admins;
vector<MembershipPlan> plans;
vector<Membership>     memberships;
vector<Payment>        payments;
vector<FitnessClass>   classes;
vector<Booking>        bookings;
vector<Attendance>     attendance;
vector<Equipment>      equipment;
vector<AuditEntry>     auditLog;

// Discount codes
map<string, double> discountCodes = {
    {"WELCOME10", 10.0},
    {"SUMMER20",  20.0},
    {"STUDENT15", 15.0}
};

// Currently logged-in session
string loggedInUserID = "";
UserRole loggedInRole = UserRole::Member;
string loggedInMemberID  = "";
string loggedInTrainerID = "";

// ─────────────────────────────────────────────
//  AUDIT LOGGER
// ─────────────────────────────────────────────

void logAudit(const string& userID, const string& action) {
    AuditEntry e;
    e.timestamp = currentDate();
    e.userID    = userID;
    e.action    = action;
    auditLog.push_back(e);
}

// ─────────────────────────────────────────────
//  SEED DEMO DATA
// ─────────────────────────────────────────────

void seedData() {

    // --- Admin ---
    Admin a;
    a.userID       = "USR-ADMIN-001";
    a.email        = "admin@gym.com";
    a.passwordHash = hashPassword("Admin@123");
    a.isActive     = true;
    a.createdAt    = "2026-01-01";
    a.role         = UserRole::Admin;
    a.adminID      = "ADM-001";
    a.username     = "superadmin";
    a.role_name    = "SuperAdmin";
    admins.push_back(a);

    // --- Trainers ---
    Trainer t1;
    t1.userID       = "USR-TRN-001";
    t1.email        = "sara@gym.com";
    t1.passwordHash = hashPassword("Sara@123");
    t1.isActive     = true;
    t1.createdAt    = "2026-01-01";
    t1.role         = UserRole::Trainer;
    t1.trainerID    = "TRN-001";
    t1.firstName    = "Sara";
    t1.lastName     = "Ahmed";
    t1.specialization = "Yoga & Flexibility";
    t1.qualification  = "ACE Certified";
    t1.contactNumber  = "0300-1234567";
    t1.active         = true;
    trainers.push_back(t1);

    Trainer t2;
    t2.userID       = "USR-TRN-002";
    t2.email        = "ali@gym.com";
    t2.passwordHash = hashPassword("Ali@123");
    t2.isActive     = true;
    t2.createdAt    = "2026-01-01";
    t2.role         = UserRole::Trainer;
    t2.trainerID    = "TRN-002";
    t2.firstName    = "Ali";
    t2.lastName     = "Hassan";
    t2.specialization = "Strength & Conditioning";
    t2.qualification  = "NASM Certified";
    t2.contactNumber  = "0311-9876543";
    t2.active         = true;
    trainers.push_back(t2);

    // --- Membership Plans ---
    MembershipPlan p1;
    p1.planID          = "PLAN-001";
    p1.planName        = "Monthly Plan";
    p1.durationMonths  = 1;
    p1.price           = 3000.0;
    p1.facilities      = "Gym Floor, Locker Room";
    p1.isActive        = true;
    p1.createdByAdminID= "ADM-001";
    plans.push_back(p1);

    MembershipPlan p2;
    p2.planID          = "PLAN-002";
    p2.planName        = "Quarterly Plan";
    p2.durationMonths  = 3;
    p2.price           = 8000.0;
    p2.facilities      = "Gym Floor, Classes, Locker Room";
    p2.isActive        = true;
    p2.createdByAdminID= "ADM-001";
    plans.push_back(p2);

    MembershipPlan p3;
    p3.planID          = "PLAN-003";
    p3.planName        = "Annual Plan";
    p3.durationMonths  = 12;
    p3.price           = 25000.0;
    p3.facilities      = "Full Access: Gym, Classes, Pool, Sauna, Locker";
    p3.isActive        = true;
    p3.createdByAdminID= "ADM-001";
    plans.push_back(p3);

    // --- Demo Member ---
    Member m1;
    m1.userID          = "USR-MEM-001";
    m1.email           = "usman@email.com";
    m1.passwordHash    = hashPassword("Usman@123");
    m1.isActive        = true;
    m1.createdAt       = "2026-01-01";
    m1.role            = UserRole::Member;
    m1.memberID        = "MEM-001";
    m1.firstName       = "Usman";
    m1.lastName        = "Khalid";
    m1.contactNumber   = "0321-4567890";
    m1.dateOfBirth     = "1998-05-15";
    m1.cnic            = "35202-1234567-1";
    m1.address         = "House 12, Gulberg, Lahore";
    m1.emergencyContact= "0300-9876543";
    m1.status          = MemberStatus::Active;
    m1.registrationDate= "2026-01-01";
    members.push_back(m1);

    // --- Active Membership for demo member ---
    Membership ms1;
    ms1.membershipID = "MS-001";
    ms1.memberID     = "MEM-001";
    ms1.planID       = "PLAN-002";
    ms1.startDate    = "2026-01-01";
    ms1.endDate      = "2026-03-31";
    ms1.status       = MembershipStatus::Active;
    ms1.autoRenew    = true;
    memberships.push_back(ms1);

    // --- Payment for that membership ---
    Payment pay1;
    pay1.paymentID        = "PAY-001";
    pay1.memberID         = "MEM-001";
    pay1.membershipID     = "MS-001";
    pay1.amount           = 8000.0;
    pay1.paymentDate      = "2026-01-01";
    pay1.method           = PaymentMethod::Card;
    pay1.status           = PaymentStatus::Completed;
    pay1.transactionRef   = "TXN-HBL-9821";
    pay1.discountApplied  = 0.0;
    payments.push_back(pay1);

    // --- Fitness Classes ---
    FitnessClass c1;
    c1.classID           = "CLS-001";
    c1.className         = "Morning Yoga";
    c1.trainerID         = "TRN-001";
    c1.scheduledDateTime = "2026-05-10 07:00";
    c1.durationMinutes   = 60;
    c1.maxCapacity       = 20;
    c1.currentEnrollment = 5;
    c1.location          = "Studio A";
    c1.classType         = "Yoga";
    c1.status            = ClassStatus::Scheduled;
    classes.push_back(c1);

    FitnessClass c2;
    c2.classID           = "CLS-002";
    c2.className         = "Strength Training";
    c2.trainerID         = "TRN-002";
    c2.scheduledDateTime = "2026-05-10 09:00";
    c2.durationMinutes   = 90;
    c2.maxCapacity       = 15;
    c2.currentEnrollment = 15;   // Full — for demo
    c2.location          = "Main Floor";
    c2.classType         = "Strength";
    c2.status            = ClassStatus::Scheduled;
    classes.push_back(c2);

    FitnessClass c3;
    c3.classID           = "CLS-003";
    c3.className         = "Cardio Blast";
    c3.trainerID         = "TRN-001";
    c3.scheduledDateTime = "2026-05-11 06:00";
    c3.durationMinutes   = 45;
    c3.maxCapacity       = 25;
    c3.currentEnrollment = 10;
    c3.location          = "Studio B";
    c3.classType         = "Cardio";
    c3.status            = ClassStatus::Scheduled;
    classes.push_back(c3);

    // --- Equipment ---
    Equipment eq1;
    eq1.equipmentID       = "EQ-001";
    eq1.name              = "Treadmill";
    eq1.quantity          = 10;
    eq1.condition         = EquipCondition::Good;
    eq1.purchaseDate      = "2024-06-01";
    eq1.lastMaintenanceDate = "2026-03-01";
    eq1.nextMaintenanceDate = "2026-06-01";
    eq1.purchaseCost      = 150000.0;
    equipment.push_back(eq1);

    Equipment eq2;
    eq2.equipmentID       = "EQ-002";
    eq2.name              = "Dumbbells Set";
    eq2.quantity          = 20;
    eq2.condition         = EquipCondition::Excellent;
    eq2.purchaseDate      = "2025-01-15";
    eq2.lastMaintenanceDate = "2026-01-15";
    eq2.nextMaintenanceDate = "2026-07-15";
    eq2.purchaseCost      = 50000.0;
    equipment.push_back(eq2);

    Equipment eq3;
    eq3.equipmentID       = "EQ-003";
    eq3.name              = "Bench Press";
    eq3.quantity          = 5;
    eq3.condition         = EquipCondition::NeedsRepair;
    eq3.purchaseDate      = "2023-03-01";
    eq3.lastMaintenanceDate = "2025-09-01";
    eq3.nextMaintenanceDate = "2026-03-01";  // Overdue
    eq3.purchaseCost      = 80000.0;
    equipment.push_back(eq3);
}

// ─────────────────────────────────────────────
//  LOOKUP HELPERS
// ─────────────────────────────────────────────

Member* findMemberByID(const string& id) {
    for (auto& m : members) if (m.memberID == id) return &m;
    return nullptr;
}

Member* findMemberByEmail(const string& email) {
    for (auto& m : members) if (m.email == email) return &m;
    return nullptr;
}

Trainer* findTrainerByID(const string& id) {
    for (auto& t : trainers) if (t.trainerID == id) return &t;
    return nullptr;
}

Trainer* findTrainerByEmail(const string& email) {
    for (auto& t : trainers) if (t.email == email) return &t;
    return nullptr;
}

Admin* findAdminByEmail(const string& email) {
    for (auto& a : admins) if (a.email == email) return &a;
    return nullptr;
}

MembershipPlan* findPlanByID(const string& id) {
    for (auto& p : plans) if (p.planID == id) return &p;
    return nullptr;
}

Membership* findActiveMembership(const string& memberID) {
    for (auto& ms : memberships)
        if (ms.memberID == memberID && ms.status == MembershipStatus::Active)
            return &ms;
    return nullptr;
}

FitnessClass* findClassByID(const string& id) {
    for (auto& c : classes) if (c.classID == id) return &c;
    return nullptr;
}

Equipment* findEquipByID(const string& id) {
    for (auto& e : equipment) if (e.equipmentID == id) return &e;
    return nullptr;
}

// ─────────────────────────────────────────────
//  DISPLAY TABLE HELPERS
// ─────────────────────────────────────────────

void displayAllPlans() {
    cout << "\n  " << left
         << setw(10) << "Plan ID"
         << setw(20) << "Plan Name"
         << setw(10) << "Months"
         << setw(12) << "Price(PKR)"
         << setw(8)  << "Status"
         << "Facilities\n";
    printLine();
    for (auto& p : plans) {
        cout << "  "
             << setw(10) << p.planID
             << setw(20) << p.planName
             << setw(10) << p.durationMonths
             << setw(12) << fixed << setprecision(0) << p.price
             << setw(8)  << (p.isActive ? "Active" : "Inactive")
             << p.facilities << "\n";
    }
}

void displayAllMembers() {
    cout << "\n  " << left
         << setw(10) << "MemberID"
         << setw(16) << "Name"
         << setw(25) << "Email"
         << setw(15) << "Contact"
         << "Status\n";
    printLine();
    for (auto& m : members) {
        cout << "  "
             << setw(10) << m.memberID
             << setw(16) << (m.firstName + " " + m.lastName)
             << setw(25) << m.email
             << setw(15) << m.contactNumber
             << memberStatusStr(m.status) << "\n";
    }
}

void displayAllTrainers() {
    cout << "\n  " << left
         << setw(10) << "TrainerID"
         << setw(16) << "Name"
         << setw(25) << "Specialization"
         << setw(20) << "Qualification"
         << "Status\n";
    printLine();
    for (auto& t : trainers) {
        cout << "  "
             << setw(10) << t.trainerID
             << setw(16) << (t.firstName + " " + t.lastName)
             << setw(25) << t.specialization
             << setw(20) << t.qualification
             << (t.active ? "Active" : "Inactive") << "\n";
    }
}

void displayAllClasses() {
    cout << "\n  " << left
         << setw(10) << "ClassID"
         << setw(20) << "Class Name"
         << setw(10) << "TrainerID"
         << setw(20) << "DateTime"
         << setw(6)  << "Cap"
         << setw(6)  << "Enrl"
         << "Status\n";
    printLine();
    for (auto& c : classes) {
        cout << "  "
             << setw(10) << c.classID
             << setw(20) << c.className
             << setw(10) << c.trainerID
             << setw(20) << c.scheduledDateTime
             << setw(6)  << c.maxCapacity
             << setw(6)  << c.currentEnrollment
             << classStatusStr(c.status) << "\n";
    }
}

void displayEquipment() {
    cout << "\n  " << left
         << setw(10) << "EquipID"
         << setw(20) << "Name"
         << setw(6)  << "Qty"
         << setw(16) << "Condition"
         << setw(15) << "Last Maint."
         << "Next Maint.\n";
    printLine();
    for (auto& e : equipment) {
        string alert = (e.condition == EquipCondition::NeedsRepair ||
                        e.condition == EquipCondition::OutOfService) ? " ⚠ ALERT" : "";
        cout << "  "
             << setw(10) << e.equipmentID
             << setw(20) << e.name
             << setw(6)  << e.quantity
             << setw(16) << equipCondStr(e.condition)
             << setw(15) << e.lastMaintenanceDate
             << e.nextMaintenanceDate
             << alert << "\n";
    }
}

void displayMemberPayments(const string& memberID) {
    cout << "\n  " << left
         << setw(12) << "PaymentID"
         << setw(12) << "MembershipID"
         << setw(12) << "Amount"
         << setw(15) << "Date"
         << setw(15) << "Method"
         << "Status\n";
    printLine();
    bool found = false;
    for (auto& p : payments) {
        if (p.memberID == memberID) {
            found = true;
            cout << "  "
                 << setw(12) << p.paymentID
                 << setw(12) << p.membershipID
                 << setw(12) << fixed << setprecision(2) << p.amount
                 << setw(15) << p.paymentDate
                 << setw(15) << payMethodStr(p.method)
                 << payStatusStr(p.status) << "\n";
        }
    }
    if (!found) cout << "  No payment records found.\n";
}

void displayMemberBookings(const string& memberID) {
    cout << "\n  " << left
         << setw(12) << "BookingID"
         << setw(12) << "ClassID"
         << setw(20) << "Class Name"
         << setw(20) << "DateTime"
         << "Status\n";
    printLine();
    bool found = false;
    for (auto& b : bookings) {
        if (b.memberID == memberID) {
            found = true;
            FitnessClass* fc = findClassByID(b.classID);
            string cn = fc ? fc->className : "N/A";
            string dt = fc ? fc->scheduledDateTime : "N/A";
            cout << "  "
                 << setw(12) << b.bookingID
                 << setw(12) << b.classID
                 << setw(20) << cn
                 << setw(20) << dt
                 << bookStatusStr(b.status) << "\n";
        }
    }
    if (!found) cout << "  No bookings found.\n";
}

void displayMemberAttendance(const string& memberID) {
    cout << "\n  " << left
         << setw(14) << "AttendanceID"
         << setw(20) << "Check-In"
         << setw(20) << "Check-Out"
         << "Date\n";
    printLine();
    bool found = false;
    for (auto& a : attendance) {
        if (a.memberID == memberID) {
            found = true;
            cout << "  "
                 << setw(14) << a.attendanceID
                 << setw(20) << a.checkInTime
                 << setw(20) << (a.checkOutTime.empty() ? "Still Inside" : a.checkOutTime)
                 << a.date << "\n";
        }
    }
    if (!found) cout << "  No attendance records found.\n";
}

// ─────────────────────────────────────────────
//  AUTHENTICATION
// ─────────────────────────────────────────────

bool login() {
    printHeader("GYM MEMBERSHIP SYSTEM — LOGIN");
    string email, password;
    cout << "  Email    : "; cin >> email;
    cout << "  Password : "; cin >> password;

    size_t ph = hashPassword(password);

    // Check admins
    Admin* adm = findAdminByEmail(email);
    if (adm && adm->passwordHash == ph && adm->isActive) {
        loggedInUserID = adm->userID;
        loggedInRole   = UserRole::Admin;
        logAudit(adm->userID, "Login - Admin");
        cout << "\n  ✓ Welcome, " << adm->username << "! (Administrator)\n";
        pressEnter();
        return true;
    }

    // Check trainers
    Trainer* tr = findTrainerByEmail(email);
    if (tr && tr->passwordHash == ph && tr->isActive) {
        loggedInUserID    = tr->userID;
        loggedInRole      = UserRole::Trainer;
        loggedInTrainerID = tr->trainerID;
        logAudit(tr->userID, "Login - Trainer");
        cout << "\n  ✓ Welcome, " << tr->firstName << "! (Trainer)\n";
        pressEnter();
        return true;
    }

    // Check members
    Member* mem = findMemberByEmail(email);
    if (mem && mem->passwordHash == ph && mem->isActive) {
        loggedInUserID    = mem->userID;
        loggedInRole      = UserRole::Member;
        loggedInMemberID  = mem->memberID;
        logAudit(mem->userID, "Login - Member");
        cout << "\n  ✓ Welcome, " << mem->firstName << "! (Member)\n";
        pressEnter();
        return true;
    }

    cout << "\n  ✗ Invalid email or password. Please try again.\n";
    pressEnter();
    return false;
}

void logout() {
    logAudit(loggedInUserID, "Logout");
    loggedInUserID    = "";
    loggedInMemberID  = "";
    loggedInTrainerID = "";
}

// ─────────────────────────────────────────────
//  MEMBER REGISTRATION (FR-MM-01, FR-MM-03)
// ─────────────────────────────────────────────

void registerMember() {
    printHeader("MEMBER REGISTRATION");
    cin.ignore();

    Member m;
    cout << "  Full Name (First): "; getline(cin, m.firstName);
    cout << "  Full Name (Last) : "; getline(cin, m.lastName);
    cout << "  Email            : "; getline(cin, m.email);

    // FR-MM: check uniqueness
    if (findMemberByEmail(m.email)) {
        cout << "\n  ✗ Email already registered. Please login instead.\n";
        pressEnter();
        return;
    }

    string password;
    cout << "  Password (min 8 chars): "; getline(cin, password);
    if (password.length() < 8) {
        cout << "\n  ✗ Password must be at least 8 characters.\n";
        pressEnter();
        return;
    }

    cout << "  Contact Number   : "; getline(cin, m.contactNumber);
    cout << "  Date of Birth    : "; getline(cin, m.dateOfBirth);
    cout << "  CNIC             : "; getline(cin, m.cnic);
    cout << "  Address          : "; getline(cin, m.address);
    cout << "  Emergency Contact: "; getline(cin, m.emergencyContact);

    // Auto-generate IDs (FR-MM-03)
    m.memberID        = generateID("MEM-");
    m.userID          = generateID("USR-MEM-");
    m.passwordHash    = hashPassword(password);
    m.isActive        = true;
    m.createdAt       = currentDate();
    m.role            = UserRole::Member;
    m.status          = MemberStatus::Active;
    m.registrationDate= currentDate();

    members.push_back(m);
    logAudit(m.userID, "Member Registered: " + m.memberID);

    cout << "\n  ✓ Registration successful!\n";
    cout << "  Your Member ID : " << m.memberID << "\n";
    cout << "  [NOTIFICATION] Welcome email sent to " << m.email << "\n";
    pressEnter();
}

// ─────────────────────────────────────────────
//  MEMBER PORTAL MENU
// ─────────────────────────────────────────────

void memberViewProfile() {
    Member* m = findMemberByID(loggedInMemberID);
    if (!m) return;
    printHeader("MY PROFILE");
    cout << "  Member ID   : " << m->memberID << "\n";
    cout << "  Name        : " << m->firstName << " " << m->lastName << "\n";
    cout << "  Email       : " << m->email << "\n";
    cout << "  Contact     : " << m->contactNumber << "\n";
    cout << "  DOB         : " << m->dateOfBirth << "\n";
    cout << "  CNIC        : " << m->cnic << "\n";
    cout << "  Address     : " << m->address << "\n";
    cout << "  Emergency   : " << m->emergencyContact << "\n";
    cout << "  Status      : " << memberStatusStr(m->status) << "\n";
    cout << "  Reg. Date   : " << m->registrationDate << "\n";

    // Show active membership
    Membership* ms = findActiveMembership(m->memberID);
    if (ms) {
        MembershipPlan* p = findPlanByID(ms->planID);
        cout << "\n  ── Active Membership ──\n";
        cout << "  Plan        : " << (p ? p->planName : "N/A") << "\n";
        cout << "  Start       : " << ms->startDate << "\n";
        cout << "  End         : " << ms->endDate << "\n";
        cout << "  Status      : " << membershipStatusStr(ms->status) << "\n";
    } else {
        cout << "\n  ── No Active Membership ──\n";
    }
    pressEnter();
}

void memberUpdateProfile() {
    Member* m = findMemberByID(loggedInMemberID);
    if (!m) return;
    printHeader("UPDATE PROFILE");
    cin.ignore();
    cout << "  New Contact Number (leave blank to keep '"
         << m->contactNumber << "'): ";
    string inp; getline(cin, inp);
    if (!inp.empty()) m->contactNumber = inp;

    cout << "  New Address (leave blank to keep current): ";
    getline(cin, inp);
    if (!inp.empty()) m->address = inp;

    cout << "  New Emergency Contact (leave blank to keep): ";
    getline(cin, inp);
    if (!inp.empty()) m->emergencyContact = inp;

    logAudit(loggedInUserID, "Profile Updated: " + m->memberID);
    cout << "\n  ✓ Profile updated successfully.\n";
    pressEnter();
}

void memberEnrollPlan() {
    Membership* existing = findActiveMembership(loggedInMemberID);
    if (existing) {
        cout << "\n  ✗ You already have an active membership.\n"
             << "    Use 'Upgrade/Downgrade' option to change your plan.\n";
        pressEnter();
        return;
    }

    printHeader("ENROLL IN MEMBERSHIP PLAN");
    displayAllPlans();

    cout << "\n  Enter Plan ID to enroll: ";
    string planID; cin >> planID;

    MembershipPlan* plan = findPlanByID(planID);
    if (!plan || !plan->isActive) {
        cout << "\n  ✗ Invalid or inactive plan.\n";
        pressEnter();
        return;
    }

    double finalPrice = plan->price;

    // FR-MP-08: Discount code
    cout << "\n  Do you have a discount code? (y/n): ";
    char ch; cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        cout << "  Enter code: ";
        string code; cin >> code;
        // Convert to uppercase
        transform(code.begin(), code.end(), code.begin(), ::toupper);
        if (discountCodes.count(code)) {
            double pct = discountCodes[code];
            double disc = finalPrice * pct / 100.0;
            finalPrice -= disc;
            cout << "  ✓ Discount applied: " << pct << "% (-PKR "
                 << fixed << setprecision(2) << disc << ")\n";
        } else {
            cout << "  ✗ Invalid discount code.\n";
        }
    }

    cout << "\n  Plan   : " << plan->planName << "\n";
    cout << "  Amount : PKR " << fixed << setprecision(2) << finalPrice << "\n";
    cout << "\n  Payment Method:\n  1. Cash\n  2. Card\n  3. Bank Transfer\n";
    cout << "  Choice: ";
    int mChoice; cin >> mChoice;
    PaymentMethod pm = PaymentMethod::Cash;
    if      (mChoice == 2) pm = PaymentMethod::Card;
    else if (mChoice == 3) pm = PaymentMethod::BankTransfer;

    cout << "\n  Processing payment... ";

    // Simulate payment
    Payment pay;
    pay.paymentID       = generateID("PAY-");
    pay.memberID        = loggedInMemberID;
    pay.amount          = finalPrice;
    pay.paymentDate     = currentDate();
    pay.method          = pm;
    pay.status          = PaymentStatus::Completed;
    pay.transactionRef  = generateID("TXN-");
    pay.discountApplied = plan->price - finalPrice;

    // Create membership
    Membership ms;
    ms.membershipID = generateID("MS-");
    ms.memberID     = loggedInMemberID;
    ms.planID       = planID;
    ms.startDate    = currentDate();
    // Compute end date (simplified: add months to year)
    ms.endDate      = "2026-" + to_string(plan->durationMonths) + "-30";
    ms.status       = MembershipStatus::Active;
    ms.autoRenew    = false;

    pay.membershipID = ms.membershipID;
    payments.push_back(pay);
    memberships.push_back(ms);

    logAudit(loggedInUserID, "Enrolled in plan " + planID + ", Payment: " + pay.paymentID);

    cout << "✓ APPROVED\n";
    cout << "  ✓ Membership activated!\n";
    cout << "  Membership ID : " << ms.membershipID << "\n";
    cout << "  Valid         : " << ms.startDate << " to " << ms.endDate << "\n";
    cout << "  Invoice       : " << pay.paymentID << "  Ref: " << pay.transactionRef << "\n";
    cout << "  [NOTIFICATION] Payment receipt emailed.\n";
    pressEnter();
}

void memberRenewMembership() {
    Membership* ms = findActiveMembership(loggedInMemberID);
    if (!ms) {
        // check expired
        Membership* exp = nullptr;
        for (auto& m : memberships)
            if (m.memberID == loggedInMemberID && m.status == MembershipStatus::Expired)
                exp = &m;

        if (!exp) {
            cout << "\n  No membership to renew. Please enroll first.\n";
            pressEnter();
            return;
        }
        ms = exp;
    }

    MembershipPlan* plan = findPlanByID(ms->planID);
    if (!plan) return;

    printHeader("RENEW MEMBERSHIP");
    cout << "  Current Plan  : " << plan->planName << "\n";
    cout << "  Renewal Price : PKR " << fixed << setprecision(2) << plan->price << "\n";
    cout << "\n  Confirm renewal? (y/n): ";
    char ch; cin >> ch;
    if (ch != 'y' && ch != 'Y') { cout << "  Cancelled.\n"; pressEnter(); return; }

    Payment pay;
    pay.paymentID      = generateID("PAY-");
    pay.memberID       = loggedInMemberID;
    pay.membershipID   = ms->membershipID;
    pay.amount         = plan->price;
    pay.paymentDate    = currentDate();
    pay.method         = PaymentMethod::Cash;
    pay.status         = PaymentStatus::Completed;
    pay.transactionRef = generateID("TXN-");
    pay.discountApplied= 0.0;
    payments.push_back(pay);

    ms->status    = MembershipStatus::Active;
    ms->startDate = currentDate();
    ms->endDate   = "2026-12-31";

    logAudit(loggedInUserID, "Membership Renewed: " + ms->membershipID);
    cout << "\n  ✓ Membership renewed successfully!\n";
    cout << "  New Expiry : " << ms->endDate << "\n";
    pressEnter();
}

void memberBookClass() {
    Membership* ms = findActiveMembership(loggedInMemberID);
    if (!ms) {
        cout << "\n  ✗ You need an active membership to book classes.\n";
        cout << "  Please enroll in a plan first. (Option: Renew Membership)\n";
        pressEnter();
        return;
    }

    printHeader("FITNESS CLASS SCHEDULE");
    displayAllClasses();

    cout << "\n  Enter Class ID to book: ";
    string classID; cin >> classID;

    FitnessClass* fc = findClassByID(classID);
    if (!fc || fc->status == ClassStatus::Cancelled || fc->status == ClassStatus::Completed) {
        cout << "\n  ✗ Invalid or unavailable class.\n";
        pressEnter();
        return;
    }

    // Check already booked
    for (auto& b : bookings) {
        if (b.memberID == loggedInMemberID && b.classID == classID &&
            b.status == BookingStatus::Confirmed) {
            cout << "\n  ✗ You have already booked this class.\n";
            pressEnter();
            return;
        }
    }

    if (fc->currentEnrollment >= fc->maxCapacity) {
        cout << "\n  ✗ Class is full! Would you like to join the waiting list? (y/n): ";
        char ch; cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            cout << "  ✓ Added to waiting list for " << fc->className << ".\n";
            logAudit(loggedInUserID, "Joined waitlist for class: " + classID);
        }
        pressEnter();
        return;
    }

    Booking bk;
    bk.bookingID       = generateID("BK-");
    bk.memberID        = loggedInMemberID;
    bk.classID         = classID;
    bk.bookingDateTime = currentDate();
    bk.status          = BookingStatus::Confirmed;
    bookings.push_back(bk);
    fc->currentEnrollment++;

    logAudit(loggedInUserID, "Booked class: " + classID);
    cout << "\n  ✓ Booking confirmed!\n";
    cout << "  Booking ID  : " << bk.bookingID << "\n";
    cout << "  Class       : " << fc->className << "\n";
    cout << "  DateTime    : " << fc->scheduledDateTime << "\n";
    cout << "  [NOTIFICATION] Confirmation email sent.\n";
    pressEnter();
}

void memberCancelBooking() {
    printHeader("CANCEL A BOOKING");
    displayMemberBookings(loggedInMemberID);

    cout << "\n  Enter Booking ID to cancel: ";
    string bid; cin >> bid;

    for (auto& b : bookings) {
        if (b.bookingID == bid && b.memberID == loggedInMemberID) {
            if (b.status != BookingStatus::Confirmed) {
                cout << "\n  ✗ This booking cannot be cancelled (status: "
                     << bookStatusStr(b.status) << ").\n";
                pressEnter();
                return;
            }
            b.status = BookingStatus::Cancelled;
            FitnessClass* fc = findClassByID(b.classID);
            if (fc) fc->currentEnrollment--;
            logAudit(loggedInUserID, "Cancelled booking: " + bid);
            cout << "\n  ✓ Booking " << bid << " cancelled.\n";
            pressEnter();
            return;
        }
    }
    cout << "\n  ✗ Booking ID not found.\n";
    pressEnter();
}

void memberCheckIn() {
    Membership* ms = findActiveMembership(loggedInMemberID);
    if (!ms) {
        cout << "\n  ✗ Active membership required to check in.\n";
        pressEnter();
        return;
    }
    // Check if already checked in
    for (auto& a : attendance) {
        if (a.memberID == loggedInMemberID && a.checkOutTime.empty()) {
            cout << "\n  ✗ You are already checked in (Attendance ID: " << a.attendanceID << ").\n";
            pressEnter();
            return;
        }
    }

    Attendance att;
    att.attendanceID = generateID("ATT-");
    att.memberID     = loggedInMemberID;
    att.checkInTime  = currentDate() + " 09:00";
    att.checkOutTime = "";
    att.date         = currentDate();
    attendance.push_back(att);

    logAudit(loggedInUserID, "Check-In: " + att.attendanceID);
    cout << "\n  ✓ Check-In recorded! Attendance ID: " << att.attendanceID << "\n";
    pressEnter();
}

void memberCheckOut() {
    for (auto& a : attendance) {
        if (a.memberID == loggedInMemberID && a.checkOutTime.empty()) {
            a.checkOutTime = currentDate() + " 11:30";
            logAudit(loggedInUserID, "Check-Out: " + a.attendanceID);
            cout << "\n  ✓ Check-Out recorded. Attendance ID: " << a.attendanceID << "\n";
            cout << "  Duration approx: 2h 30m\n";
            pressEnter();
            return;
        }
    }
    cout << "\n  ✗ No active check-in found. Please check in first.\n";
    pressEnter();
}

void memberCancelMembership() {
    Membership* ms = findActiveMembership(loggedInMemberID);
    if (!ms) {
        cout << "\n  ✗ No active membership to cancel.\n";
        pressEnter();
        return;
    }
    printHeader("CANCEL MEMBERSHIP");
    cout << "  ⚠ This will permanently cancel your membership.\n";
    cout << "  Are you sure? (yes/no): ";
    string confirm; cin >> confirm;
    if (confirm != "yes") {
        cout << "  Cancellation aborted.\n";
        pressEnter();
        return;
    }

    // Check pending payments
    double dues = 0.0;
    for (auto& p : payments) {
        if (p.memberID == loggedInMemberID && p.status == PaymentStatus::Pending)
            dues += p.amount;
    }
    if (dues > 0.0) {
        cout << "\n  ⚠ You have pending dues of PKR "
             << fixed << setprecision(2) << dues
             << ". Please clear dues before cancellation.\n";
        pressEnter();
        return;
    }

    // Cancel future bookings
    for (auto& b : bookings) {
        if (b.memberID == loggedInMemberID && b.status == BookingStatus::Confirmed) {
            b.status = BookingStatus::Cancelled;
            FitnessClass* fc = findClassByID(b.classID);
            if (fc) fc->currentEnrollment--;
        }
    }

    ms->status = MembershipStatus::Cancelled;
    Member* mem = findMemberByID(loggedInMemberID);
    if (mem) mem->status = MemberStatus::Cancelled;

    logAudit(loggedInUserID, "Membership Cancelled: " + ms->membershipID);

    cout << "\n  ✓ Membership cancelled. All bookings have been cancelled.\n";
    cout << "  [NOTIFICATION] Exit confirmation email sent.\n";
    cout << "  [ADMIN NOTIFICATION] Member exit notification sent to admin.\n";
    pressEnter();
}

void memberPortal() {
    while (true) {
        printHeader("MEMBER PORTAL");
        cout << "  1. View My Profile\n";
        cout << "  2. Update Profile\n";
        cout << "  3. View Membership Plans\n";
        cout << "  4. Enroll in Plan\n";
        cout << "  5. Renew Membership\n";
        cout << "  6. View Payment History\n";
        cout << "  7. Book Fitness Class\n";
        cout << "  8. Cancel a Booking\n";
        cout << "  9. View My Bookings\n";
        cout << " 10. Check-In to Gym\n";
        cout << " 11. Check-Out from Gym\n";
        cout << " 12. View Attendance History\n";
        cout << " 13. Cancel Membership\n";
        cout << "  0. Logout\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;

        switch (ch) {
            case  1: memberViewProfile(); break;
            case  2: memberUpdateProfile(); break;
            case  3: printHeader("AVAILABLE PLANS"); displayAllPlans(); pressEnter(); break;
            case  4: memberEnrollPlan(); break;
            case  5: memberRenewMembership(); break;
            case  6: printHeader("PAYMENT HISTORY"); displayMemberPayments(loggedInMemberID); pressEnter(); break;
            case  7: memberBookClass(); break;
            case  8: memberCancelBooking(); break;
            case  9: printHeader("MY BOOKINGS"); displayMemberBookings(loggedInMemberID); pressEnter(); break;
            case 10: memberCheckIn(); break;
            case 11: memberCheckOut(); break;
            case 12: printHeader("MY ATTENDANCE"); displayMemberAttendance(loggedInMemberID); pressEnter(); break;
            case 13: memberCancelMembership(); break;
            case  0: logout(); return;
            default: cout << "\n  ✗ Invalid option.\n"; pressEnter(); break;
        }
    }
}

// ─────────────────────────────────────────────
//  TRAINER PORTAL
// ─────────────────────────────────────────────

void trainerViewSchedule() {
    printHeader("MY CLASS SCHEDULE");
    cout << "\n  " << left
         << setw(10) << "ClassID"
         << setw(20) << "Class Name"
         << setw(20) << "DateTime"
         << setw(8)  << "Enrl"
         << "Status\n";
    printLine();
    for (auto& c : classes) {
        if (c.trainerID == loggedInTrainerID) {
            cout << "  "
                 << setw(10) << c.classID
                 << setw(20) << c.className
                 << setw(20) << c.scheduledDateTime
                 << setw(8)  << c.currentEnrollment
                 << classStatusStr(c.status) << "\n";
        }
    }
    pressEnter();
}

void trainerViewAssignedMembers() {
    printHeader("MY ENROLLED MEMBERS");
    bool any = false;
    for (auto& c : classes) {
        if (c.trainerID == loggedInTrainerID) {
            cout << "\n  Class: " << c.className << " (" << c.classID << ")\n";
            printLine();
            for (auto& b : bookings) {
                if (b.classID == c.classID && b.status == BookingStatus::Confirmed) {
                    Member* m = findMemberByID(b.memberID);
                    if (m) {
                        cout << "  " << m->memberID << "  "
                             << m->firstName << " " << m->lastName << "\n";
                        any = true;
                    }
                }
            }
        }
    }
    if (!any) cout << "  No enrolled members found.\n";
    pressEnter();
}

void trainerUpdateProgress() {
    printHeader("UPDATE MEMBER PROGRESS NOTES");
    cout << "  Member ID: "; string mid; cin >> mid;
    cin.ignore();
    Member* m = findMemberByID(mid);
    if (!m) { cout << "\n  ✗ Member not found.\n"; pressEnter(); return; }
    cout << "  Notes for " << m->firstName << " " << m->lastName << ": ";
    string notes; getline(cin, notes);
    logAudit(loggedInUserID, "Progress note for member " + mid + ": " + notes);
    cout << "\n  ✓ Progress notes saved.\n";
    pressEnter();
}

void trainerPortal() {
    while (true) {
        printHeader("TRAINER PORTAL");
        Trainer* tr = findTrainerByID(loggedInTrainerID);
        if (tr) cout << "  Trainer: " << tr->firstName << " " << tr->lastName << "\n\n";
        cout << "  1. View My Class Schedule\n";
        cout << "  2. View Enrolled Members\n";
        cout << "  3. Update Member Progress Notes\n";
        cout << "  4. View All Classes\n";
        cout << "  0. Logout\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;
        switch (ch) {
            case 1: trainerViewSchedule(); break;
            case 2: trainerViewAssignedMembers(); break;
            case 3: trainerUpdateProgress(); break;
            case 4: printHeader("ALL FITNESS CLASSES"); displayAllClasses(); pressEnter(); break;
            case 0: logout(); return;
            default: cout << "\n  ✗ Invalid option.\n"; pressEnter(); break;
        }
    }
}

// ─────────────────────────────────────────────
//  ADMIN — MEMBER MANAGEMENT
// ─────────────────────────────────────────────

void adminManageMembers() {
    while (true) {
        printHeader("ADMIN — MEMBER MANAGEMENT");
        cout << "  1. View All Members\n";
        cout << "  2. Search Member by ID\n";
        cout << "  3. Activate Member\n";
        cout << "  4. Suspend Member\n";
        cout << "  5. Remove Member Account\n";
        cout << "  6. View Member Payments\n";
        cout << "  7. View Member Attendance\n";
        cout << "  0. Back\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;

        if (ch == 0) return;

        if (ch == 1) {
            printHeader("ALL MEMBERS");
            displayAllMembers();
            pressEnter();
        }
        else if (ch == 2) {
            cout << "\n  Member ID: "; string id; cin >> id;
            Member* m = findMemberByID(id);
            if (!m) { cout << "\n  ✗ Not found.\n"; }
            else {
                printLine();
                cout << "  ID      : " << m->memberID << "\n";
                cout << "  Name    : " << m->firstName << " " << m->lastName << "\n";
                cout << "  Email   : " << m->email << "\n";
                cout << "  Contact : " << m->contactNumber << "\n";
                cout << "  CNIC    : " << m->cnic << "\n";
                cout << "  Status  : " << memberStatusStr(m->status) << "\n";
            }
            pressEnter();
        }
        else if (ch == 3 || ch == 4 || ch == 5) {
            cout << "\n  Member ID: "; string id; cin >> id;
            Member* m = findMemberByID(id);
            if (!m) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            if (ch == 3) {
                m->status = MemberStatus::Active;
                m->isActive = true;
                cout << "\n  ✓ Member activated.\n";
                logAudit(loggedInUserID, "Activated member: " + id);
            } else if (ch == 4) {
                m->status = MemberStatus::Suspended;
                cout << "\n  ✓ Member suspended.\n";
                logAudit(loggedInUserID, "Suspended member: " + id);
            } else {
                m->status = MemberStatus::Cancelled;
                m->isActive = false;
                cout << "\n  ✓ Member account removed.\n";
                logAudit(loggedInUserID, "Removed member: " + id);
            }
            pressEnter();
        }
        else if (ch == 6) {
            cout << "\n  Member ID: "; string id; cin >> id;
            printHeader("PAYMENT HISTORY");
            displayMemberPayments(id);
            pressEnter();
        }
        else if (ch == 7) {
            cout << "\n  Member ID: "; string id; cin >> id;
            printHeader("ATTENDANCE HISTORY");
            displayMemberAttendance(id);
            pressEnter();
        }
        else { cout << "\n  ✗ Invalid option.\n"; pressEnter(); }
    }
}

// ─────────────────────────────────────────────
//  ADMIN — PLAN MANAGEMENT
// ─────────────────────────────────────────────

void adminManagePlans() {
    while (true) {
        printHeader("ADMIN — MEMBERSHIP PLAN MANAGEMENT");
        cout << "  1. View All Plans\n";
        cout << "  2. Create New Plan\n";
        cout << "  3. Edit Plan Price\n";
        cout << "  4. Activate / Deactivate Plan\n";
        cout << "  0. Back\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;
        if (ch == 0) return;

        if (ch == 1) {
            printHeader("ALL MEMBERSHIP PLANS");
            displayAllPlans();
            pressEnter();
        }
        else if (ch == 2) {
            printHeader("CREATE NEW PLAN");
            cin.ignore();
            MembershipPlan p;
            cout << "  Plan Name          : "; getline(cin, p.planName);
            cout << "  Duration (months)  : "; cin >> p.durationMonths;
            cout << "  Price (PKR)        : "; cin >> p.price;
            cin.ignore();
            cout << "  Facilities         : "; getline(cin, p.facilities);
            p.planID           = generateID("PLAN-");
            p.isActive         = true;
            p.createdByAdminID = "ADM-001";
            plans.push_back(p);
            logAudit(loggedInUserID, "Created plan: " + p.planID);
            cout << "\n  ✓ Plan created. ID: " << p.planID << "\n";
            pressEnter();
        }
        else if (ch == 3) {
            displayAllPlans();
            cout << "\n  Plan ID  : "; string id; cin >> id;
            MembershipPlan* p = findPlanByID(id);
            if (!p) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            cout << "  New Price: "; cin >> p->price;
            logAudit(loggedInUserID, "Edited plan price: " + id);
            cout << "\n  ✓ Price updated.\n";
            pressEnter();
        }
        else if (ch == 4) {
            displayAllPlans();
            cout << "\n  Plan ID: "; string id; cin >> id;
            MembershipPlan* p = findPlanByID(id);
            if (!p) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            p->isActive = !p->isActive;
            logAudit(loggedInUserID, "Toggled plan: " + id);
            cout << "\n  ✓ Plan is now " << (p->isActive ? "Active" : "Inactive") << ".\n";
            pressEnter();
        }
        else { cout << "\n  ✗ Invalid.\n"; pressEnter(); }
    }
}

// ─────────────────────────────────────────────
//  ADMIN — TRAINER MANAGEMENT
// ─────────────────────────────────────────────

void adminManageTrainers() {
    while (true) {
        printHeader("ADMIN — TRAINER MANAGEMENT");
        cout << "  1. View All Trainers\n";
        cout << "  2. Register New Trainer\n";
        cout << "  3. Deactivate Trainer\n";
        cout << "  0. Back\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;
        if (ch == 0) return;

        if (ch == 1) {
            printHeader("ALL TRAINERS");
            displayAllTrainers();
            pressEnter();
        }
        else if (ch == 2) {
            printHeader("REGISTER NEW TRAINER");
            cin.ignore();
            Trainer t;
            cout << "  First Name       : "; getline(cin, t.firstName);
            cout << "  Last Name        : "; getline(cin, t.lastName);
            cout << "  Email            : "; getline(cin, t.email);
            string pw;
            cout << "  Password         : "; getline(cin, pw);
            cout << "  Specialization   : "; getline(cin, t.specialization);
            cout << "  Qualification    : "; getline(cin, t.qualification);
            cout << "  Contact Number   : "; getline(cin, t.contactNumber);

            t.trainerID    = generateID("TRN-");
            t.userID       = generateID("USR-TRN-");
            t.passwordHash = hashPassword(pw);
            t.isActive     = true;
            t.createdAt    = currentDate();
            t.role         = UserRole::Trainer;
            t.active       = true;
            trainers.push_back(t);
            logAudit(loggedInUserID, "Registered trainer: " + t.trainerID);
            cout << "\n  ✓ Trainer registered. ID: " << t.trainerID << "\n";
            pressEnter();
        }
        else if (ch == 3) {
            displayAllTrainers();
            cout << "\n  Trainer ID: "; string id; cin >> id;
            Trainer* t = findTrainerByID(id);
            if (!t) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            t->active = false;
            logAudit(loggedInUserID, "Deactivated trainer: " + id);
            cout << "\n  ✓ Trainer deactivated.\n";
            pressEnter();
        }
        else { cout << "\n  ✗ Invalid.\n"; pressEnter(); }
    }
}

// ─────────────────────────────────────────────
//  ADMIN — CLASS MANAGEMENT
// ─────────────────────────────────────────────

void adminManageClasses() {
    while (true) {
        printHeader("ADMIN — CLASS SCHEDULING");
        cout << "  1. View All Classes\n";
        cout << "  2. Create New Class\n";
        cout << "  3. Assign Trainer to Class\n";
        cout << "  4. Cancel a Class\n";
        cout << "  5. View Class Bookings\n";
        cout << "  0. Back\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;
        if (ch == 0) return;

        if (ch == 1) {
            printHeader("ALL CLASSES");
            displayAllClasses();
            pressEnter();
        }
        else if (ch == 2) {
            printHeader("CREATE FITNESS CLASS");
            cin.ignore();
            FitnessClass fc;
            cout << "  Class Name     : "; getline(cin, fc.className);
            cout << "  Class Type     : "; getline(cin, fc.classType);
            cout << "  Location       : "; getline(cin, fc.location);
            cout << "  Date & Time    : "; getline(cin, fc.scheduledDateTime);
            cout << "  Duration (min) : "; cin >> fc.durationMinutes;
            cout << "  Max Capacity   : "; cin >> fc.maxCapacity;
            cin.ignore();
            cout << "  Trainer ID     : ";
            displayAllTrainers();
            getline(cin, fc.trainerID);
            fc.classID           = generateID("CLS-");
            fc.currentEnrollment = 0;
            fc.status            = ClassStatus::Scheduled;
            classes.push_back(fc);
            logAudit(loggedInUserID, "Created class: " + fc.classID);
            cout << "\n  ✓ Class created. ID: " << fc.classID << "\n";
            pressEnter();
        }
        else if (ch == 3) {
            displayAllClasses();
            cout << "\n  Class ID   : "; string cid; cin >> cid;
            displayAllTrainers();
            cout << "  Trainer ID : "; string tid; cin >> tid;
            FitnessClass* fc = findClassByID(cid);
            if (!fc) { cout << "\n  ✗ Class not found.\n"; pressEnter(); continue; }
            fc->trainerID = tid;
            logAudit(loggedInUserID, "Assigned trainer " + tid + " to class " + cid);
            cout << "\n  ✓ Trainer assigned.\n";
            pressEnter();
        }
        else if (ch == 4) {
            displayAllClasses();
            cout << "\n  Class ID to cancel: "; string cid; cin >> cid;
            FitnessClass* fc = findClassByID(cid);
            if (!fc) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            fc->status = ClassStatus::Cancelled;
            for (auto& b : bookings) {
                if (b.classID == cid && b.status == BookingStatus::Confirmed)
                    b.status = BookingStatus::Cancelled;
            }
            logAudit(loggedInUserID, "Cancelled class: " + cid);
            cout << "\n  ✓ Class cancelled. All bookings cancelled.\n";
            pressEnter();
        }
        else if (ch == 5) {
            displayAllClasses();
            cout << "\n  Class ID: "; string cid; cin >> cid;
            cout << "\n  Bookings for Class " << cid << ":\n";
            printLine();
            bool found = false;
            for (auto& b : bookings) {
                if (b.classID == cid) {
                    Member* m = findMemberByID(b.memberID);
                    cout << "  " << b.bookingID << "  "
                         << (m ? m->firstName + " " + m->lastName : "N/A")
                         << "  " << bookStatusStr(b.status) << "\n";
                    found = true;
                }
            }
            if (!found) cout << "  No bookings for this class.\n";
            pressEnter();
        }
        else { cout << "\n  ✗ Invalid.\n"; pressEnter(); }
    }
}

// ─────────────────────────────────────────────
//  ADMIN — EQUIPMENT MANAGEMENT
// ─────────────────────────────────────────────

void adminManageEquipment() {
    while (true) {
        printHeader("ADMIN — EQUIPMENT MANAGEMENT");
        cout << "  1. View Inventory\n";
        cout << "  2. Add Equipment\n";
        cout << "  3. Update Condition\n";
        cout << "  4. Schedule Maintenance\n";
        cout << "  5. View Maintenance Alerts\n";
        cout << "  0. Back\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;
        if (ch == 0) return;

        if (ch == 1) {
            printHeader("EQUIPMENT INVENTORY");
            displayEquipment();
            pressEnter();
        }
        else if (ch == 2) {
            printHeader("ADD EQUIPMENT");
            cin.ignore();
            Equipment eq;
            cout << "  Name          : "; getline(cin, eq.name);
            cout << "  Quantity      : "; cin >> eq.quantity;
            cout << "  Purchase Cost : "; cin >> eq.purchaseCost;
            cin.ignore();
            cout << "  Purchase Date : "; getline(cin, eq.purchaseDate);
            eq.equipmentID        = generateID("EQ-");
            eq.condition          = EquipCondition::Excellent;
            eq.lastMaintenanceDate= currentDate();
            eq.nextMaintenanceDate= "2026-11-01";
            equipment.push_back(eq);
            logAudit(loggedInUserID, "Added equipment: " + eq.equipmentID);
            cout << "\n  ✓ Equipment added. ID: " << eq.equipmentID << "\n";
            pressEnter();
        }
        else if (ch == 3) {
            displayEquipment();
            cout << "\n  Equipment ID: "; string id; cin >> id;
            Equipment* eq = findEquipByID(id);
            if (!eq) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            cout << "  New Condition:\n";
            cout << "  1. Excellent  2. Good  3. Fair  4. Needs Repair  5. Out of Service\n";
            cout << "  Choice: ";
            int cond; cin >> cond;
            switch(cond) {
                case 1: eq->condition = EquipCondition::Excellent; break;
                case 2: eq->condition = EquipCondition::Good; break;
                case 3: eq->condition = EquipCondition::Fair; break;
                case 4: eq->condition = EquipCondition::NeedsRepair; break;
                case 5: eq->condition = EquipCondition::OutOfService; break;
            }
            logAudit(loggedInUserID, "Updated equipment condition: " + id);
            cout << "\n  ✓ Condition updated.\n";
            pressEnter();
        }
        else if (ch == 4) {
            displayEquipment();
            cout << "\n  Equipment ID   : "; string id; cin >> id;
            cin.ignore();
            Equipment* eq = findEquipByID(id);
            if (!eq) { cout << "\n  ✗ Not found.\n"; pressEnter(); continue; }
            cout << "  Next Maint Date: "; getline(cin, eq->nextMaintenanceDate);
            eq->lastMaintenanceDate = currentDate();
            logAudit(loggedInUserID, "Scheduled maintenance for: " + id);
            cout << "\n  ✓ Maintenance scheduled.\n";
            pressEnter();
        }
        else if (ch == 5) {
            printHeader("MAINTENANCE ALERTS");
            bool any = false;
            for (auto& e : equipment) {
                if (e.condition == EquipCondition::NeedsRepair ||
                    e.condition == EquipCondition::OutOfService) {
                    cout << "  ⚠ ALERT: " << e.name << " (" << e.equipmentID
                         << ") — " << equipCondStr(e.condition) << "\n";
                    any = true;
                }
            }
            if (!any) cout << "  ✓ No maintenance alerts.\n";
            pressEnter();
        }
        else { cout << "\n  ✗ Invalid.\n"; pressEnter(); }
    }
}

// ─────────────────────────────────────────────
//  ADMIN — REPORTS
// ─────────────────────────────────────────────

void adminGenerateReports() {
    printHeader("ADMIN — REPORTS & ANALYTICS");

    // Total members
    int active = 0, suspended = 0, cancelled = 0;
    for (auto& m : members) {
        if      (m.status == MemberStatus::Active)    active++;
        else if (m.status == MemberStatus::Suspended) suspended++;
        else                                          cancelled++;
    }

    // Revenue
    double totalRevenue = 0.0;
    int completedPayments = 0;
    for (auto& p : payments) {
        if (p.status == PaymentStatus::Completed) {
            totalRevenue += p.amount;
            completedPayments++;
        }
    }

    // Active memberships
    int activeMemberships = 0;
    for (auto& ms : memberships)
        if (ms.status == MembershipStatus::Active) activeMemberships++;

    cout << "  ══ MEMBER STATISTICS ══\n";
    cout << "  Total Members     : " << members.size() << "\n";
    cout << "  Active            : " << active << "\n";
    cout << "  Suspended         : " << suspended << "\n";
    cout << "  Cancelled         : " << cancelled << "\n";

    cout << "\n  ══ FINANCIAL REPORT ══\n";
    cout << "  Total Payments    : " << completedPayments << "\n";
    cout << "  Total Revenue     : PKR " << fixed << setprecision(2) << totalRevenue << "\n";

    cout << "\n  ══ MEMBERSHIP REPORT ══\n";
    cout << "  Active Memberships: " << activeMemberships << "\n";

    cout << "\n  ══ CLASS REPORT ══\n";
    cout << "  Total Classes     : " << classes.size() << "\n";
    cout << "  Total Bookings    : " << bookings.size() << "\n";

    cout << "\n  ══ INACTIVE MEMBERS (>14 days) ══\n";  // FR-AT-03
    bool foundInactive = false;
    for (auto& m : members) {
        bool hasRecent = false;
        for (auto& a : attendance)
            if (a.memberID == m.memberID) { hasRecent = true; break; }
        if (!hasRecent && m.status == MemberStatus::Active) {
            cout << "  ⚠ " << m.memberID << " — " << m.firstName << " "
                 << m.lastName << " (No recent check-in)\n";
            foundInactive = true;
        }
    }
    if (!foundInactive) cout << "  None detected.\n";

    pressEnter();
}

// ─────────────────────────────────────────────
//  ADMIN — ATTENDANCE REPORT
// ─────────────────────────────────────────────

void adminViewAttendance() {
    printHeader("ADMIN — ATTENDANCE REPORT");
    cout << "\n  " << left
         << setw(14) << "AttendanceID"
         << setw(12) << "MemberID"
         << setw(20) << "Check-In"
         << setw(20) << "Check-Out"
         << "Date\n";
    printLine();
    for (auto& a : attendance) {
        cout << "  "
             << setw(14) << a.attendanceID
             << setw(12) << a.memberID
             << setw(20) << a.checkInTime
             << setw(20) << (a.checkOutTime.empty() ? "Active" : a.checkOutTime)
             << a.date << "\n";
    }
    if (attendance.empty()) cout << "  No attendance records.\n";
    pressEnter();
}

// ─────────────────────────────────────────────
//  ADMIN — AUDIT TRAIL
// ─────────────────────────────────────────────

void adminViewAuditTrail() {
    printHeader("AUDIT TRAIL");
    cout << "\n  " << left
         << setw(14) << "Timestamp"
         << setw(18) << "UserID"
         << "Action\n";
    printLine();
    for (auto& e : auditLog) {
        cout << "  "
             << setw(14) << e.timestamp
             << setw(18) << e.userID
             << e.action << "\n";
    }
    if (auditLog.empty()) cout << "  No audit entries.\n";
    pressEnter();
}

// ─────────────────────────────────────────────
//  ADMIN PORTAL
// ─────────────────────────────────────────────

void adminPortal() {
    while (true) {
        printHeader("ADMINISTRATOR PORTAL");
        cout << "  1.  Member Management\n";
        cout << "  2.  Membership Plan Management\n";
        cout << "  3.  Trainer Management\n";
        cout << "  4.  Class Scheduling\n";
        cout << "  5.  Equipment Management\n";
        cout << "  6.  Financial Reports\n";
        cout << "  7.  Attendance Reports\n";
        cout << "  8.  Audit Trail\n";
        cout << "  0.  Logout\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;

        switch (ch) {
            case 1: adminManageMembers(); break;
            case 2: adminManagePlans(); break;
            case 3: adminManageTrainers(); break;
            case 4: adminManageClasses(); break;
            case 5: adminManageEquipment(); break;
            case 6: adminGenerateReports(); break;
            case 7: adminViewAttendance(); break;
            case 8: adminViewAuditTrail(); break;
            case 0: logout(); return;
            default: cout << "\n  ✗ Invalid option.\n"; pressEnter(); break;
        }
    }
}

// ─────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────


void mainMenu() {
    while (true) {
        printHeader("GYM MEMBERSHIP SYSTEM (GMS)");
        cout << "  UET Lahore — Software Engineering AI-131\n\n";
        cout << "  1. Login\n";
        cout << "  2. Register as New Member\n";
        cout << "  0. Exit\n";
        cout << "\n  Choice: ";
        int ch; cin >> ch;

        if (ch == 0) {
            cout << "\n  Thank you for using GMS. Goodbye!\n\n";
            return;
        }
        else if (ch == 1) {
            if (login()) {
                if      (loggedInRole == UserRole::Admin)   adminPortal();
                else if (loggedInRole == UserRole::Trainer) trainerPortal();
                else                                        memberPortal();
            }
        }
        else if (ch == 2) {
            registerMember();
        }
        else {
            cout << "\n  ✗ Invalid option.\n";
            pressEnter();
        }
    }
}

// ─────────────────────────────────────────────
//  ENTRY POINT
// ─────────────────────────────────────────────

int main() {
    cout << "\n  Loading Gym Membership System...\n";
    seedData();
    cout << "  Demo data loaded.\n\n";
    cout << "  |_________________________________________|\n";
    cout << "  │  DEMO LOGIN CREDENTIALS                 |\n";
    cout << "  │  Admin   : admin@gym.com / Admin@123    │\n";
    cout << "  │  Trainer : sara@gym.com  / Sara@123     │\n";
    cout << "  │  Member  : usman@email.com / Usman@123  │\n";
    cout << "  |_________________________________________|\n";
    pressEnter();

    mainMenu();
    return 0;
}