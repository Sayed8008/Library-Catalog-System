#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

/* ========= UI EFFECTS ========= */

void slowPrint(string text, int delay = 6) {
    for (char c : text) {
        cout << c;
        Sleep(delay);
    }
}

void loadingDots(string text = "Processing") {
    cout << text;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        Sleep(250);
    }
    cout << endl;
}

/* ========= PASSWORD ========= */

string getPassword() {
    string pass;
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !pass.empty()) {
            pass.pop_back();
            cout << "\b \b";
        } else if (ch != '\b') {
            pass += ch;
            cout << "*";
        }
    }
    cout << endl;
    return pass;
}

/* ========= TRIE ========= */

class TrieNode {
public:
    map<char, TrieNode*> child;
    vector<string> codes;
};

class Trie {
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }

    void insert(string word, string code) {
        TrieNode* cur = root;
        for (char c : word) {
            if (!cur->child[c])
                cur->child[c] = new TrieNode();
            cur = cur->child[c];
        }
        cur->codes.push_back(code);
    }

    void search(string prefix,
                unordered_map<string,string>& names,
                unordered_map<string,int>& stock,
                vector<string>& results) {

        results.clear();
        TrieNode* cur = root;

        for (char c : prefix) {
            if (!cur->child[c]) {
                cout << "Nothing came up. Maybe try another word.\n";
                return;
            }
            cur = cur->child[c];
        }

        queue<TrieNode*> q;
        q.push(cur);

        int i = 1;
        cout << "\nResults:\n";

        while (!q.empty()) {
            TrieNode* node = q.front(); q.pop();

            for (auto &code : node->codes) {
                results.push_back(code);
                cout << i++ << ". "
                     << code << " | "
                     << names[code] << " | "
                     << stock[code] << " copies\n";
            }

            for (auto &p : node->child)
                q.push(p.second);
        }
    }
};

/* ========= GLOBAL ========= */

Trie trie;

unordered_map<string,string> bookNames;
unordered_map<string,int> bookStock;
unordered_map<string, vector<string>> userBooks;
unordered_map<string, queue<string>> waitlist;
unordered_map<string, pair<string,string>> users;

stack<pair<string,string>> undoStack;

vector<string> bucket;
vector<string> lastResults;

int nextBookId = 104;

/* ========= CORE ========= */

void registerUser() {
    slowPrint("\n[New User]\n");
    slowPrint("Alright, let's get you in.\n");

    string phone, name;
    cout << "Phone: "; cin >> phone;

    if (users.count(phone)) {
        cout << "This number is already in the system.\n";
        return;
    }

    cout << "Name: "; cin >> name;
    cout << "Passcode: ";
    string pass = getPassword();

    loadingDots("Saving");

    users[phone] = {name, pass};

    cout << "Done. You are officially in.\n";
}

void searchAndAdd() {
    slowPrint("\n[Find Books]\n");

    string p;
    cout << "Search: ";
    cin >> p;

    loadingDots("Searching");

    trie.search(p, bookNames, bookStock, lastResults);

    if (lastResults.empty()) return;

    cout << "\nPick number (0 to go back): ";
    int ch; cin >> ch;

    if (ch <= 0 || ch > lastResults.size()) return;

    string code = lastResults[ch-1];

    if (bucket.size() >= 2) {
        cout << "Two books max. Keep it balanced.\n";
        return;
    }

    if (find(bucket.begin(), bucket.end(), code) != bucket.end()) {
        cout << "Already picked. No need twice.\n";
        return;
    }

    bucket.push_back(code);
    undoStack.push({"", code});
    cout << "Added. Nice pick.\n";
}

void viewBucket() {
    slowPrint("\n[Your Selection]\n");

    if (bucket.empty()) {
        cout << "Nothing here yet. Go pick something.\n";
        return;
    }

    for (int i = 0; i < bucket.size(); i++)
        cout << i+1 << ". " << bucket[i]
             << " | " << bookNames[bucket[i]] << endl;
}

void checkout() {
    slowPrint("\n[Borrow]\n");

    if (bucket.empty()) {
        cout << "You came here with nothing selected.\n";
        return;
    }

    string phone;
    cout << "Phone: "; cin >> phone;

    if (!users.count(phone)) {
        cout << "User not found. Register first.\n";
        return;
    }

    cout << "Passcode: ";
    string pass = getPassword();

    if (users[phone].second != pass) {
        cout << "That passcode did not match.\n";
        return;
    }

    loadingDots("Processing");

    for (auto &code : bucket) {
        if (bookStock[code] > 0) {
            userBooks[phone].push_back(code);
            bookStock[code]--;
            undoStack.push({phone, code});
        } else {
            waitlist[code].push(phone);
        }
    }

    bucket.clear();
    cout << "Done. Books are now under your name.\n";
}

void returnBook() {
    slowPrint("\n[Return]\n");

    string phone, pass, code;

    cout << "Phone: "; cin >> phone;

    if (!users.count(phone)) {
        cout << "User not found.\n";
        return;
    }

    cout << "Passcode: ";
    pass = getPassword();

    if (users[phone].second != pass) {
        cout << "Wrong passcode.\n";
        return;
    }

    cout << "Book code: "; cin >> code;

    auto &v = userBooks[phone];
    auto it = find(v.begin(), v.end(), code);

    if (it == v.end()) {
        cout << "That book is not under your name.\n";
        return;
    }

    loadingDots("Processing");

    v.erase(it);
    bookStock[code]++;

    cout << "Returned. Smooth.\n";
}
void checkUser() {
    slowPrint("\n[Your Books]\n");

    string phone;
    cout << "Phone: "; cin >> phone;

    if (userBooks[phone].empty()) {
        cout << "No books right now.\n";
        return;
    }

    for (auto &c : userBooks[phone])
        cout << c << " | " << bookNames[c] << endl;
}

void undo() {
    slowPrint("\n[Undo]\n");

    if (undoStack.empty()) {
        cout << "Nothing to undo.\n";
        return;
    }

    string code = undoStack.top().second;
    undoStack.pop();

    auto it = find(bucket.begin(), bucket.end(), code);

    if (it != bucket.end()) {
        bucket.erase(it);
        cout << "Removed last selected book.\n";
    } else {
        cout << "That book is no longer in your list.\n";
    }
}

/* ========= SUBMENUS ========= */

void userMenu() {
    int ch;
    while (1) {
        cout << "\n-- USER --\n1. Register\n0. Back\nChoice: ";
        cin >> ch;
        if (ch == 0) return;
        if (ch == 1) registerUser();
    }
}

void searchMenu() {
    int ch;
    while (1) {
        cout << "\n-- FIND --\n1. Search\n0. Back\nChoice: ";
        cin >> ch;
        if (ch == 0) return;
        if (ch == 1) searchAndAdd();
    }
}

void borrowMenu() {
    int ch;
    while (1) {
        cout << "\n-- BORROW --\n";
        cout << "1. View List\n";
        cout << "2. Checkout\n";
        cout << "3. Undo Last Action\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> ch;

        if (ch == 0) return;
        else if (ch == 1) viewBucket();
        else if (ch == 2) checkout();
        else if (ch == 3) undo();
        else cout << "Invalid choice.\n";
    }
}

void returnMenu() {
    int ch;
    while (1) {
        cout << "\n-- RETURN --\n1. Return\n2. My Books\n3. Undo\n0. Back\nChoice: ";
        cin >> ch;
        if (ch == 0) return;
        if (ch == 1) returnBook();
        if (ch == 2) checkUser();
        if (ch == 3) undo();
    }
}

/* ========= MAIN ========= */

int main() {

    bookNames["B101"] = "math";
    bookStock["B101"] = 2;

    bookNames["B102"] = "physics";
    bookStock["B102"] = 2;

    bookNames["B103"] = "chemistry";
    bookStock["B103"] = 2;

    bookNames["B104"] = "biology";
    bookStock["B104"] = 2;

    bookNames["B105"] = "english";
    bookStock["B105"] = 2;

    bookNames["B106"] = "history";
    bookStock["B106"] = 2;

    /* CS & TECH */

    bookNames["B107"] = "data structures";
    bookStock["B107"] = 3;

    bookNames["B108"] = "algorithms";
    bookStock["B108"] = 2;

    bookNames["B109"] = "operating systems";
    bookStock["B109"] = 2;

    bookNames["B110"] = "database systems";
    bookStock["B110"] = 2;

    bookNames["B111"] = "computer networks";
    bookStock["B111"] = 2;

    bookNames["B112"] = "artificial intelligence";
    bookStock["B112"] = 2;

    bookNames["B113"] = "machine learning";
    bookStock["B113"] = 2;

    bookNames["B114"] = "software engineering";
    bookStock["B114"] = 2;

    /* PROGRAMMING */

    bookNames["B115"] = "c programming";
    bookStock["B115"] = 3;

    bookNames["B116"] = "c plus plus";
    bookStock["B116"] = 2;

    bookNames["B117"] = "java programming";
    bookStock["B117"] = 2;

    bookNames["B118"] = "python programming";
    bookStock["B118"] = 3;

    bookNames["B119"] = "web development";
    bookStock["B119"] = 2;

    /* ENGINEERING */

    bookNames["B120"] = "digital logic";
    bookStock["B120"] = 2;

    bookNames["B121"] = "microprocessors";
    bookStock["B121"] = 2;

    bookNames["B122"] = "computer architecture";
    bookStock["B122"] = 2;

    /* MATH */

    bookNames["B123"] = "discrete mathematics";
    bookStock["B123"] = 3;

    bookNames["B124"] = "linear algebra";
    bookStock["B124"] = 2;

    bookNames["B125"] = "probability";
    bookStock["B125"] = 2;

    bookNames["B126"] = "statistics";
    bookStock["B126"] = 2;

    /* NOVELS */

    bookNames["B127"] = "the hobbit";
    bookStock["B127"] = 3;

    bookNames["B128"] = "harry potter";
    bookStock["B128"] = 3;

    bookNames["B129"] = "game of thrones";
    bookStock["B129"] = 2;

    bookNames["B130"] = "lord of the rings";
    bookStock["B130"] = 2;

    bookNames["B131"] = "sherlock holmes";
    bookStock["B131"] = 3;

    bookNames["B132"] = "hamlet";
    bookStock["B132"] = 2;

    /* SELF HELP */

    bookNames["B133"] = "the alchemist";
    bookStock["B133"] = 3;

    bookNames["B134"] = "rich dad poor dad";
    bookStock["B134"] = 2;

    bookNames["B135"] = "atomic habits";
    bookStock["B135"] = 3;

    bookNames["B136"] = "deep work";
    bookStock["B136"] = 2;

    bookNames["B137"] = "ikigai";
    bookStock["B137"] = 2;

    bookNames["B138"] = "psychology of money";
    bookStock["B138"] = 2;

    for (auto &b : bookNames)
        trie.insert(b.second, b.first);

    int ch;

    while (true) {
        slowPrint("\n===== LIBRARY SYSTEM =====\n", 5);
        cout << "1. User\n2. Find\n3. Borrow\n4. Return\n0. Exit\nChoice: ";
        cin >> ch;

        switch(ch) {
            case 1: userMenu(); break;
            case 2: searchMenu(); break;
            case 3: borrowMenu(); break;
            case 4: returnMenu(); break;
            case 0: return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
