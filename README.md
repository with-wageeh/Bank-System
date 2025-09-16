# 🏦 Bank System

## 📌 Overview
The **Bank Management System** is a modular, object-oriented C++17 project.  
It demonstrates **OOP principles** (inheritance, polymorphism, encapsulation) while simulating a simplified bank with accounts, customers, transactions, and authentication.  
The project is split into multiple headers and source files for clarity and maintainability.

---

## ✨ Features
- **Accounts**:
  - `Checking` — supports deposits/withdrawals with overdraft limit.
  - `Saving` — supports deposits, withdrawals, and monthly interest updates.
  - `Loan` — tracks principal, remaining balance, and payment due dates.
- **Customers**:
  - Hold accounts inside an ordered `set` with a custom comparator.
  - Deposit/withdraw operations use **binary search** on a vector for faster lookup.
- **Transactions**:
  - `Transaction` and `Transaction_History` classes to store and print past operations.
- **Authentication**:
  - Register, login, and change password with an in-memory `unordered_map`.
- **Utilities**:
  - `Date` and `Address` structs to manage extra details.
- **Bank Service**:
  - Central manager for customers, transactions, and account operations.

---

