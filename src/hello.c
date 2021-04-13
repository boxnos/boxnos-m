typedef struct _ESTOP {
    void *d;
    unsigned long long(*s)(struct _ESTOP *t, unsigned short *s);
} ESOP;

typedef struct {
    char d[52];
    void *h;
    ESOP *o;
} EST;

unsigned long long EfiMain (void* not_used, EST *tbl) {
    tbl->o->s(tbl->o, L"Hello, Boxnos World!!\n");
    for (;;);
    return 0;
}
