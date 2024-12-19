# exam_06

## Notes about Rossi

### Why do we FD_ZERO only current?

Since we copy at every loop current to read_set and write_set there is no need to FD_ZERO also read_set and write_set.

### What is fd_set?

From `man select`

> fd_set: A structure type that can represent a set of file descriptors. According to POSIX, the maximum number of file descriptors in an fd_set structure is the value of the macro FD_SET-SIZE.

The implementation is system-dependent.

`fd_set` can have a maximum size that we can found out running:

```c
#include <stdio.h>
#include <sys/select.h>

int main() {
    printf("FD_SETSIZE = %d\n", FD_SETSIZE);
    return 0;
}
```

or

```c
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    // Get soft limit
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("Soft limit: %lu\n", rlim.rlim_cur);
        printf("Hard limit: %lu\n", rlim.rlim_max);
    }

    // Alternative using sysconf
    long max_fd = sysconf(_SC_OPEN_MAX);
    printf("sysconf(_SC_OPEN_MAX) = %ld\n", max_fd);

    return 0;
}
```

### How does the assignment `read_set = write_set = current;` works?

In C, the assignment operator `=` works from right to left and returns the assigned value. In an expression `a = b = c;`, `b = c` gets evaluated first, it retures the value of the assignment which is then assigned to a.

`fd_set` is an array of bits. At the end we will have three different arrays with the same values.

### addr.sin_addr.s_addr = htonl(INADDR_ANY);

The subject states that our server that we should listen only to localhost and not to all IPs, so this line is possingbly wrong, so we should have

`serveraddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);`

or

`serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");`

### `for (int fd = 0; fd <= max_fd; fd++)`

In the for loop right after entering the while loop we are looping through all fds from 0 to maxfd. The file descriptor 0, 1 and 2 are reserved for `stdin`, `stdout` and `stderr` so we could skip them. The fd are assigned automatically and incrementally by the system and maxfd will always have the biggest id even if the client with the fd has being closed. While we check all FDs, the `FD_ISSET` check ensures we only process active ones. The alternative would be to keep an array of active fds and loop only over them. The performance gain would be neglectable and the code would get more complex, so we can just keep it the way it is.

### Why do we have `max_fd + 1`?

From `man select`:

> `int select(int nfds, fd_set *_Nullable restrict readfds, fd_set *_Nullable restrict writefds, fd_set *_Nullable restrict exceptfds, struct timeval *_Nullable restrict timeout);`

> `nfds This argument should be set to the highest-numbered file descriptor in any of the three sets, plus 1. The indicated file descriptors in each set are checked, up to this limit (but see BUGS).`

FDs are 0 indexed, when we create the serverfd, it will normally get assigned the fd 3. The total numbers of fd will be at this point 4. So we are just passing the total number of fds. But actually this is not the problem. We are asked specifically to pass the number of the highest FD number + 1. This is cause the fd_set is a arrys of bits and select need to check for 'events' till to the index of the highest fd. The actual number of active FD in this case doesn't matter.

### Is `if (select(maxfd + 1, &read_set, &write_set, 0, 0) == -1) continue;` necessary?

Without the if [...] == -1 check we would just go through the for loop again, but anything 'bad' would happen.

## TODOs

- [ ] Before closing a socket we should and erasing clients[fd].msg we should sent the possible 'new lines messages' stored there.
- [ ] The sockets are fundamentally blocking so we should integrate MSG_DONTWAIT
