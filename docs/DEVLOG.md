# 開發日誌

## 🗓️ 6/1 ~ 6/13：前期構思與技術選型

* 因應期末考試，尚未正式動工。
* 思考技術選型與可行性：
  * **Server 端通訊技術**：使用 `Winsock` 搭配 **multi-threading** 以支援多用戶連線。
  * **前端 UI**：透過 `shell commend` 控制終端機顯示，實作基本互動介面。 ( 原本打算使用 `Windows.h` 提供的 API 對終端機進行操作，後來在教授的建議下放棄 )

---

## 🗓️ 6/13：佇列系統初步實作

* 開始撰寫 **MT-safe queue**，但 `pop()` 時發生錯誤。

---

## 🗓️ 6/14：佇列修復與 logger 開始實作

* 修復 MT-safe queue：
  * 問題：在同一 thread 上重複上鎖導致死鎖。
  * 解決方案：改良架構，去除「查看不 pop」功能，避免資源競爭。

* 開始實作 logger：
  * 發現 **Segmentation Fault**。
  * 使用 `gdb` 除錯，原因是誤引用 `nullptr`。
* 完成 logger 並整理專案架構，首次使用 Git 進行版本控管。

---

## 🗓️ 6/15：logger 測試與 socket 問題修復

* Roger 測試 logger 與 queue：
  * **單線程表現良好**，但在 multi-thread 下出現輸出錯誤。
  * 原因：logger 中 `sink` 等元件未加鎖，導致 race condition。
  * 檢查到物件的 copy 與 move 並沒有正確地被刪除
  * 已修復並提交 Git。

* 進行 socket 測試：
  * 發現文字解析錯誤。
  * 問題原因：buffer 沒有妥善清除，導致殘留資料干擾。
  * 解決方案：改用 `std::vector<char>` 處理傳輸文字，確保清除機制。

---

## 🗓️ 6/16：引入測試框架與模組封裝

* 引入 **doctest** 作為測試框架：
  * 新增 `test/` 資料夾。
  * 將 logger 獨立為模組，封裝成 library。
  * 發生 `doctest` define 重複執行導致 Link Error，已修復。

* 封裝 client-side 程式碼：
  * `ClientSocket` 封裝成 class，方便管理。

* 發現 UI 問題：
  * `cin` 與 `cout` 無法同時在終端顯示，UX 極差。

* 委派 Roger 撰寫 logger 註解。
* 封裝 server 程式碼，但對於多線程效能仍有疑慮。

---

## 🗓️ 6/17：改善 UX 與 server 非阻塞解決方案

* 引入新 UI Library：**FTXUI** 改善終端機使用者體驗。
* Server 端研究非阻塞 socket 解法：

  * [參考資料：StackOverflow - 非阻塞 `recv`](https://stackoverflow.com/questions/28636286/can-a-socket-be-made-non-blocking-only-for-the-recv-function)

---

## 🗓️ 6/18：問題解決與專案進展

* 諮詢教授，解決多個問題： (張宸瑋)
  * **Server 多線程效能疑慮解除**：確認資源消耗可接受。
  * **FTXUI 渲染挑戰**：修復了 UI 渲染的問題

* 學長建議 Git commit 加上標籤格式（如 `[Fix]`）。
* 添加教授與助教的 Discord 聯絡方式。
* 完成 server 多人連線測試與封裝。

---

## 🗓️ 6/19：socket 狀態管理問題

* 遇到一個新問題：`ServerSocket` 封裝雖獨立，但限制了 socket 間互動能力。 (張宸瑋)
  * 例如：來自 A socket 的訊息無法輕易透過 B socket 傳送。
  * 正在思考解決架構與訊息轉送設計。

* Roger 補上了所有的 logger 的註解

## 🗓️ 6/20：迎接大周與裸男的會議 (1) 、 解決 socket 狀態管理問題

* 會議事項: (參與者: 張宸瑋、Roger)
  * 文件交給 Roger 撰寫，張宸瑋 做檢查
  * 由 張宸瑋 負責對新人 onboarding

* 與教授的討論 ( 解決 socket 狀態管理問題 )
  * 基本兩種解決方案:
    1. ServerSocket 向 Server 做 polling
    2. ServerSocket 做 callback ( Server 提供 callback function ): 對於這個方案，教授建議我們加上 time limit 以免 callback 過程中出現問題

## 🗓️ 6/21：

* 更改代碼架構，更近 cmake 的邏輯
  * 將 logger 與 queue 獨立到 lib 中
  * 新增 cmake 資料夾表示第三方 library
  * 使用 cmake 管理 library
  * 新增正在實驗中的 server 與 client 邏輯 -> 位於 src 與 include