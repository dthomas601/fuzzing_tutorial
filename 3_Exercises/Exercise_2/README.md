# Exercise 2: libexif (libFuzzer)

For this exercise, we will take the libexif package used in Exercise 1 and use libFuzzer to identify vulnerabilities.

Again we are using libexif 0.6.14 and determine if we can replicate CVEs that we explored previously. (CVE-2012-2836)

Below are instructions to build the project.

Create a directory to save the project:

```
cd $HOME
mkdir fuzzing_libexif_libfuzzer && cd fuzzing_libexif_libfuzzer/
```

Download and uncompress libexif 0.6.14:

```
wget https://github.com/libexif/libexif/archive/refs/tags/libexif-0_6_14-release.tar.gz
tar -xzvf libexif-0_6_14-release.tar.gz	
```

Build and install libexif:

```
cd libexif-libexif-0_6_14-release/
./autogen.sh
autoreconf -i
CC=clang ./configure --enable-shared=no --prefix="$HOME/fuzzing_libexif_libfuzzer/install/"
make
make install
```

##### Downloading File Corpus


We will now download pictures that will be used to test libexif tool for any potential vulnerabilities. Below we download an existing archive of photos.

```
cd $HOME/fuzzing_libexif_libfuzzer
wget https://github.com/ianare/exif-samples/archive/refs/heads/master.zip
unzip master.zip
ls
```

##### Copy Files to Corpus Directory

Take the JPG files and copy them to the test corpus directory.

```
mkdir libexif-libexif-0_6_14-release/test/my_corpus
cp -r $HOME/fuzzing_libexif_libfuzzer/exif-samples-master/jpg/* $HOME/fuzzing_libexif_libfuzzer/libexif-libexif-0_6_14-release/test/my_corpus
```

#####  Test Program 

This program is based off test cases from exif the library. We modified ```$HOME/fuzzing_libexif_libfuzzer/libexif-libexif-0_6_14-release/test/test-mnote.c``` for our purposes. Our full modifications are shown below in :

```
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
```

##### Copy test program

Before compiling we move the test file inside of the libexif test directory:

```
cp test-mem_libfuzzer.cpp $HOME/fuzzing_libexif_libfuzzer/libexif-libexif-0_6_14-release/test/
```

##### Compile libFuzzer Executable

```
clang -g -O1 -fsanitize=fuzzer,address test-mem_libfuzzer.cpp `pkg-config --cflags --libs $HOME/fuzzing_libexif_libfuzzer/install/lib/pkgconfig/libexif.pc` -o libexif.run
```

* -g - Provides source-level debugging information
* -O1 - Sets compiler optimizations for the produced executable
* -fsanitize=fuzzer,address - This option specifies that libFuzzer and the AddressSanitizer are used.
* `pkg-config --cflags --libs $HOME/fuzzing_libexif_libfuzzer/install/lib/pkgconfig/libexif.pc` - To build this executable, the library location must be known. libexif uses the pkg-config utility to point where these are located.
* -o libexif.run - This is the name of the final executable that will be generated.

Now we run the executable created. In this example we want to focus on crashes instead of memory leaks, so we provide a flag to disable references to memory leaks. We also point to the corpus to give a template of valid JPG files.

```
./libexif.run -detect_leaks=0 my_corpus/
```

 We allow libfuzzer to run until it identifies a crash. 


 Running this application results in a heap-buffer-overflow attributed to [CVE-2012-2836
](https://www.cvedetails.com/cve/CVE-2012-2836/).