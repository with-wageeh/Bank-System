# Bank Management System 

## Overview
A small object-oriented bank system written in modern C++ (C++17).  
Features accounts (Checking, Saving, Loan), customers holding sets of accounts, transaction history, and a simple in-memory authentication service. The code is split into modular headers and source files for clarity and maintainability.

## Features
- Account types: `Checking`, `Saving`, `Loan` (polymorphic `Account` base).
- Customers hold accounts in an ordered `set` with custom comparator.
- Binary-search-based deposit/withdraw helpers on ordered vectors for faster lookups.
- Transaction history and simple `Authentication`.
- `Date` and `Address` utility structs.

## Files / Structure
