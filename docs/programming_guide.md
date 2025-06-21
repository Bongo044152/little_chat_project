# Programming Guide

## 1. Project Structure

```
/project/
├── .vscode/         # VSCode 設定檔（不影響編譯）
├── build/           # CMake build 輸出目錄（不納入版本控制）
├── cmake/           # CMake 自定義模組與第三方整合
├── docs/            # 專案文件與說明
├── include/         # 公開 header（可作為其他模組或專案使用）
├── lib/             # 自訂模組（如 logger、queue 等）
├── logs/            # 執行時 log 檔案輸出
├── playground/      # 實驗性代碼或開發草稿（可清除）
├── src/             # 主程式邏輯（包含 server / client 等）
├── test/            # 單元測試代碼與 doctest.hpp
├── .gitignore       # Git 忽略規則
├── CMakeLists.txt   # 主 CMake 組態
└── main.cpp         # 主程式進入點
```

---

## 2. Build Instructions

### ⚙️ 初始建置
( 首推 MinGW )
```bash
cmake -S . -B build -G "MinGW Makefiles"
```

### 🔨 編譯專案

```bash
cmake --build build
```

### ✅ 執行測試

```bash
cd build
ctest --output-on-failure
```

---

> 註：請勿直接在 `build/` 修改任何檔案，該目錄為 CMake 輸出目錄，可隨時清除重建。