#pragma once
#include <functional>
#include <vector>
#include <string>
#include "Imgui/imgui.h"

//#define printCon(x) ConsoleWindow::get().printText(std::string(x)) trash
#define printfCon(x, ...) ConsoleWindow::get().printf(x, __VA_ARGS__)

typedef std::vector<std::string> Arguments;
class ConsoleWindow
{
public:
    ~ConsoleWindow() {};
    static ConsoleWindow& get()
    {
        static ConsoleWindow wino;
        return wino;
    }


    void addCommand(std::string command, std::function<std::string(Arguments args)> func);
    void printText(std::string text);
    void printf(const char* format, ...);
    void update(bool setFocusOnTextbox);
    void runCommand(std::string command);

private:
    struct Command
    {
        std::string string;
        std::function<std::string(Arguments args)> func;

        Command(std::string s, std::function<std::string(Arguments args)> f)
        {
            this->string = s;
            this->func = f;
        }

        std::string operator()(Arguments args)
        {
            return func(args);
        }
    };

    struct LogEntry
    {
        ImVec4 color;
        std::string text;

        LogEntry(ImVec4 col, std::string string)
        {
            color = col;
            text = string;
        }
    };

    struct Colors 
    {
        ImVec4 errorColor;
        ImVec4 commandColor;
        ImVec4 infoColor;
        ImVec4 fromOutsideColor;
        ImVec4 windowColor;
    };

    std::vector<Command> commands;
    std::vector<LogEntry> log;
    std::vector<std::string> history;
    int historyPos;

    const int LOG_MAX = 3000;

    Colors colors;

    ConsoleWindow();

    void updateInputBox();

    int textEditCallback(ImGuiInputTextCallbackData* data);
    int textEditCallbackHistory(ImGuiInputTextCallbackData* data);
    int textEditCallbackComplete(ImGuiInputTextCallbackData* data);


    void addLog(std::string string, ImVec4 color = ImVec4(255, 255, 255, 255));
    void clearLog();
    void listAllCommands();

    static bool sortByLength(const std::string& a, const std::string& b)
    {
        return a.size() < b.size();
    }

    static std::string lowercase(std::string str)
    {
        std::string ret;

        for (char& c : str)
        {
            ret += std::tolower(c);
        }

        return ret;
    }
};