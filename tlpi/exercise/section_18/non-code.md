### 18-1
2 file đều refer đến cùng 1 inode number trong inode table

### 18-2
ownership của symlink bị bỏ qua, nên ko thể set chmod cho symlink

-> Sai
Symlink có cấu trúc giống như đường dẫn relative tới parent directory, sau đó khi chmod thực thi, nó đọc symlink như 1 relative path và ko tìm thấy file trong relative path đó.