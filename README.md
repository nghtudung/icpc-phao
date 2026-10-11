# Phao ICPC của HSGSBN.DjmNatBu 2026

2026 HSGSBN.DjmnatBu's ICPC Notebook

## Một số thông tin có thể hữu ích :D

| Đường dẫn  | Nội dung                                                                                                                            |
| -------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| `main.tex`   | File LaTeX chính: trang bìa, mục lục, định dạng trang và thứ tự ghép các phần.                                          |
| `topics/`    | Các phần lý thuyết bằng LaTeX. Mỗi file tương ứng một chủ đề; những mẫu code được in vào tài liệu từ`code/`. |
| `code/`      | Các mẫu code C++ theo chủ đề. Phần đầu chung của mỗi mẫu được ẩn khi in trong PDF.                                    |
| `assets/`    | Chỗ để hình và sơ đồ dùng trong tài liệu; hiện có file hướng dẫn`README.md`.                                       |
| `tests/`     | Kiểm thử các mẫu code. Các file`.cpp` chứa ca kiểm thử; `run.py` chạy toàn bộ bộ kiểm thử.                         |
| `build/`     | File sinh ra khi biên dịch LaTeX, gồm`main.pdf` và các file trung gian; thư mục này được bỏ qua trong Git.             |
| `.gitignore` | Danh sách file tạo tự động không đưa vào Git.                                                                               |
| `LICENSE`    | Giấy phép Apache 2.0 của repo.                                                                                                    |

Các file trong `topics/` theo thứ tự xuất hiện trong notebook:

| File                    | Chủ đề                      |
| ----------------------- | ------------------------------ |
| `geometry.tex`        | Hình học.                    |
| `probability.tex`     | Xác suất và kỳ vọng.      |
| `game_theory.tex`     | Lý thuyết trò chơi.        |
| `combinatorics.tex`   | Tổ hợp.                      |
| `number_theory.tex`   | Số học và số nguyên lớn. |
| `linear_algebra.tex`  | Đại số tuyến tính.        |
| `flow.tex`            | Luồng và lát cắt.          |
| `matching.tex`        | Cặp ghép.                    |
| `graph_logic.tex`     | Logic trên đồ thị, 2-SAT.  |
| `strings.tex`         | Xử lý chuỗi.                |
| `dp_optimization.tex` | Tối ưu quy hoạch động.    |
| `quick_reference.tex` | Các ghi chú tra cứu nhanh.  |

Trong `code/`, các thư mục mang tên chủ đề tương ứng với `topics/`;
`quick_reference/pragmas.txt` là danh sách pragma và ghi chú về cách dùng.
Các file `README.md` nằm trong những thư mục con ghi thêm phạm vi của mẫu code tại đó.

Trong `tests/`, `geometry.cpp` và `probability.cpp` kiểm tra hai phần tương ứng;
`notebook_sections.cpp` kiểm tra nhiều mẫu khác; `audit_regressions.cpp`
kiểm tra các trường hợp biên và lỗi đã từng gặp. `tests/README.md` giải thích
bộ kiểm thử chi tiết hơn.

---

2026 HSGSBN.DjmNatBu

Last modified: 11/10/2026

Built for:

* The 2026 ICPC Vietnam Northern Provincial Programming Contest - Khu giảng đường 4 UET (VNU), 12 Kiều Mai, phường Xuân Phương, thành phố Hà Nội 11/10/2026
  * Result: # 49 OVR, # 18 HS, # 2 BN
