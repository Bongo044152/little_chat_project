# Programming Guide

## 1. Project Structure

```
/little_chat_project/
├── client/           # Client 端源碼
├── server/           # Server 端源碼
├── docs/             # 說明文件、協議設計、架構圖
├── lib/              # 共用模組（logger、thread-safe queue、第三方 library 等）
├── logs/             # 執行時產生的 log 檔案
├── test/             # 單元測試（doctest, mock 專用）
├── .gitignore        # Git 忽略規則
├── CMakeLists.txt    # 頂層 CMake 組態
└── README.md         # 專案介紹與快速開始
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

> 註：請勿直接在 `build/` 修改任何檔案，該目錄為 CMake 輸出目錄，可隨時清除重建。