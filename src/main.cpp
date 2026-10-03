#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <fstream>
#include <iostream>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace ftxui;

struct TaskItem {
  std::string title;
  bool done = false;
};

struct TaskList {
  std::string name;
  std::vector<TaskItem> tasks;
};

int main() {
  auto screen = ScreenInteractive::TerminalOutput();

  std::vector<TaskList> lists = {
      {"Life",
       {{"Life task 1"},
        {"Life task 2"},
        {"Life task 3"},
        {"Life task 4"},
        {"Life task 5"}}},
      {"School",
       {{"School task 1"},
        {"School task 2"},
        {"School task 3"},
        {"School task 4"},
        {"School task 5"}}},
  };

  std::vector<std::string> list_names;
  for (const auto &list : lists) {
    list_names.push_back(list.name);
  }

  int selected_list = 0;
  auto menu = Menu(&list_names, &selected_list);

  ButtonOption opt;
  opt.transform = [](const EntryState &state) {
    Element e = text("+ New List");
    if (state.focused) {
      e = text("+ New List") | bold | color(Color::Cyan);
    } else {
      e = text("+ New List") | dim;
    }
    return e;
  };

  auto add_list_button = Button("+ New List", [&] {}, opt);
  auto tabs = Container::Vertical({
      menu,
      // filler(),
      add_list_button,
  });

  auto tab_bar = Renderer(tabs, [&] {
    return vbox({
        menu->Render(),
        filler(),
        separatorLight(),
        add_list_button->Render(),
    });
  });

  auto task_tabs = Container::Tab({}, &selected_list);
  for (auto &list : lists) {
    auto task_container = Container::Vertical({});
    for (auto &task : list.tasks) {
      task_container->Add(Checkbox(&task.title, &task.done));
    }
    task_tabs->Add(task_container);
  }

  auto layout = Container::Horizontal({
      // menu,
      tab_bar,
      task_tabs,
  });

  auto renderer = Renderer(layout, [&] {
    return vbox({
               text("TuDo - " + list_names[selected_list]) | bold |
                   color(Color::Cyan),
               separator(),
               hbox({
                   tab_bar->Render(),
                   separator(),
                   task_tabs->Render() | frame | size(HEIGHT, LESS_THAN, 8) |
                       flex,
               }),
           }) |
           border | size(WIDTH, EQUAL, 60);
  });

  auto catch_close = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('q')) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  // screen.Loop(catch_close);

  struct passwd *pw = getpwuid(getuid());
  const char *homedir = pw->pw_dir;
  std::ofstream json_file(std::string(homedir) + "/tudo.json");

  if (!json_file.is_open()) {
    std::cout << "Could not open/create json file" << std::endl;
  }
  return 0;
}
