# sys
시스템프로그래밍

6주차 과제 결과 및 ai 사용 흐름

1_sigint.c

<img width="525" height="208" alt="image" src="https://github.com/user-attachments/assets/9de6876b-c7ec-4916-9c93-08e5aa257e16" />

2_alarm.c 

1. 2_alarm.c 변경요청(이유포함) -> (ai) 변경했으나 기존 pause() 방식은 조건 검사와 시그널 대기 사이에 시그널이 도착하면 이를 놓쳐 프로그램이 무한히 대기하는 경쟁 조건이 발생 할 수 있으므로 sigprocmask() 사용해야함
2. sigprocmask()사용한 2_alarm.c변경 요청 및 흐름 설명 요청

<img width="388" height="222" alt="image" src="https://github.com/user-attachments/assets/1934d0e1-6de4-46f5-9455-d298b771e542" />

3_signal_block.c

1. 3_signal_block.c 변경 요청 -> (ai) 기존 코드에서도 시그널 차단 및 해제 기능은 구현되어 있으나, SIGINT가 실제로 대기 중인지 정확하게 확인하기 위해 sigpending()과 sigismember()를 추가할 것을 제안함
2. sigpending()과 sigismember()를 사용한 3_signal_block.c 변경 요청 및 시그널 차단, 대기, 해제 과정에 대한 흐름 설명 요청

<img width="940" height="254" alt="image" src="https://github.com/user-attachments/assets/c5a0228b-a826-4448-ac18-a557fb09af53" />
