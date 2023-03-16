#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
// #include "ftxui/component/loop.hpp"
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

void to_lower(string& str) {
  for(char& ch : str) {
    if(ch >= 'A' && ch <='Z') ch = 'a' + ch - 'A';
  }
}

Elements e;
void explore(fs::path p) {
  for(const auto& entry : fs::directory_iterator(p)) {
    if(is_hidden(entry)) continue;

    if(entry.is_directory()) {
      explore(entry.path());
    } else {
      string name = entry.path().filename().string();
      to_lower(name);

      if(name.find(srch) != string::npos) {
        e.push_back(hbox({
          text(entry.path().filename().string()) | flex,
          separator(),
          text(entry.path().string())
        }));
      }

    }
  }
}

int main(void) {
  string inp;
  Component input = Input(&inp, "Name");

  fs::path p = fs::current_path();

  Elements vec;
  auto comp = Renderer(input, [&] {
    return vbox({
      hbox(text("Input file name")),
      hbox({
        input->Render() | dim | bold
      }),
      window(
        text("Files found"), vbox(vec)
      ),
    }) | border;
  });

  comp |= CatchEvent([&](Event event) {
    if(event == Event::Return) {
      e.clear();
      srch = inp;
      to_lower(srch);
      explore(p);
      vec = e;
      return true;
    }
    return false;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(comp);

  return 0;
}