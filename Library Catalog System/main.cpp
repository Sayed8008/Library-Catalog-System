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
