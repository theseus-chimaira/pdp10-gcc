typedef unsigned long word_t;

extern int strcmp(const char *, const char *);
extern int assemble_file(const char *, const char *, int);

static int
arg_text(const word_t *arg, char *text, unsigned int cap)
{
    unsigned int n;
    unsigned int i;
    unsigned int slot;
    unsigned int pos;
    unsigned int ch;
    word_t word;

    if (arg == 0 || text == 0 || cap == 0U)
        return 1;
    n = (unsigned int)(arg[0] & 0777777U);
    if (n + 1U > cap)
        return 1;
    for (i = 0U; i < n; i++) {
        slot = 1U + i / 6U;
        pos = i % 6U;
        word = arg[slot];
        ch = (unsigned int)((word >> (30U - pos * 6U)) & 077U);
        ch += 040U;
        if (ch >= 'A' && ch <= 'Z')
            ch = ch - 'A' + 'a';
        text[i] = (char)ch;
    }
    text[n] = 0;
    return 0;
}

int
pdp10_regrename_main(int argc, word_t **argv)
{
    char out[129];
    char in[129];
    char argtext[129];
    int have_out;
    int have_in;
    int i;

    have_out = 0;
    have_in = 0;
    if (argc < 0 || argc > 64 || argv == 0)
        return 1;
    for (i = 1; i < argc; i++) {
        unsigned int j;

        if (arg_text(argv[i], argtext, sizeof(argtext)) != 0)
            return 1;
        if (strcmp(argtext, "-o") == 0) {
            if (++i >= argc || arg_text(argv[i], out, sizeof(out)) != 0)
                return 1;
            have_out = 1;
        } else {
            if (argtext[0] == '-')
                return 1;
            for (j = 0U; j < sizeof(in) - 1U && argtext[j] != 0; j++)
                in[j] = argtext[j];
            in[j] = 0;
            have_in = 1;
        }
    }
    if (!have_out || !have_in)
        return 1;
    return assemble_file(in, out, 1);
}
