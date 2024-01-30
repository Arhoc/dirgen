#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        puts("dirgen - C Project Generator");
        puts("\t- ./dirgen [DIRECTORY] {--clang | --tcc | --gcc (default)} { pkg-config-libraries }");
        puts("\t- ./dirgen . --clang gtk+-3.0 libsoup-2.4");
        puts("\t- ./dirgen suC-baka --tcc");

        return 0;
    }

    if(strcmp(argv[1], ".") != 0) {
        mkdir(argv[1], 504);
        chdir(argv[1]);
    }

    mkdir("include", 504);
    mkdir("examples", 504);
    mkdir("docs", 504);
    mkdir("src", 504);

    FILE* f;

    f = fopen("README.MD", "w");
    fprintf(f, "# %s\nProject Description ...", argv[1]);
    fclose(f);

    f = fopen("Makefile", "w");
    fprintf(f, "CC = %s\n", 
        (argc > 2 && strcmp(argv[2], "--clang") == 0) ? "clang" : 
        (argc > 2 && strcmp(argv[2], "--tcc") == 0) ? "tcc" :
        "gcc"
    );

    if(argc > 2)
        fprintf(f, "LIBS = %s\n", 
            (argc > 2 && (strcmp(argv[2], "--clang") != 0) && (strcmp(argv[2], "--tcc") != 0)) ? argv[2] : 
            (argc > 1 && (strcmp(argv[1], "--cflags")) && (strcmp(argv[2], "--tcc") != 0)) ? argv[1] : 
            ""
        );

    fputs("INCLUDE_DIR = include\n", f);
    fputs("BUILD_DIR = build\n", f);
    fputs("SRC_DIR = src\n", f);
    fputs("SRCS = $(wildcard $(SRC_DIR)/*.c)\n", f);
    fputc('\n', f);
    fputs("all:\n", f);
    fputs("\tmkdir -p $(BUILD_DIR)\n", f);

    /* %s-o %s -o. ensures that the arguments are properly concatenated */
    fprintf(f, "\t$(CC) $(SRCS) -Wall -Wextra -I$(INCLUDE_DIR) %s -o $(BUILD_DIR)/%s", 
        (argc > 2 && strcmp(argv[2], "--clang") != 0) ? "`pkg-config --libs --cflags $(LIBS)` " : 
        (argc > 2) ? argv[3] : 
        "", 
        argv[1]
    );
    fclose(f);

    return 0;
}
