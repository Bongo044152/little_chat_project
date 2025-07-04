# 📝 PR 模板簡述

* **版本**：`v1`
* **目的**：統一 Pull Request 訊息格式，提升追蹤效率與可讀性
  （非強制，但建議依此撰寫）
* **說明**：

  * 每個 PR 請自行加上適當的 [**tag**](../../docs/programming_guide.md#3-常見-tag)
  * 若此 PR 關聯到某個 Issue，請在內文標註關聯（例如 `Closes #123`）
  * 🚩 **注意：PR 的主標題需使用英文**，內文可使用任意語言

---

> 💡 你可依需求自行擴充，例如：
>
> * 附加 GitHub Actions 訊息
> * 更具體的 bug 描述
> * 性能數據、測試報告
> * ...

---

## 📌 簡易說明

> （除了 PR 主標題要求英文，內文語言不限）

```
# 內文標題
< 簡要描述此 PR 做了什麼 >

< 細節補充，例如實作細節、使用的第三方庫、潛在風險等 >

---
關聯事項（PR/Issue）
例如：
- Closes #123
- Related PR: #45
```

---

## ✅ 範例

```
# Implement LRU cache for chat messages

- Add Redis-based caching for frequent messages
- Reduce DB writes by batching flush every 5 seconds
- Integrated with existing Session lifecycle

---
Related issue: #45
```