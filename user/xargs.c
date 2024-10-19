#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char buf[512];
    int n;
    char *xargv[MAXARG];
    int i, start_idx = 1;

    // Kiểm tra nếu người dùng truyền tùy chọn -n 1
    if (argc >= 3 && strcmp(argv[1], "-n") == 0 && atoi(argv[2]) == 1) {
        start_idx = 3;  // Bắt đầu sau -n 1
    }

    // Sao chép lệnh truyền vào, bỏ qua tùy chọn -n 1 nếu có
    for (i = start_idx; i < argc; i++) {
        xargv[i - start_idx] = argv[i];
    }

    // Đọc dữ liệu từ stdin
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        buf[n] = '\0';  // Kết thúc chuỗi
        char *p = buf;

        // Xử lý từng dòng
        while (*p) {
            char *q = p;
            while (*q != '\n' && *q != '\0') {
                q++;
            }
            *q = '\0';  // Kết thúc chuỗi

            // Thêm đối số từ stdin vào mảng argv
            xargv[argc - start_idx] = p;
            xargv[argc - start_idx + 1] = 0;  // Kết thúc mảng argv bằng NULL

            // Tạo tiến trình con để thực hiện lệnh
            if (fork() == 0) {
                exec(xargv[0], xargv);  // Thực hiện lệnh với đối số
                exit(0);  // Tiến trình con kết thúc
            } else {
                wait(0);  // Tiến trình cha chờ tiến trình con hoàn tất
            }

            p = q + 1;  // Tiếp tục với dòng tiếp theo
        }
    }

    exit(0);
}
