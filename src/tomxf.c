#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filelist.h"
#include "mxf.h"

//TODO: use glog

void usage(char *name) {
    FILE *fd = stdout;

    fprintf(fd, "Usage:\n");
    fprintf(fd, "       %s -i <file> -o <file> [options ...]\n\n", name);
    fprintf(fd, "Required:\n");
    fprintf(fd, "       -i | --input <file>            - input file or directory\n");
    fprintf(fd, "       -o | --output <file>           - output file or directory\n");
    fprintf(fd, "       -r | --frame_rate <24|0>       - frame_rate\n             Choose between 24 for 24.000FPS or 0 for 23.976FPS (default)\n");
    fprintf(fd, "\n");
    fprintf(fd, "Options:\n");
    fprintf(fd, "       -h | --help                    - show help\n");
    fprintf(fd, "\n\n");

    fclose(fd);
    exit(1);
}

int main (int argc, char **argv) {
    char *in_path  = NULL;
    char *out_path = NULL;
    int frame_rate;
    //filelist_t *filelist;

    if (argc <= 1) {
        usage(argv[0]);
    }

    //parse options
    while (1)
    {
        static struct option long_options[] =
        {
            {"help",           required_argument, 0, 'h'},
            {"input",          required_argument, 0, 'i'},
            {"output",         required_argument, 0, 'o'},
            {"frame_rate",     required_argument, 0, 'r'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        int c = getopt_long (argc, argv, "i:o:r:h",
                         long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch (c)
        {
            case 0:
                if (long_options[option_index].flag != 0) {
                    break;
                }
                break;

            case 'i':
                in_path = optarg;
                break;

            case 'o':
                out_path = optarg;
                break;

            case 'r':
                frame_rate = strtol(optarg, NULL, 10);
                if (frame_rate != 0 &&  frame_rate != 24) {
                    fprintf(stderr, "Unsupported frame rate, set frame_rate to default 23.976(24000/1001)\n");
                    frame_rate = 0;
                }
                break;

            case 'h':
                usage(argv[0]);
                break;

        }
    }

    //check input path check
    if (in_path == NULL) {
        fprintf(stderr, "Missing input file path\n");
        return 0;
    }

    if (in_path[strlen(in_path) - 1] == '/') {
        in_path[strlen(in_path) - 1] = '\0';
    }

    //check output path
    if (out_path == NULL) {
        fprintf(stderr, "Missing output file path\n");
        return 0;
    }

    if (out_path[strlen(out_path) - 1] == '/') {
        out_path[strlen(out_path) - 1] = '\0';
    }

    //check single or dir in the path
    if (!is_dir(in_path)) {
        fprintf(stderr, "Input should be a directory but %s found\n", in_path);
    }

    if (is_dir(out_path)) {
        fprintf(stderr, "Output must be a sigle file .mxf but %s found\n", out_path);
    }  
    
#if 0
    //get file list
    fprintf(stdout, "Searching path %s\n", in_path);
    filelist = get_filelist(in_path, "j2k");

    if (filelist == NULL || filelist->nfiles < 1) {
        fprintf(stdout, "No input files located\n");
    }

    sort_filelist(filelist);
#endif

    //TODO: callbacks for the mxf writer */
    char str[7];
    if (frame_rate == 24) {
        strcpy(str,"24.000");
    }
    else {
        strcpy(str,"23.976");
    }
    printf("Pack images from %s to %s with frame_rate %s\n", in_path, out_path, str);
    if (write_mxf(in_path, out_path, frame_rate) != 0 )  {
        fprintf(stderr, "Could not create MXF file\n");
    }
    else {
        fprintf(stdout, "MXF creation complete\n");
    }

#if 0
    free_filelist(filelist);
#endif

    return 0;
}
