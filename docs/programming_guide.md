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

## 3. 常見 tag

在撰寫 commit / PR / issue 訊息時，請在標題加上適當的 **tag**，可快速辨識變更類型與影響範圍。
以下為專案常用的 tag 與說明，請依照實際情況選擇：

| Tag        | 說明                           | 範例                                     |
| ---------- | ---------------------------- | -------------------------------------- |
| `feat`     | ✨ 新功能（Feature）               | `feat: add user login API`             |
| `fix`      | 🐛 修復 bug（Bugfix）            | `fix: handle null pointer on logout`   |
| `docs`     | 📝 文件修改（Documentation）       | `docs: update README`                  |
| `style`    | 💄 程式碼格式（不影響行為）              | `style: format code with clang-format` |
| `refactor` | 🔨 重構（非修 bug/非新功能）           | `refactor: split user service`         |
| `perf`     | 🚀 效能優化（Performance）         | `perf: improve DB query speed`         |
| `test`     | ✅ 測試（Test cases）             | `test: add unit tests for cache`       |
| `chore`    | 🔧 其他雜項（建構、工具、CI/CD）         | `chore: bump cppcheck version`         |
| `ci`       | 🛠 持續整合、部署設定（CI/CD）          | `ci: update GitHub Actions workflow`   |
| `build`    | 📦 建構相關（如 makefile, package） | `build: update makefile target`        |
| `revert`   | ⏪ 還原先前提交                     | `revert: feat add login`               |

---

### ✅ 小提醒

* **commit message** 標題請以 `tag: description` 格式撰寫
* 一個 commit message 只允許有 1 個 tag，如果有很多個 tag，分次提交會比較好
* `tag` 只寫一個主要目的，必要時可在詳細描述列出副作用
* 在 PR 內文中可再補充更多上下文或關聯的 issue / PR
