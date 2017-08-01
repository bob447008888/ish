#include <poll.h>
#include "sys/fs.h"
#include "sys/calls.h"

struct pollfd_ {
    fd_t fd;
    word_t events;
    word_t revents;
};

dword_t sys_poll(addr_t fds, dword_t nfds, dword_t timeout) {
    if (nfds != 1)
        TODO("actual working poll");

    struct pollfd_ fake_poll;
    user_get_count(fds, &fake_poll, sizeof(fake_poll));
    struct pollfd real_poll;
    real_poll.fd = current->files[fake_poll.fd]->real_fd;
    real_poll.events = fake_poll.events;
    real_poll.revents = fake_poll.revents;
    int res = poll(&real_poll, 1, timeout);
    fake_poll.revents = real_poll.revents;
    user_put_count(fds, &fake_poll, sizeof(fake_poll));
    return res;
}