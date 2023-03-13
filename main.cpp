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

void to_lower(string& str) {
    for(char& ch : str) {
        if(ch >= 'A' && ch <='Z') ch = 'a' + ch - 'A';
    }
}

void explore(fs::path p) {
    for(const auto& entry : fs::directory_iterator(p)) {
        if(is_hidden(entry)) continue;

        if(entry.is_directory()) {
            explore(entry.path());
        } else {
            string name = entry.path().filename().string();
            to_lower(name);

            if(name.find(search) != string::npos) {
                vec.push_back(File(name, entry.path().string()));
            }

        }
    }
}

int main(int argc, char **argv) {

    string path;

    if(argc != 2) {
        cout << "Nacin koriscenja: grep.exe naziv_fajla\n";
        return 0;
    }

    fs::path p = fs::current_path();
    search = argv[1];
    
    to_lower(search);
    explore(p);

    if(vec.size() == 0) {
        cout << "Nema takvog fajla" << "\n";
    } else { 
        cout << "Pronadjeno:\n";
        for(const File& f : vec) {
            cout << "\t" << f.path << "\n";
        } 
    }

    return 0;
}