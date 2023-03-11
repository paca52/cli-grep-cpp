#include <iostream>
#include <string>
#include <filesystem>
using namespace std;

namespace fs = std::filesystem;

string path;

bool is_hidden(const fs::directory_entry& entry) {
    // cout << "test_path: " << entry.path() << endl;
    // cout << "test: " <<  entry.path().filename().string()[0] << endl;
    // cout << endl;
    return entry.path().filename().string()[0] == '.';
}

int br = 0;
void explore(fs::path p) {
    for(const auto& entry : fs::directory_iterator(p)) {
        if(is_hidden(entry)) continue;
        int i = 0;
        while(i++ < br) cout << "\t";
        cout << entry << endl;
        if(entry.is_directory()) {
            br += 1;
            explore(entry.path());
            br -= 1;
        }
        // if(!entry.exists()) continue;
        // if(entry.is_directory()) {
        //     br += 1;
        //     explore(p.append(entry.path().filename().string()));
        //     br -= 1;
        // } else {
        //     cout << entry.path() << endl;
        // }
    }
}

int main(void) {

    cout << "Uneti apsolutni ili relativni put do foldera: ";
    cin >> path;
    fs::path p(::path);

    cout << "files:\n";
    explore(p);
    // for(const auto& entry : fs::directory_iterator(path)) {
    //     if(entry.is_directory()) {
    //     }
    //     if(is_hidden(entry)) {
    //         continue;
    //     }
    //     cout << entry.path() << endl;
    // }

    return 0;
}