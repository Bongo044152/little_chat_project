// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <iostream>  // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream
#include <string>    // for string, basic_string, allocator
#include <vector>    // for vector

#include "ftxui/component/captured_mouse.hpp"      // for ftxui
#include "ftxui/component/component.hpp"           // for Menu
#include "ftxui/component/component_options.hpp"   // for MenuOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

#include "ftxui/component/component.hpp"       // for Toggle, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, vbox, Element
#include "ftxui/screen/color.hpp"  // for Color

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace ftxui;

int menu_page(ScreenInteractive &screen)
{
    screen.Clear();

    std::vector<std::string> entries = {
        "manage friend",
        "chat",
    };
    int selected = -1;
    bool ex = false;

    auto exit_loop = screen.ExitLoopClosure();

    MenuOption option = MenuOption::Vertical();
    auto menu =
        Menu(&entries, &selected, option) | CatchEvent([&](Event event) {
            if (event == Event::Escape) {
                exit_loop();
                ex = true;
                return true;
            }
            return false;
        });


    Component component =
        menu | CatchEvent([&](Event event) {
            if (event == Event::Return) {
                exit_loop();
                return true;
            }

            if (event.is_mouse() && event.mouse().button == Mouse::Left &&
                event.mouse().motion == Mouse::Pressed) {
                exit_loop();
                return false;
            }

            if (event == Event::Escape) {
                exit_loop();
                ex = true;
                return true;
            }
            return false;
        });

    screen.Loop(component);

    screen.Clear();
    return ex ? -1 : selected;
}

void chat_page(ScreenInteractive &screen, const std::string name)
{
    screen.Clear();
    system("cls");

    std::vector<std::string> chat_history;
    std::string input_text;

    // --- 輸入框元件 --- //
    auto input =
        Input(&input_text, "Type a message...") | CatchEvent([&](Event e) {
            if (e == Event::Return && !input_text.empty()) {
                chat_history.push_back("你說: " + input_text);
                input_text.clear();
                return true;
            }
            return false;
        });

    // --- 聊天紀錄顯示元件 --- //
    auto chat_box = Renderer([&] {
        std::vector<Element> lines;
        for (const auto &msg : chat_history)
            lines.push_back(text(msg));

        return window(text("Chat History") | bold | hcenter,
                      vbox(std::move(lines))) |
               size(HEIGHT, GREATER_THAN, 20);
    });

    auto inner = Container::Vertical({chat_box, input});

    Component renderer = Renderer(inner, [&] {
        return window(text(name) | bold,
                      vbox({chat_box->Render(),
                            hbox(text("輸入訊息: "), input->Render())}));
    });

    renderer |= CatchEvent([&](Event e) {
        if (e == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
    });

    // 模擬 Server 發送訊息
    std::thread server(
        [](std::vector<std::string> &chat_history, ScreenInteractive &screen) {
            for (size_t i = 0; i < 10; ++i) {
                std::string out_message = "[SERVER] test: " + std::to_string(i);
                chat_history.push_back(out_message);
                screen.PostEvent(Event::Custom);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        },
        std::ref(chat_history), std::ref(screen));
    server.detach();

    screen.Loop(renderer);
    screen.Clear();
    system("cls");
}

void add_friend_page(ScreenInteractive &screen, const std::string applicant)
{
    screen.Clear();
    system("cls");

    int selected = 1;
    std::vector<std::string> options = {
        "Yes",
        "No",
    };

    auto screen_exit = screen.ExitLoopClosure();

    Component toggle =
        Toggle(&options, &selected) | CatchEvent([&](Event e) {
            if (e.is_mouse() && e.mouse().button == Mouse::Left &&
                e.mouse().motion == Mouse::Pressed) {
                screen_exit();
                return false;
            }
            return false;
        });


    auto rander = Renderer(toggle, [&] {
        return hbox(text(applicant + " 想添加你為好友: "), toggle->Render());
    });

    rander |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            screen_exit();
            return true;
        } else if (event == Event::Return) {
            screen_exit();
        }
        return false;
    });

    screen.Loop(rander);

    screen.Clear();
    system("cls");

    if (selected == 1) {
        std::cout << "你居然拒絕!!!" << std::endl;
        exit(-1);
    }
}

// int main() {
//     system("cls");
//     ScreenInteractive screen = ScreenInteractive::TerminalOutput();
//     screen.Clear();

//     while(true) {
//         int selected = menu_page(screen);
//         if(selected == 1) {
//             chat_page(screen, "裸男");
//         }
//         else if(selected == 0){
//             add_friend_page(screen, "Austin Yang");
//         }
//         else if(selected == -1) {
//             exit(-1);
//         }
//     }

//     return 0;
// }
