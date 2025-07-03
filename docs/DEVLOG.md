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
  * 發現 **Segmentation Fault**。   -- BUG-001: logger 在 addSink 後觸發了 Segmentation Fault
  * 使用 `gdb` 除錯，原因是誤引用 `nullptr`。 -- Fix #001: 引用了 `nullptr`
* 完成 logger 並整理專案架構，首次使用 Git 進行版本控管。

---

## 🗓️ 6/15：logger 測試與 socket 問題修復

* Roger 測試 logger 與 queue：
  * **單線程表現良好**，但在 multi-thread 下出現輸出錯誤。
  * 原因：logger 中 `sink` 等元件未加鎖，導致 race condition。  -- BUG-002: logger 的 `sink` 元件寫入文件產生了 race condition
  * 檢查到物件的 copy 與 move 並沒有正確地被刪除  -- Fix #002: 為 `sink` 上鎖，並且 singletone
  * 已修復並提交 Git。

* 進行 socket 測試：
  * 發現文字解析錯誤。  -- BUG-003: socket 連線時，文字解析錯誤，產生如 � 的亂碼
  * 問題原因：buffer 沒有妥善清除，導致殘留資料干擾。 -- Fix #003: 儲存字元的 buffer 沒有被妥善清除。
  * 解決方案：改用 `std::vector<char>` 處理傳輸文字，確保清除機制。

---

## 🗓️ 6/16：引入測試框架與模組封裝 [小專案第一次革命 - 1]

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

## 🗓️ 6/21：更新專案架構 [小專案第一次革命 - 2]

* 更改專案架構
  * 將 logger 與 queue 獨立到 lib 中
  * 新增 cmake 資料夾表示第三方 library
  * 使用 cmake 管理 library，包括 FTXUI
  * 新增正在實驗中的 server 與 client 邏輯 -> 添加到 src 與 include

* Roger 完成了 Programming guide
* 張宸瑋 完成了 恭迎大周和裸男的 onbording plan: https://www.notion.so/2198413ce7ec806a9ce2f4bc5d23965f
* 前端 UI 分業實驗完成
  * menu page 初步實驗成功: 允許自由的下一頁和上一頁
  * 添加好友初步實驗: 使用 ftxui::Toggle 來當作選擇
  * 傳送訊息初步實驗: 雖然輸入輸出不是很完美，但是能用

## 🗓️ 6/22：測試連線、思考下一步計畫

* 原本預定晚上測試 VPN 連線，看看延遲等 (參與人員: 張宸瑋、Roger)；簡稱 「VPN 連線測試」
  結果因為某些因素沒有做。
  * 進行 VPN 連線之前，張宸瑋 發現了一些 bug: 有些字元解析出現問題，出現 '�'  -- BUG-004: 錯誤的 socket 連線邏輯
    初步推測是因為內容沒有傳遞完全 -> 存放字元的 `buffer` 有問題
  * 後來認為是 io 有問題
  * 最後發現網路連線有問題，`recv` 在不正確的時機接收了自己發送出去的訊息

* 新人引入計畫: 
  1. 講解專案架構
  2. 介紹 json 格式，以及它在本專案有什麼用 (socket 傳遞訊息的首選)
  3. 這個 library 要怎麼用?
  4. 除錯能力 -> 如何對程式 debug (簡單介紹 vscode: include 路徑問題解決、lunch.json、task.json、怎麼用 vscode 抓 bug)
  5. 基本多線程概念: https://www.youtube.com/watch?v=4rLW7zg21gI <!-- 需要準備 -->
  6. 基本 socket 概念: https://www.geeksforgeeks.org/computer-networks/socket-in-computer-network/ <!-- 需要準備 -->
  7. API 的概念: https://www.geeksforgeeks.org/what-is-an-api/  <!-- 需要準備 -->

* 新人引入計畫: (針對大周)
  1. 介紹前端近況
  2. 介紹 FTXUI，以及為什麼我們需要它
  3. 這個 library 怎麼使用?

* 新人引入計畫: (針對裸男)
  1. 介紹後端近況、架構
  2. 介紹工作內容
  3. 講解 C++ 多線程

* 準備討論議題: 目前小專案遇到的困難
  * 對話紀錄如何儲存? 要儲存什麼?
  * 前端如何獲取對話紀錄?
  * API 要怎麼設計?
  * 我們要怎麼合作? 文件要怎麼管理? 開會頻率怎麼說?
  * <font color="red"><b>周思妤不讀訊息的問題</b></font>

## 🗓️ 6/23：修復錯誤、更近 onbording plan

* -- Fix #004: 錯誤的訊息輸出，以及錯誤的傳送訊息邏輯
  * server 輸出了錯誤的訊息，導致判斷失誤
  * send 函數的 bufferlen 寫錯

* onbording plan 需要製作一些教材、準備，防止出現問題 ( 張宸瑋 )
  * 弄個資料夾或者備忘錄
  * 平時有時間就自己潤稿

## 🗓️ 6/25：添加 git/github 教學 (備案)

* 添加 git/github 的教學: https://www.notion.so/2198413ce7ec806a9ce2f4bc5d23965f

## 🗓️ 6/27：VPN 測試成功、小專案第二次革命

* 測試 VPN 連線成功 (參與者: 張宸瑋 和 Roger)
  * 能夠正常的收發訊息
  * 連線沒有明顯的延遲

* 第二次格命的細項:
  * 建立討論用 dc 群組，取代現用的通訊軟體: Line
  * 經過考慮，正式建立 github 倉庫
  * 架設稍微完整體系的任務追蹤與溝通方式

## 🗓️ 6/28：大周和裸男第一次出現

* 會議參與者: (張宸瑋、大周、裸男)
  * 章節推進到【手動演示編譯流程】
  * 預計下一次持續到進度【函式庫介紹(可選)】
  * 需要更多實戰練習，稱之為專案前小練習；預計使用李伯營以前的作業: https://www.notion.so/1f78413ce7ec804b98d1cc8a8b779b2c
  * 下一次開會時間是 7/1(二) 上午 (大周說不要晚上)
