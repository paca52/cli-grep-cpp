#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

using namespace std;
using namespace ftxui;
namespace fs = std::filesystem;

const size_t TAB_SIZE = 2;
int tab_num = 0;
string srch;

bool is_hidden(const fs::directory_entry& entry) {
  return 
    entry.path().filename().string()[0] == '.' ||
    entry.path().filename().string() == "build";
}

string to_lower(string str) {
  for(char& ch : str) {
    if(ch >= 'A' && ch <='Z') ch = 'a' + ch - 'A';
  }
  return str;
}

vector<fs::path> Data;

void getData(fs::path p = fs::current_path()) {
  for(const auto& entry : fs::directory_iterator(p)) {
    if(is_hidden(entry)) continue;

    Data.push_back(entry.path());

    if(entry.is_directory()) {
      getData(entry.path());
    }
  }
}

Elements e;
void explore() {
  for(const auto& entry : Data) {
    string name = entry.filename().string();

    if(to_lower(name).find(to_lower(srch)) != string::npos) {
      // string pref = "", suff = "";
      // pref = string(name.begin(), name.begin()+name.find(srch));
      // if(srch.length() != 1)
      //   suff = string(name.begin()+name.find(srch)+srch.length(), name.end());

      // if(fs::directory_entry(entry).is_directory()) suff += "/";

      e.push_back(hbox({
        // text(pref),
        // text(srch) | color(Color::Magenta),
        // text(suff) | flex,
        text(name) | flex,
        separator(),
        text((fs::directory_entry(entry).is_directory() ? "folder" : "file")),
        separatorLight(),
        text(entry.string()) | color(Color::DeepSkyBlue1)
      }));
    }

  }
}

int main(void) {
  Component input = Input(&srch, "Name");

  getData();

  Elements vec;
  auto comp = Renderer(input, [&] {

    e.clear();
    explore();
    vec = e;
    return vbox({
      hbox(
        text("Input file name: "),
        input->Render() | bold | color(Color::Magenta)
      ),
      separatorLight(),
      window(
        text("Files found"), vbox(vec)
      ),
    }) | border | color(Color::LightSkyBlue1);
  });

  // comp |= CatchEvent([&](Event event) {
  //   if(event == Event::Return) {
  //     e.clear();
  //     explore();
  //     vec = e;
  //     return true;
  //   }
  //   return false;
  // });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(comp);

  return 0;
}