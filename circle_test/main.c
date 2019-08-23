#include <stdio.h>
#include <sys/types.h>
#include "Circle.h"

int encode(int ac, char **av);
int decode(int ac, char **av);

int main(int argc, char* argv[]) {
    return encode(argc, argv);
    //return decode(argc, argv);
}

/* Write the encoded output into some FILE stream. */
static int write_out(const void *buffer, size_t size, void *app_key) {
    FILE *out_fp = app_key;
    size_t wrote = fwrite(buffer, 1, size, out_fp);
    return (wrote == size) ? 0 : -1;
}
int encode(int ac, char **av) {
    Circle_t *rectangle; /* Type to encode */
    asn_enc_rval_t ec; /* Encoder return value */
    /* Allocate the Rectangle_t */
    rectangle = calloc(1, sizeof(Circle_t)); /* not malloc! */
    if(!rectangle) {
        perror("calloc() failed");
        exit(1);
    }
    /* Initialize the Rectangle members */
    rectangle->position_x = 42; /* any random value */
    rectangle->position_y = 23; /* any random value */
    rectangle->radius = 100;
    /* BER encode the data if filename is given */
    if(ac < 2) {
        fprintf(stderr, "Specify filename for BER output\n");
    } else {
        const char *filename = av[1];
        FILE *fp = fopen(filename, "wb"); /* for BER output */
        if(!fp) {
            perror(filename);
            exit(1);
        }
        /* Encode the Rectangle type as BER (DER) */
        ec = der_encode(&asn_DEF_Circle, rectangle, write_out, fp);
        fclose(fp);
        if(ec.encoded == -1) {
            fprintf(stderr, "Could not encode Rectangle (at %s)\n",
            ec.failed_type ? ec.failed_type->name : "unknown");
            exit(1);
        } else {
            fprintf(stderr, "Created %s with BER encoded Rectangle\n", filename);
        }
    }
    /* Also print the constructed Rectangle XER encoded (XML) */
    xer_fprint(stdout, &asn_DEF_Circle, rectangle);
    return 0; /* Encoding finished successfully */
}


//**************************************************************************************************
int decode(int argc, char* argv[]) {
    char buf[1024]; /* Temporary buffer */
    asn_dec_rval_t rval; /* Decoder return value */
    Circle_t *rectangle = 0; /* Type to decode. Note this 01! */
    FILE *fp; /* Input file handler */
    size_t size; /* Number of bytes read */
    char *filename; /* Input file name */

    /* Require a single filename argument */
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file.ber>\n", argv[0]);
        exit(1);
    } else {
        filename = argv[1];
    }

    /* Open input file as read-only binary */
    fp = fopen(filename, "rb");
    if(!fp) {
        perror(filename);
        exit(1);
    }

    /* Read up to the buffer size */
    size = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);
    if(!size) {
        fprintf(stderr, "%s: Empty or broken\n", filename);
        exit(1);
    }
    /* Decode the input buffer as Rectangle type */
    rval = ber_decode(0, &asn_DEF_Circle, (void **)&rectangle, buf, size);
    if(rval.code != RC_OK) {
        fprintf(stderr, "%s: Broken Rectangle encoding at byte %ld\n", filename, (long)rval.consumed);
        exit(1);
    }
    /* Print the decoded Rectangle type as XML */
    xer_fprint(stdout, &asn_DEF_Circle, rectangle);
    return 0; /* Decoding finished successfully */
}