// vim: noai:ts=2:sw=2

/*
 * gsufsort
 *
 * Authors: Felipe A. Louza
 * contact: louza@usp.br
 * 18/09/2018
 *
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <assert.h>
#include <getopt.h>

#include "lib/utils.h"
#include "lib/file.h"
#include "external/gsacak.h"
#include "lib/rankbv.h"

#define SIGMA 256

#ifndef DEBUG
#define DEBUG 0
#endif

#define WORD (size_t)(pow(256,sizeof(int_t))/2.0)

/******************************************************************************/
void usage(char *name){
    printf("\n\tUsage: %s FILENAME [options]\n\n",name);
    puts("Extensions supported: txt (raw files) fasta and fastq");
    puts("Available options:");

    puts("\t--corr                choose Exact matching version ");
    puts("\t--raw                 choose Fault-tolerant matching version ");
    puts("\t--time                output time (seconds)");
    puts("\t--verbose             verbose output");
    puts("\t--help                this help message");
    exit(EXIT_FAILURE);
}

/******************************************************************************/

int build=1;
int light=0;

int main(int argc, char** argv){

    time_t t_total=0;clock_t c_total=0;
    time_t t_start=0;clock_t c_start=0;
    int_t i;
    int choose_opt=0;
    int lcp=1, da=1, time=0, upper=0;
    int in_type=0;// (txt, 1), (fasta, 2) and (fastq, 3)

    extern char *optarg;
    extern int optind;
    int c,topk=10,Fault_tolerance=100;
    char *c_input=NULL, *c_output=NULL;

    int d=0; //number of string
    int p=0; //print A[1,p]
    int output=0; //output
    int verbose=0, print=1;

/********/
    while (1) {
        //int this_option_optind = optind ? optind : 1;
        int option_index = 0;

        static struct option long_options[] = {//int tempFault_tolerance
                {"topk",      required_argument, 0, 'k'},
								{"tolerance",      required_argument, 0, 't'},
                {"verbose", no_argument,       0, 'v'},
                {"corr",    no_argument,       0, 'C'},
                {"raw",     no_argument,       0, 'R'},
                {"time",    no_argument,       0, 'T'},
                {0,         0,                 0,  0 }
        };

        c = getopt_long(argc, argv, "S:vtP:d:L:D:g:lG:B:bhtfqo:ic:Qum", long_options, &option_index);

        if (c == -1) break;
        //printf("%d %c\n",c,c);
        switch (c) {
            case 'C':
                choose_opt=1; break;
            case 'R':
                choose_opt=2;break;
            case 'v':
                verbose++; break;
            case 'T':
                time++; break;
            case 'h':
                usage(argv[0]); break;
						case 'k':
                topk=atoi(optarg); break;
						case 't':
                Fault_tolerance=atoi(optarg); break;
            case '?':
                exit(EXIT_FAILURE);
        }
	
    }
		//printf("topk:%d \n\n",topk);
/********/
    if(optind+1==argc) {
        c_input=argv[optind++];
    }
    else  usage(argv[0]);

    if(choose_opt==0){
        puts("please choose sldms version");
        exit(EXIT_FAILURE);
    }
    if(time){
        time_start(&t_total, &c_total);
    }
    lcp=1;
    if(!output){
        c_output = (char*) malloc(strlen(c_input)+5);
        sprintf(c_output, "%s", filename_without_path(c_input));
    }

    if(build){

        unsigned char **R;
        size_t n=0;
        printf("## sldms start! ##\n");
        //disk access
        R = (unsigned char**) file_load_multiple(c_input, &d, &n, in_type, 0);

        if(n>pow(2,30) && (sizeof(int_t)<8)){
            fprintf(stderr, "####\n");
            fprintf(stderr, "ERROR: INPUT LARGER THAN %.1lf GB (%.1lf GB)\n", WORD/pow(2,30), n/pow(2,30));
            fprintf(stderr, "PLEASE USE %s-64\n", argv[0]);
            fprintf(stderr, "####\n");
            exit(EXIT_FAILURE);
        }

        //concatenate all string
        unsigned char *T = cat_all(R, d, &n, verbose, upper);

        //free memory
        for(i=0; i<d; i++)
            free(R[i]);
        free(R);

        int_t *SA = NULL;
        SA = (int_t*) malloc(n*sizeof(int_t));
        assert(SA);
        int_t i;
        for(i=0; i<n; i++) SA[i]=0;

        int_t *ISA = NULL;

        int_t *LCP = NULL;
        if(lcp){
            LCP = (int_t*) malloc(n*sizeof(int_t));
            assert(LCP);
            for(i=0; i<n; i++) LCP[i]=0;
        }

        int_da *DA = NULL;
        if(da){
            if(light==0){
                DA = (int_da*) malloc(n*sizeof(int_da));
                assert(DA);
                for(i=0; i<n; i++) DA[i]=0;
            }
        }

        if(verbose){
            printf("########\n");
            printf("INPUT = %s\n", c_input);
            printf("OUTPUT = %s", c_output);
            printf("\n");
            printf("N = %zu\n", n);
            printf("d = %d\n", d);
            printf("sizeof(int) = %zu bytes\n", sizeof(int_t));
            if(da){
                printf("sizeof(int_da) = %zu bytes\n", sizeof(int_da));
            }
#if TERMINATOR
            printf("TERMINATOR\n");
#endif
            printf("########\n");
        }

        /********/

        printf("## GET SA LCP DA! ##\n");

        if(time){
            time_start(&t_start, &c_start);
        }

        gsacak(T, (uint_t*)SA, LCP, DA, n);

        if(time){
            double d_time = time_stop(t_start, c_start);
            if(verbose) fprintf(stderr,"%.6lf\n", d_time);
        }

        /********/

        if(time){
            time_start(&t_start, &c_start);
        }

        /********/
        if(print){
            printf("## print tempout.txt ##\n");
            FILE *fp=fopen("tempout.txt","w+");
            if(fp==NULL){
                printf("cannot open this tempout file\n");
                exit(0);
            }
            else{
                if(!p)p=n;
#if TERMINATOR
                if(choose_opt==1)print_array(T, DA, SA, ISA, LCP, n, min(n,p), 1,d,fp,topk);
								if(choose_opt==2)print_array_raw(T, DA, SA, ISA, LCP,n, min(n,p), 1,d,fp,topk,Fault_tolerance);
#else
                if(choose_opt==1)print_array(T, DA+1, SA+1, ISA+1, LCP+1,n-1, min(n-1,p), 0,d,fp,topk);
                if(choose_opt==2)print_array_raw(T, DA+1, SA+1, ISA+1, LCP+1,n-1, min(n-1,p), 0,d,fp,topk,Fault_tolerance);
#endif
            } 
            fclose(fp);
        }

        if(time){
            double d_time = time_stop(t_start, c_start);
            if(verbose) fprintf(stderr,"%.6lf\n", d_time);
        }

        free(T);
        free(SA);
        free(LCP);
        free(DA);
    }
    if(time){
        printf("## total ##\n");
        double d_time = time_stop(t_total, c_total);
        if(verbose) fprintf(stderr,"%.6lf\n", d_time);
    }

    if(!output)
        free(c_output);

    return 0;
}
/******************************************************************************/
