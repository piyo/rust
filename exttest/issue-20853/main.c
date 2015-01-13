/*
 DESC This test program changes its own rlimits::RLIMIT_STACK to be
 page-aligned or not depending on option, then execve() a
 /usr/local/bin/rustc --verbose --version (installed via nightly).

 COMPILE gcc -Wall main.c
 RUN ./a.out e ; ./a.out n ; ./a.out

 EXPECT './a.out e' reports thread '<unnamed>' panicked at 'failed to
 allocate a guard page', when rust.git 44a287e63, Ubuntu 12.04.5

 EXPECT './a.out n' runs with no errors.

 EXPECT './a.out' will inherit whatever RLIMIT_STACK was set (like
 inside an erroneous emacs).

 DATE [2015-01-10 Sat 15:51]
 */

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

void create_new_env( char* additions[], char*** final_env );

int main(int argc, char *argv[])
{
    struct rlimit old = {0};
    struct rlimit new = {0};

    int j = 0;
    for (j = 0; j < argc; ++j)
      printf("argv[%d]: %s\n", j, argv[j]);

    getrlimit(RLIMIT_STACK, &old);
    printf("curr: %lx %lx\n", old.rlim_cur, old.rlim_max);

    /* option: force normal case */
    if (argc > 1 && argv[1][0] == 'n') {
      old.rlim_cur = 8192 * 1024;
      setrlimit(RLIMIT_STACK, &old);
    }

    /* option: force abnormal case */
    if (argc > 1 && argv[1][0] == 'e') {
      old.rlim_cur = 8720000; /* Ubuntu 12.04.5, Emacs 12.04.05 sample */
      setrlimit(RLIMIT_STACK, &old);
    }

    getrlimit(RLIMIT_STACK, &new);
    printf("2nd:  %lx %lx\n", new.rlim_cur, new.rlim_max);

    printf("Is RLIMIT_STACK 4KB page-aligned? %s\n",
           (new.rlim_cur % 4096 == 0) ? "Yes": "NO!");

    const char* const RUSTC_BIN_DEFAULT = "/usr/local/bin/rustc";
    const char* exe = (char* const) RUSTC_BIN_DEFAULT;
    char *new_argv[] = { (char* const)exe, "--version", "--verbose", NULL };
    char *new_env[] = { "RUST_BACKTRACE=1", NULL };
    char** final_env = NULL;
    create_new_env( new_env, &final_env );
    execve(exe, new_argv, final_env);
    if (final_env != NULL) {
      free(final_env);
    }
    exit(EXIT_FAILURE);
}


void create_new_env( char* addl_env[], char*** final_env )
{
    extern char **environ; /* see environ(7) */
    size_t i,j;

    for (i=0; environ[i] != NULL; ++i) {
      /* do nothing additional */
    }
    for (j=0; addl_env[j] != NULL; ++j) {
      /* do nothing additional */
    }
    const size_t kFINAL_SIZE = i+j;
    if (final_env != NULL) {
      (*final_env) = malloc(sizeof(char*) * (kFINAL_SIZE+1));
      (*final_env)[kFINAL_SIZE] = NULL;
      size_t k;
      for (k = 0; k < kFINAL_SIZE+1; ++k) {
        size_t srci = k;
        char** src_env = environ;
        if (k >= i) {
          srci = k - i;
          src_env = addl_env;
        }
        (*final_env)[k] = src_env[srci];
      }
    }
}
