#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

int end_file_flag = 0;
int match_found = 0;
int file_matched = 0;

char *readLine(FILE *fp) {
    size_t buffer_size = 2;
    char *buffer = malloc(buffer_size);
    size_t len = 0;

    if (buffer == NULL) {
        perror("malloc");
        exit(2);
    }
    
    while(1) {
        // if end of file
        if (fgets(buffer + len, buffer_size - len, fp) == NULL) {
            if (len == 0) {
                end_file_flag = 1;
                free(buffer);
                return NULL;
            }
            break;
        }

        len += strlen(buffer + len);

        // if end of line
        if (buffer[len - 1] == '\n') {
            break;
        }
        // double size of buffer
        else {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
            if (buffer == NULL) {
                perror("realloc");
                exit(2);
            }
        }
    }
    return buffer;
}

void fgrep(FILE *fp, char *pattern, int ignore_flag, int name_flag, int line_flag, int quiet_flag, char *file_name, int multiple_files) {
    int line_number = 1;
    end_file_flag = 0;

    while (end_file_flag == 0) {
        match_found = 0;
        char *line = readLine(fp);
        // avoid segfault on NULL line
        if (line == NULL) {
            break;
        }
        // check if pattern is in line
        for (int i = 0; i <= (strlen(line)-strlen(pattern)); i++) {
            // if ignore flag is set, ignore case
            if (ignore_flag == 1) {
                if (strncasecmp(&line[i], pattern, strlen(pattern)) == 0) {
                    match_found = 1;
                    file_matched = 1;
                    if (quiet_flag) {
                        free(line);
                        return;
                    }
                    if (line_flag) {
                        printf("%s\n", file_name);
                        free(line);
                        return;
                    }
                }
            }
            else if (strncmp(&line[i], pattern, strlen(pattern)) == 0) {
                match_found = 1;
                file_matched = 1;
                if (quiet_flag) {
                    free(line);
                    return;
                }
                if (line_flag) {
                    printf("%s\n", file_name);
                    free(line);
                    return;
                }
            }
        }
        if (name_flag == 1) {
            // if match found
            if (line_flag == 1) {
                if (match_found != 0) {
                    if (multiple_files) {
                        printf("%s:", file_name);
                    }
                    printf("%d", line_number);
                    printf("%s", ":");
                    printf("%s", file_name);
                }
            }
            else {
                if (match_found != 0) {
                    if (multiple_files) {
                        printf("%s:", file_name);
                    }
                    printf("%d", line_number);
                    printf("%s", ":");
                    printf("%s", line);
                }
            }
        }

        else {
            // if match found
            if (line_flag == 1) {
                if (match_found != 0) {
                    if (multiple_files) {
                        printf("%s:", file_name);
                    }
                    printf("%s", file_name);
                }
            }
            else {
                if (match_found != 0) {
                    if (multiple_files) {
                        printf("%s:", file_name);
                    }
                    printf("%s", line);
                }
            }
        }
        line_number++;
    }
}

int main(int argc, char *argv[]) {
    int ignore_flag = 0;
    int name_flag = 0;
    int line_flag = 0;
    int quiet_flag = 0;
    int i = 1;
    int multiple_files = 0;
    char *pattern = NULL;
    int error = 0;
    FILE *fp = NULL;

    while (argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            ignore_flag = 1;
        } 
        else if (strcmp(argv[i], "-n") == 0) {
            name_flag = 1;
        } 
        else if (strcmp(argv[i], "-l") == 0) {
            line_flag = 1;
        } 
        else if (strcmp(argv[i], "-q") == 0) {
            quiet_flag = 1;
        }
        else {
            fprintf(stderr, "Usage: fgrep [-i] [-l] [-n] [-q] pattern [files]\n");
            exit(2);
        }
        i++;
    }

    pattern = argv[i];
    i++;

    if (argv[i] == NULL) {
        fgrep(stdin, pattern, ignore_flag, name_flag, line_flag, quiet_flag, "(standard input)", multiple_files);
    }

    else {
        if (argc - i > 1) {
            multiple_files = 1;
        }

        while (argv[i] != NULL) {
            fp = fopen(argv[i], "r");

            if (fp == NULL) {
                error = 1;
                if (quiet_flag == 0) {
                    fprintf(stderr, "Could not open %s: %s\n", argv[i], strerror(errno));
                }
            }
            else {
                fgrep(fp, pattern, ignore_flag, name_flag, line_flag, quiet_flag, argv[i], multiple_files);
                fclose(fp);
            }
            i++;
        }

        if (quiet_flag == 1) {
            // -q ignores all output, but still needs correct status
            if (file_matched == 1) {
                exit(0);
            } else {
                exit(1);
            }
        }
        else if (error == 1) {
            exit(1);
        }
        if (file_matched == 1) {
            exit(0);
        } 
        else {
            exit(1);
        }
    }
}