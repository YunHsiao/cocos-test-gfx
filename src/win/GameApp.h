#pragma once

#include "tests/TestBase.h"
#include <Windows.h>

namespace se {
class ScriptEngine;
}

namespace cc {
class FileUtils;

class GameApp {
public:
    static GameApp *getInstance() { return _instance; }
    GameApp();

    void Run();

    LRESULT CALLBACK MessageHandler(HWND hWnd, DWORD msg, WPARAM wParam, LPARAM lParam);

private:
    void initialize();
    void destroy();

    void createWindow(const std::wstring &name, int width, int height, float x, float y);

    static GameApp *_instance;

    bool _running = true;
    bool _paused = false;

    se::ScriptEngine* _se = nullptr;
    FileUtils* _fs = nullptr;

    HINSTANCE _hInstance = NULL;

    vector<HWND> _hWnds;
    vector<std::wstring> _windowNames;
};

} // namespace cc
