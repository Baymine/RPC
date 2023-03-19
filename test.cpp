#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace learningSem{

    #define N 10

    sem_t sem1, sem2;

    void* thread1(void* arg)
    {
        int i;
        for (i = 0; i < N; i++) {
            sem_wait(&sem1);  // 等待信号量 sem1
            printf("Thread1: %d\n", i);
            sem_post(&sem2);  // 发送信号量 sem2
        }
        pthread_exit(NULL);
    }

    void* thread2(void* arg)
    {
        int i;
        for (i = 0; i < N; i++) {
            sem_wait(&sem2);  // 等待信号量 sem2
            printf("Thread2: %d\n", i);
            sem_post(&sem1);  // 发送信号量 sem1
        }
        pthread_exit(NULL);
    }

    int main(int argc, char* argv[])
    {
        pthread_t tid1, tid2;

        sem_init(&sem1, 0, 0);  // 初始化信号量 sem1，初始值为 0
        sem_init(&sem2, 0, 1);  // 初始化信号量 sem2，初始值为 1

        pthread_create(&tid1, NULL, thread1, NULL);
        pthread_create(&tid2, NULL, thread2, NULL);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);

        sem_destroy(&sem1);
        sem_destroy(&sem2);

        return 0;
    }
}

namespace learningTimefd{

    int main() {
        int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
        if (timer_fd == -1) {
            std::cerr << "Failed to create timerfd: " << strerror(errno) << std::endl;
            return 1;
        }

        struct itimerspec timer_spec;
        memset(&timer_spec, 0, sizeof(timer_spec));
        timer_spec.it_value.tv_sec = 1; // 定时器首次触发时间：1秒
        timer_spec.it_interval.tv_sec = 2; // 定时器重复触发时间间隔：2秒

        if (timerfd_settime(timer_fd, 0, &timer_spec, nullptr) == -1) {
            std::cerr << "Failed to set timerfd: " << strerror(errno) << std::endl;
            close(timer_fd);
            return 1;
        }

        int epoll_fd = epoll_create1(0);
        if (epoll_fd == -1) {
            std::cerr << "Failed to create epoll: " << strerror(errno) << std::endl;
            close(timer_fd);
            return 1;
        }

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = timer_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &event) == -1) {
            std::cerr << "Failed to add timerfd to epoll: " << strerror(errno) << std::endl;
            close(epoll_fd);
            close(timer_fd);
            return 1;
        }

        while (true) {
            struct epoll_event events[10];
            int num_events = epoll_wait(epoll_fd, events, 10, -1);
            if (num_events == -1) {
                std::cerr << "epoll_wait failed: " << strerror(errno) << std::endl;
                break;
            }

            for (int i = 0; i < num_events; ++i) {
                if (events[i].data.fd == timer_fd) {
                    uint64_t expirations;
                    ssize_t n = read(timer_fd, &expirations, sizeof(expirations));
                    if (n != sizeof(expirations)) {
                        std::cerr << "Failed to read timerfd: " << strerror(errno) << std::endl;
                        break;
                    }
                    std::cout << "Timer expired " << expirations << " times." << std::endl;
                }
            }
        }

        close(epoll_fd);
        close(timer_fd);
        return 0;
    }

}

int main(){
    learningTimefd::main();
}
