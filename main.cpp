#include <bits/stdc++.h>
using namespace std;

struct Date {
    int year = 0;
    int month = 0;
    int day = 0;

    Date() {}

    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    void set(int y, int m, int d) {
        year = y;
        month = m;
        day = d;
    }

    void print() {
        cout << year << " / " << month << " / " << day;
    }
};

Date next_month_date(Date &d) {
    int y = d.year;
    int m = d.month + 1;
    if (m > 12) {
        m = 1;
        y += 1;
    }
    int day = d.day;
    return Date(y, m, day);
}

struct Address {
    string country;
    string city;
    string street;
    string extra;

    Address() {}

    Address(string country_, string city_, string street_, string extra_ = "")
            : country(country_), city(city_), street(street_), extra(extra_) {}

    void print() {
        cout << street << " , " << city << " , " << country << '\n';
        if (!extra.empty()) cout << " More informations : " << extra;
    }
};

enum class status {
    Active, Frozen, Closed
};

class Account {
protected:
    int account_id = 0;
    int customer_id = 0;
    int id = 0;
    long long balance = 0;
    status current_status = status::Active;

public:
    Account() {}

    Account(int acc_id, int cust_id, int id_, long long balance_ = 0)
            : account_id(acc_id), customer_id(cust_id), id(id_), balance(balance_), current_status(status::Active) {}

    virtual ~Account() {}

    int get_account_id() { return account_id; }
    int get_customer_id() { return customer_id; }
    int get_id() { return id; }
    long long get_balance() { return balance; }
    status get_status() { return current_status; }

    void set_status(status s) { current_status = s; }
    void set_balance(long long v) { balance = v; }

    virtual void deposit(long long amount) {
        if (amount <= 0) {
            cout << " You can't do it by negative deposit";
            return;
        }
        balance += amount;
        cout << " the operation has been done\n";
    }

    virtual void withdraw(long long amount) {
        if (amount <= 0) {
            cout << " You can't withdraw it by negative ";
            return;
        }
        if (balance < amount) {
            cout << " your balance is not enough\n";
            return;
        }
        balance -= amount;
    }

    virtual void monthly_update() {}
};

class Checking : public Account {
    long long overdraft_limit = 30000;
public:
    Checking() {}

    Checking(int acc_id, int customer_id, long long init_balance, int id_)
            : Account(acc_id, customer_id, id_, init_balance) {}

    void deposit(long long amount) override {
        if (amount <= 0) {
            cout << " You can't do it by negative deposit";
            return;
        }
        balance += amount;
        cout << " deposit: " << amount << " new balance: " << balance << '\n';
    }

    void withdraw(long long amount) override {
        if (amount <= 0) return;
        if (current_status != status::Active) {
            cout << " account is not active\n";
            return;
        }
        if (amount > overdraft_limit) {
            cout << " it is above the limit\n";
            return;
        }
        balance -= amount;
        cout << " withdraw: " << amount << " new balance: " << balance << '\n';
    }
};

class Saving : public Account {
    double rate = 0.0;
public:
    Saving() {}

    Saving(int acc_id, int cust_id, double rate_ = 0.0, int id_ = 0)
            : Account(acc_id, cust_id, id_, 0), rate(rate_) {}

    void deposit(long long amount) override {
        if (amount <= 0) return;
        balance += amount;
        cout << " deposit: " << amount << " new balance: " << balance << '\n';
    }

    void withdraw(long long amount) override {
        if (amount <= 0) return;
        if (current_status != status::Active) {
            cout << " account not active\n";
            return;
        }
        if (balance < amount) {
            cout << " insufficient funds\n";
            return;
        }
        balance -= amount;
        cout << " saving withdraw: " << amount << " new balance: " << balance << '\n';
    }

    void monthly_update() override {
        long long interest = (balance * (double)(rate / 12));
        if (interest != 0) {
            balance += interest;
            cout << " monthly interest applied: " << interest << " new balance: " << balance << '\n';
        }
    }
};

class Loan : public Account {
    long long main = 0;
    long long remaining = 0;
    int percent = 0;
    Date next_date;
public:
    Loan() {}

    Loan(int acc_id, int cust_id, long long m, int pct, Date due, int id_)
            : Account(acc_id, cust_id, id_, 0), main(m), remaining(m), percent(pct), next_date(due) {}

    void deposit(long long amount) override {
        if (amount <= 0) return;
        if (amount > remaining) amount = remaining;
        remaining -= amount;
        cout << " loan payment accepted: " << amount << " remaining: " << remaining << '\n';
        next_date = next_month_date(next_date);
        cout << " The next date of payment is : ";
        next_date.print();
    }

    long long get_remaining() { return remaining; }
    Date get_next_due() { return next_date; }
    void set_next_due(Date &d) { next_date = d; }
};

struct account_ptr_less {
    bool operator()(const shared_ptr<Account> &a, const shared_ptr<Account> &b) const {
        if (a == b) return false;
        if (!a && !b) return false;
        if (!a) return true;
        if (!b) return false;
        if (a->get_account_id() != b->get_account_id()) return a->get_account_id() < b->get_account_id();
        return a->get_customer_id() < b->get_customer_id();
    }
};

void add_account_to_set(set<shared_ptr<Account>, account_ptr_less> &s, shared_ptr<Account> acc) {
    if (!acc) return;
    auto res = s.insert(acc);
    if (res.second) cout << " the account has been added\n";
    else cout << " the account already exists\n";
}

void remove_account_from_set(set<shared_ptr<Account>, account_ptr_less> &s, int acc_id) {
    for (auto it = s.begin(); it != s.end(); ++it) {
        if ((*it)->get_account_id() == acc_id) {
            s.erase(it);
            cout << " the account has been deleted\n";
            return;
        }
    }
    cout << " the account wasn't found\n";
}

bool deposit_to_set(set<shared_ptr<Account>, account_ptr_less> &s, int acc_id, long long amount) {
    vector<shared_ptr<Account>> v(s.begin(), s.end());
    int l = 0, r = (int)v.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (v[mid]->get_account_id() == acc_id) {
            v[mid]->deposit(amount);
            return true;
        } else if (v[mid]->get_account_id() < acc_id) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return false;
}

bool withdraw_from_set(set<shared_ptr<Account>, account_ptr_less> &s, int acc_id, long long amount) {
    vector<shared_ptr<Account>> v(s.begin(), s.end());
    int l = 0, r = (int)v.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (v[mid]->get_account_id() == acc_id) {
            v[mid]->withdraw(amount);
            return true;
        } else if (v[mid]->get_account_id() < acc_id) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return false;
}

class Customer {
    int id = 0;
    string name;
    Date dob;
    long long phone = 0;
    Address address;
    set<shared_ptr<Account>, account_ptr_less> accounts;
public:
    Customer() {}

    Customer(int id_, string name_, Date dob_, long long phone_, Address addr)
            : id(id_), name(name_), dob(dob_), phone(phone_), address(addr) {}

    void add_account(shared_ptr<Account> acc) { add_account_to_set(accounts, acc); }
    void delete_account_by_id(int acc_id) { remove_account_from_set(accounts, acc_id); }
    bool deposit_to_account(int acc_id, long long amount) { return deposit_to_set(accounts, acc_id, amount); }
    bool withdraw_from_account(int acc_id, long long amount) { return withdraw_from_set(accounts, acc_id, amount); }

    void apply_monthly_updates() {
        for (auto a : accounts) a->monthly_update();
    }

    void print_accounts() {
        if (accounts.empty()) {
            cout << " no accounts for customer " << id << '\n';
            return;
        }
        cout << " accounts for customer " << id << " :\n";
        for (auto a : accounts) {
            cout << " - id: " << a->get_account_id() << " balance: " << a->get_balance();
            if (auto p = dynamic_pointer_cast<Loan>(a)) cout << " loan remaining = " << p->get_remaining();
            cout << '\n';
        }
    }

    vector<int> get_account_ids() {
        vector<int> out;
        for (auto a : accounts) out.push_back(a->get_account_id());
        return out;
    }
};

class Transaction {
    int transaction_id = 0;
    int account_id = 0;
    long long amount = 0;
    string type;
    Date date;
public:
    Transaction() {}

    Transaction(int tid, int acc_id, long long amt, string t, Date d)
            : transaction_id(tid), account_id(acc_id), amount(amt), type(t), date(d) {}

    int get_transaction_id() { return transaction_id; }
    int get_account_id() { return account_id; }
    long long get_amount() { return amount; }
    string get_type() { return type; }
    Date get_date() { return date; }

    void print() {
        cout << " transaction_id: " << transaction_id << " account: " << account_id
             << " type: " << type << " amount: " << amount << " date: ";
        date.print();
        cout << '\n';
    }
};

class Transaction_History {
    vector<Transaction> transactions;
public:
    void add_transaction(Transaction t) { transactions.push_back(t); }

    void print_all() {
        if (transactions.empty()) {
            cout << " no transactions found\n";
            return;
        }
        cout << " transaction history:\n";
        for (auto t : transactions) t.print();
    }
};

class Authentication {
    unordered_map<string, string> users;
public:
    Authentication() {}

    void register_user(string u, string p) {
        users[u] = p;
    }

    bool login(string u, string p) {
        if (users.find(u) != users.end() && users[u] == p) return true;
        return false;
    }

    void change_password(string u, string old_p, string new_p) {
        if (users.find(u) != users.end() && users[u] == old_p) {
            users[u] = new_p;
            cout << " password changed\n";
        } else {
            cout << " old password incorrect\n";
        }
    }
};

class Bank_Service {
    vector<Customer> customers;
    Transaction_History history;
    int next_transaction_id = 1;
public:
    void add_customer(Customer c) { customers.push_back(c); }

    Customer* find_customer(int id) {
        for (auto &c: customers) {
            vector<int> accs = c.get_account_ids();
            for (int x : accs) {
                if (x == id) return &c;
            }
        }
        return nullptr;
    }

    void make_deposit(shared_ptr<Account> acc, long long amount, Date d) {
        if (!acc) return;
        acc->deposit(amount);
        Transaction t(next_transaction_id++, acc->get_account_id(), amount, "deposit", d);
        history.add_transaction(t);
    }

    void make_withdraw(shared_ptr<Account> acc, long long amount, Date d) {
        if (!acc) return;
        acc->withdraw(amount);
        Transaction t(next_transaction_id++, acc->get_account_id(), amount, "withdraw", d);
        history.add_transaction(t);
    }

    void print_transactions() { history.print_all(); }
};

int main() {
}
