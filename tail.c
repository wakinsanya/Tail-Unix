/*
 * William Akinsanya
 *
 * This is an emulation of the UNIX utility 'tail'.
 * As such, the last parts of a file/stdin are outputed by this program.
 * By default the last 10 lines (if applicable) are returned.
 * Options may be used to modify this behavior, use -h for more information.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_help() {
    printf("%s\n",
           "Usage: tail [options]:\n-n Output the last K lines.\n-o Print odd lines.\n-V Output version info.\n-h Print this help message.");
}

void print_version() {
    printf("%s\n",
           "Name: William Akinsanya\nEmail: akinsanya.will@gmail.com");
}

int main(int argc, char *argv[]) {
    int nflag = 0;
    int oflag = 0;
    char *nvalue = NULL;
    int opt;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char print_lines[20][1000];
    int curr_position = 0;
    opterr = 0;
   
    // Evaluate command line arguments.
    while ((opt = getopt(argc, argv, "n:Vho")) != -1) {
        switch (opt) {
            case 'n':
                nflag = 1;
                nvalue = optarg;
                break;
            case 'V':
                print_version();
                exit(EXIT_SUCCESS);
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
                break;
            case 'o':
                oflag = 1;
                break;
            case '?':
                if (optopt == 'n')
                    fprintf(stderr, "The -%c option musted be paired with a numerical argument.\n",
                            optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Invalid option -%c.\n", optopt);
            default:
                print_help();
                exit(EXIT_FAILURE);
        }
    }
    
    // File pointer check, if no file read from standard input.
    if (argv[optind] != NULL) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            printf("Erorr: could not read file.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        int limit = 10;
        if(nflag) limit = atoi(nvalue);
        if(oflag) limit += limit;
        char custom_print_lines[limit][1000];
        while ((read = getline(&line, &len, stdin)) != -1) {
            strcpy(custom_print_lines[(curr_position) % limit], line);
            curr_position++;
        }
        if (oflag) {
            for (int i = 0; i < limit; i++) {
                if((curr_position + i) % 2 == 0)
                    printf("%s", custom_print_lines[(curr_position + i) % limit]);
            }
        } else {
            for (int i = 0; i < limit; i++) {
                printf("%s", custom_print_lines[(curr_position + i) % limit]);
            }
        }
        exit(EXIT_SUCCESS);
    }
    
    // Non-option arg - file name, default case.
    if (!nflag) {
        while ((read = getline(&line, &len, fp)) != -1) {
            strcpy(print_lines[(curr_position) % 20], line);
            curr_position++;
        }
        int start = 10;
        if (oflag) {
            start = 0;
            for (;start < 20; start++) {
                if((curr_position + start) % 2 == 0)
                    printf("%s", print_lines[(curr_position + start) % 20]);
            }
        } else {
            for (;start < 20; start++) {
                printf("%s", print_lines[(curr_position + start) % 20]);
            }
        }
        
    // Return the last K lines of the file.
    } else if (nflag) {
        int n = atoi(nvalue);
        if(n == 0 ) exit(EXIT_SUCCESS);
        if(n < 1) {
            printf("%s\n", "-n must be paired with a natural number i.e 1, 2, 3.");
            exit(EXIT_FAILURE);
        }
        
        if (oflag) n += n;
        char custom_print_lines[n][1000];
        while ((read = getline(&line, &len, fp)) != -1) {
            strcpy(custom_print_lines[(curr_position) % n], line);
            curr_position++;
        }
        if (oflag) {
            for (int i = 0; i < n; i++) {
                if((curr_position + i) % 2 == 0)
                    printf("%s", custom_print_lines[(curr_position + i) % n]);
            }
        } else {
            for (int i = 0; i < n; i++) {
                printf("%s", custom_print_lines[(curr_position + i) % n]);
            }
        }
    } else {
        printf("Error: illegal combination of arguments.\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    return 0;
}
