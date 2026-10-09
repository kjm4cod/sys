/*
 * 2_alarm.c — 정해진 시간 뒤에 시그널을 받는다
 *
 * [핵심 개념]
 *   alarm(n) 은 n 초 뒤에 커널이 SIGALRM 을 보내도록 예약한다.
 *   시간 제한(타임아웃)을 구현하는 가장 간단한 방법이다.
 *   출처: man 2 alarm, man 2 sigaction
 *
 * [컴파일·실행]
 *   gcc -Wall -Wextra -o alarm 2_alarm.c
 *   ./alarm 2 5      # 2초 간격으로 5번 알람이 울린다
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static volatile sig_atomic_t timeout = 0;
static unsigned int interval;
static int repeat;

static void on_alarm(int sig)
{
    (void)sig;
    timeout++;

    if (timeout < repeat)
        alarm(interval);   /* 다음 SIGALRM 을 다시 예약한다 */
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("사용법: %s <간격초> <반복횟수>\n", argv[0]);
        return 1;
    }

    int seconds = atoi(argv[1]);
    repeat = atoi(argv[2]);

    if (seconds <= 0 || repeat <= 0) {
        printf("간격초와 반복횟수는 1 이상이어야 합니다.\n");
        return 1;
    }

    interval = (unsigned int)seconds;

    struct sigaction sa;
    sa.sa_handler = on_alarm;   /* SIGALRM 이 오면 이 함수를 부르게 등록한다 */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;   /* SA_RESTART 를 안 줬으므로, 시그널이 대기 중인 호출을 중단시킬 수 있다 */
    sigaction(SIGALRM, &sa, NULL);

    sigset_t block, old, wait_mask;
    sigemptyset(&block);
    sigaddset(&block, SIGALRM);

    /* SIGALRM 을 차단하고 원래 마스크를 저장한다 */
    if (sigprocmask(SIG_BLOCK, &block, &old) == -1) {
        perror("sigprocmask");
        return 1;
    }

    wait_mask = old;
    sigdelset(&wait_mask, SIGALRM);

    alarm(interval);   /* 입력받은 간격초 뒤 SIGALRM 예약 */
    printf("%u초 간격으로 %d번 알람이 울립니다.\n", interval, repeat);
    fflush(stdout);   /* 안내 메시지를 즉시 보이게 한다(버퍼에 남지 않도록) */

    int printed = 0;

    /*
     * 시그널이 오면 대기 중이던 sigsuspend 가 중단된다.
     * 그때 timeout 카운터로 알람이 울린 횟수를 확인한다.
     */
    while (printed < repeat) {
        while (printed < timeout && printed < repeat) {
            printed++;
            printf("알람 %d번 울림\n", printed);
        }

        if (printed < repeat)
            sigsuspend(&wait_mask);
    }

    alarm(0);   /* 반복 횟수를 채웠으니 예약을 취소한다 */

    /* 원래 시그널 마스크로 되돌린다 */
    sigprocmask(SIG_SETMASK, &old, NULL);

    printf("타이머가 종료되었습니다.\n");
    return 0;
}