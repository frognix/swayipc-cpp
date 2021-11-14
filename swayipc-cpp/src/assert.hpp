#ifndef NDEBUG
inline void abort_with_message(const char* name, int line, const char* file) {
    fprintf(stderr, "\n%s: %d:%s\n", name, line, file);
    std::abort();
}

inline void nop() {}

#define COND_CHECK(name, cond) \
(cond) ? nop() : abort_with_message(name, __LINE__, __FILE__)
#else
#define COND_CHECK(name, cond) ((void*)0)
#endif

#define Assert(cond) COND_CHECK("Assert", (cond))
#undef Expects
#define Expects(cond) COND_CHECK("Expects", (cond))
#undef Ensures
#define Ensures(cond) COND_CHECK("Ensures", (cond))
