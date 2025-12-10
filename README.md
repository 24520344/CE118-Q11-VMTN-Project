# [CE118.Q11.VMTN - Nhóm 1] Đồ án thiết kế vi xử lý 16-bit theo kiến trúc tập lệnh đơn giản

**Danh sách thành viên:**
```text
24520344 - Nguyễn Việt Dũng | Nhóm trưởng
24520301 - Tạ Vương Điền
24521177 - Huỳnh Hữu Nguyện
24520839 - Phan Công Đăng Khoa
24520816 - Lê Đăng Khoa
```

**Mô tả:** Đồ án này tập trung thiết kế và hiện thực một bộ vi xử lý (CPU) 16-bit hoàn chỉnh, bao gồm việc xây dựng trình biên dịch Assembler, thiết kế mạch phần cứng và mô phỏng kiểm tra hiệu năng.

## 📂 Tổng hợp
- **`Source Code:`**
  - `CPU_16bit_Assembler.cpp`: Trình biên dịch Assembler viết bằng C++, chuyển đổi mã Assembly từ tệp nguồn input.asm thành mã máy và xuất kết quả program.hex.

- **`Design Files:`**
  - Các mạch thiết kế (`.bdf`/ `.bsf`): Tập hợp các tệp thiết kế mạch nguyên lý và sơ đồ khối các thành phần của CPU 16-bit.
  - `clk.sdc`: Tệp ràng buộc thời gian, khởi tạo clock 50.0 MHz để kiểm thử thông số mạch.

- **`Simulation:`**
  - Các tệp mô phỏng (`.vwf`): Các tệp dạng sóng dùng để kiểm tra hoạt động của từng thành phần và toàn bộ CPU.

- **`Testbenches:`**
  - `program1.hex`: Chứa các lệnh số học (RRR).
  - `program2.hex`: Chứa các lệnh thao tác với bộ nhớ dữ liệu (DMem) và giá trị tức thời (Imm) (RRI + RI).
  - `program3.hex`: Chứa các lệnh giả và lệnh rẽ nhánh (beq/ jnz).
  - `program4.hex`: Chứa chương trình thao tác với I/O Controller.

## 🛠️ Công cụ sử dụng
- **Lập trình:** C++ (Viết Assembler).
- **Thiết kế phần cứng:** Quartus II 13.0sp1 (Dựa trên định dạng file .bdf, .vwf).
- **Mô phỏng:** University Program VWF.
- **Thiết kế slide:** Canva, PowerPoint.

## 📊 Hiệu năng vi xử lý


*Thực hiện bởi Nhóm 1 - CE118.Q11.VMTN*
