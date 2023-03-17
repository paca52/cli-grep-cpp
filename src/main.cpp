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
#define NO_OF_CHARS 256

// VARS
const size_t TAB_SIZE = 2;
int tab_num = 0;
int NAME_BEGIN = 0;
string srch;
string CURR_PATH;
vector<fs::path> Data;
Elements e;

// ** case sensitive search ** //
// int             CASE_SENSITIVE_SELECTED = 0;
// vector<string>  CASE_SENSITIVE_ENTRIES  = { "on", "off" };
// Component       CASE_SENSITIVE_OPT      = Toggle(&CASE_SENSITIVE_ENTRIES, &CASE_SENSITIVE_SELECTED);

  bool CASE_SENSITIVE_SELECTED = false;
  auto checkboxes = Container::Vertical({
    Checkbox("Case Sensitive Search", &CASE_SENSITIVE_SELECTED),
  });

namespace Util {
  // Children list for collapsable menu
  // src: https://arthursonzogni.github.io/FTXUI/examples_2component_2collapsible_8cpp-example.html#a5
  Component Inner(std::vector<Component> children) {
    Component vlist = Container::Vertical(std::move(children));
    return Renderer(vlist, [vlist] {
      return hbox({
          text(" "),
          vlist->Render(),
      });
    });
  }
  
  // empty element for collapsable menu 
  // src: https://arthursonzogni.github.io/FTXUI/examples_2component_2collapsible_8cpp-example.html#a5
  Component Empty() {
    return std::make_shared<ComponentBase>();
  }

  // getting data
  bool is_hidden(const fs::directory_entry& entry) {
    return 
      entry.path().filename().string()[0] == '.' ||
      entry.path().filename().string() == "build";
  }

  // for case sens. search
  void to_lower(string& str) {
    for(char& ch : str) {
      if(ch >= 'A' && ch <='Z') ch = 'a' + ch - 'A';
    }
  }
};

// OPTIONS and COMPONENTS
// INPUT
Component input = Input(&srch, "Name");

// ** collapsable menu for options ** //
Component collapable_menu = Collapsible(
  "Options",
  Util::Inner({
    checkboxes
  })
);



void getData(fs::path p = fs::path(CURR_PATH)) {
  for(const auto& entry : fs::directory_iterator(p)) {
    if(Util::is_hidden(entry)) continue;

    Data.push_back(entry.path());

    if(entry.is_directory()) {
      getData(entry.path());
    }
  }
}

void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS]) {
	for (int i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;
	for (int i = 0; i < size; i++)
		badchar[(int) str[i]] = i;
}

int search(string txt, string pat) {
 
  if(!CASE_SENSITIVE_SELECTED) {
    Util::to_lower(txt);
    Util::to_lower(pat);
  }

	int m = pat.size();
	int n = txt.size();

	int badchar[NO_OF_CHARS];

	badCharHeuristic(pat, m, badchar);

	int s = 0;
	while(s <= (n - m)) {
		int j = m - 1;

		while(j >= 0 && pat[j] == txt[s + j])
			j--;

		if (j < 0) {
      return s;
		} else {
			s += max(1, j - badchar[(int)txt[s + j]]);
    }
	}
  return -1;
}

void explore() {
  for(const auto& entry : Data) {
    string path = entry.string();
    string name (
      path.begin() + NAME_BEGIN + 1,
      path.end()
    );

    int found = search(name, srch);
    if(found != -1) {

      string pref = string(
        name.begin(),
        name.begin() + found
      );
      string suff = string(
        name.begin() + found + srch.size(),
        name.end()
      );

      if(fs::directory_entry(entry).is_directory()) suff += "\\";

      e.push_back(hbox({
        text(pref),
        text(( !CASE_SENSITIVE_SELECTED ? string(name.begin()+found, name.begin()+found+srch.size()) : srch )) | color(Color::Magenta),
        text(suff) | flex,
        separator(),
        text((fs::directory_entry(entry).is_directory() ? "folder" : "file")),
        separatorLight(),
        text(entry.relative_path()) | color(Color::DeepSkyBlue1)
      }));
    }

  }
}

int main(void) {
  CURR_PATH = fs::current_path().string();
  NAME_BEGIN = CURR_PATH.size();
  getData();

  // CONTAINER AND RENDERING
  auto container = Container::Vertical({
    input,
    collapable_menu
  });

  Elements vec;
  auto comp = Renderer(container, [&] {

    e.clear();
    explore();
    vec = e;
    return vbox({
      hbox(
        text("Input file name: "),
        input->Render() | bold | color(Color::Magenta)
      ),
      separatorLight(),
      collapable_menu->Render(),
      separator(),
      window(
        text("Files found"),
        vbox(vec) | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20)
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