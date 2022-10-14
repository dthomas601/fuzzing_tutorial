 /* test-mem_libfuzzer.cpp
 */

#include "../config.h"
#include <libexif/exif-data.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-loader.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

static int test_exif_data(ExifData *d)
{
    unsigned int i, c;
    char v[1024], *p;
    ExifMnoteData *md;

    fprintf (stdout, "Byte order: %s\n",
        exif_byte_order_get_name (exif_data_get_byte_order (d)));

    fprintf (stdout, "Parsing maker note...\n");
    md = exif_data_get_mnote_data (d);
    if (!md) {
        fprintf (stderr, "Could not parse maker note!\n");
        exif_data_unref (d);
        return 1;
    }

    fprintf (stdout, "Increasing ref-count...\n");
    exif_mnote_data_ref (md);

    fprintf (stdout, "Decreasing ref-count...\n");
    exif_mnote_data_unref (md);

    fprintf (stdout, "Counting entries...\n");
    c = exif_mnote_data_count (md);
    fprintf (stdout, "Found %i entries.\n", c);
    for (i = 0; i < c; i++) {
        fprintf (stdout, "Dumping entry number %i...\n", i);
        fprintf (stdout, "  Name: '%s'\n",
                exif_mnote_data_get_name (md, i));
        fprintf (stdout, "  Title: '%s'\n",
                exif_mnote_data_get_title (md, i));
        fprintf (stdout, "  Description: '%s'\n",
                exif_mnote_data_get_description (md, i));
        p = exif_mnote_data_get_value (md, i, v, sizeof (v));
        if (p) { fprintf (stdout, "  Value: '%s'\n", v); }
    }

    return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {   
	
	ExifData *d;
    unsigned int buf_size;
    unsigned char *buf;
    int r;

    d = exif_data_new_from_data(data, size);

    r = test_exif_data(d);
    if (r) return r;

    fprintf (stdout, "Test successful!\n");

    return 1; 
}
