# Section 15

## 15-1

### a 

Shell: chmod u-rwx 9-1.txt   // Priviledge user

### b 
Shell: chmod 455 <folder>

Recover current: chmod 755 <folder>  

### c

- What permissions are required on the parent directory and the file itself in order to create a new file, open a file for reading, open a file for writing, and delete a file?
    - Directory: Write
    - File itself: Write
- What permissions are required on the source and target directory to rename a file?
    - Write and Execute
- If the target file of a rename operation already exists, what permissions are required on that file?
    - Write and Execute
- How does setting the sticky permission bit (chmod +t) of a directory affect renaming and deletion operations?
    - Sticky bit ngăn cản việc user xóa một file ko thuộc quyền owner của mình trong directory mà user đó có quyền write + execute
    - Enable sticky bit: 
    ```
    chmod +t <dir>
    ```

## 15-2

Khi gọi system call `stat()` thì không có timestamp nào trong 3 loại `st_atime`, `st_mtime` , `st_ctime` bị thay đổi, do `stat()` chị lấy thông tin file metadata chứ không thực hiện modify file.

## 15-5

Call `unmask(0)`, khi đó ko có bit bị xóa nên bit permission sẽ ko thay đổi, và `unmask` vẫn trả về permission bit hiện tại