#include <errno.h>
#include <sys/types.h>
#include <stddef.h>

caddr_t sbrk(int incr __attribute__((unused))) {
    errno = ENOMEM;
    return (caddr_t) - 1;
}
