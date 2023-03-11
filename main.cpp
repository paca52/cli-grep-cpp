#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
using namespace std;

namespace fs = std::filesystem;

const size_t TAB_SIZE = 2;
int tab_num = 0;

string search;

struct File {
    string name;
    string path;

    File() :name(""), path("") { }
    File(string name, string path) :name(name), path(path) { }
};

vector<File> vec;

bool is_hidden(const fs::directory_entry& entry) {
    return entry.path().filename().string()[0] == '.';
}

void explore(fs::path p) {
    for(const auto& entry : fs::directory_iterator(p)) {
        if(is_hidden(entry)) continue;

        if(entry.is_directory()) {
            explore(entry.path());
        } else {
            string name = entry.path().filename().string();

            if(name.find(search) != string::npos) {
                vec.push_back(File(name, entry.path().string()));
            }

        }
    }
}

int main(void) {

    string path;
    cout << "Uneti apsolutni ili relativni put do foldera: ";
    getline(cin, path);
    fs::path p(path);

    cout << "Unesi ime ili deo imena fajla koji trazis: ";
    getline(cin, search);

    explore(p);

    if(vec.size() == 0) {
        cout << "Nema takvog fajla" << "\n";
    } else { 
        cout << "Found:\n";
        for(const File& f : vec) {
            cout << "\t" << f.path << "\n";
        } 
    }

    return 0;
}