# sys
시스템프로그래밍

6주차 ai 질문사항

2_alarm.c 

1. 2_alarm.c 변경요청 -> (ai) 변경했으나 기존 pause() 방식은 조건 검사와 시그널 대기 사이에 시그널이 도착하면 이를 놓쳐 프로그램이 무한히 대기하는 경쟁 조건이 발생 할 수 있으므로 sigprocmask() 사용해야함
2. sigprocmask()사용한 2_alarm.c변경 요청 및 흐름 설명 요청

3_signal_block.c

1. 
