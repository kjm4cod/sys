/*
 * 1_pipe_basic.c — 파이프로 부모가 자식에게 문자열을 보내고 돌려받는다
 *
 * [핵심 개념]
 *   pipe(2) 는 파일 디스크립터 두 개를 준다: fd[0] 은 읽기, fd[1] 은 쓰기.
 *   fork 하면 자식이 이 디스크립터를 물려받으므로 부모-자식이 통신할 수 있다.
 *   각자 쓰지 않는 쪽은 반드시 닫아야 한다 — 쓰기 쪽이 모두 닫혀야 읽기 쪽이 EOF(0)를 본다.
 *   출처: man 2 pipe, man 7 pipe
 *
 * [컴파일·실행]
 *   gcc -Wall -Wextra -o 1_pipe 1_pipe_basic.c
 *   ./1_pipe "hello world"
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("사용법: %s <문자열>\n", argv[0]);
        return 1;
    }
    if (strlen(argv[1]) >= 64) {
        fprintf(stderr, "문자열은 63바이트 이하로 입력하세요.\n");
        return 1;
    }

    int fd[2];   /* fd[0] = 읽기 끝, fd[1] = 쓰기 끝이 여기 채워진다 */
    int back[2]; /* 자식에서 부모로 돌려줄 파이프 */
    if (pipe(fd) == -1) { perror("pipe"); return 1; }
    if (pipe(back) == -1) { perror("pipe"); close(fd[0]); close(fd[1]); return 1; }

    pid_t pid = fork();   /* fork 이후 부모·자식이 fd[0], fd[1] 을 그대로 물려받아 같이 갖는다 */
    if (pid == -1) {
        perror("fork");
        close(fd[0]); close(fd[1]);
        close(back[0]); close(back[1]);
        return 1;
    }

    if (pid == 0) {
        /* 자식 = 읽고 대문자로 바꿔 돌려주는 쪽. 사용하지 않는 끝을 닫는다. */
        close(fd[1]);
        close(back[0]);

        char buf[64];
        /* 파이프에서 읽는다. 마지막 한 자리는 남겨 뒤에 '\0'을 붙일 자리로 쓴다 */
        ssize_t n = read(fd[0], buf, sizeof buf - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("  [자식] 받은 내용: %s\n", buf);
            for (ssize_t i = 0; i < n; i++)
                buf[i] = (char)toupper((unsigned char)buf[i]);
            write(back[1], buf, (size_t)n);   /* 대문자로 바꿔 부모에게 돌려준다 */
        }
        close(fd[0]);
        close(back[1]);
        fflush(stdout);   /* _exit 은 버퍼를 비우지 않는다 — 직접 비워야 출력이 남는다 */
        _exit(0);
    }

    /* 부모 = 문자열을 보내고 돌려받는 쪽. 사용하지 않는 끝을 닫는다. */
    close(fd[0]);
    close(back[1]);

    const char *msg = argv[1];
    write(fd[1], msg, strlen(msg));   /* 널 종료 문자는 보내지 않는다 — 길이만큼만 씀 */
    close(fd[1]);   /* 닫아야 자식의 read 가 끝난다 */

    char buf[64];
    ssize_t n = read(back[0], buf, sizeof buf - 1);
    if (n > 0) {
        buf[n] = '\0';
        printf("[부모] 돌려받은 내용: %s\n", buf);
    }
    close(back[0]);

    wait(NULL);
    printf("[부모] 전송 완료\n");
    return 0;
}
