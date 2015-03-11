Configuring Build
=================
To configure type:
  ./configure --prefix=/C/Vennie/Projects/C/libffi-3.1/result CC=/C/MinGW/bin/gcc.exe CXX=/C/MinGW/bin/g++.exe

which will use gcc and g++ from the path provided and install into
'/result' folder. Please change the absolute paths if necessary.

Build
=====
Due to some unknown reason, it seems "configure" does not make a copy of
"install-sh" in the root folder to "i686-pc-mingw32" folder when it should.
This will cause subsequent steps to fail, so just manually copy it.

Then run:
  make install

"Make" will compile and link. But near the last steps, "make" will freeze at
"ranlib .libs/libffi.a" command (executed through "libtool"). Just Ctrl-C to
break the process. Now if you issue "make install" again it will complain
'install fail' for unknown reason. To overcome this problem, issue:
  make clean
  make install

To overcome the "make install" freezing problem, you can run step-by-step
what "make" would have done. But, the "makefile" is too cyptic! What can be done
is run the make install in dry-run mode, which means running but not executing.
This avoids the freezing and it will dump all commands to build the library:
  make clean
  make install --dry-run > build.log
  make install

The last step above is to really "make install". Then Crtl-C to break out from
the process. Open the "build.log". Then "cd i686-pc-mingw32". Up to this point, we
have executed until the following command at line 154 in "build.log":
  C:/ProgramFiles_x86/Git/bin/sh.exe ./libtool  --tag=CC   --mode=link c:/MinGW/bin/gcc.exe  -O3 -fomit-frame-pointer -fstrict-aliasing -ffast-math -march=core2  -Wall -fexceptions -no-undefined -version-info `grep -v '^#' ../libtool-version`  -no-undefined -bindir "c:/Vennie/Projects/C/libffi-3.1/result/bin"  -o libffi.la -rpath c:/Vennie/Projects/C/libffi-3.1/result/lib src/prep_cif.lo src/types.lo src/raw_api.lo src/java_raw_api.lo src/closures.lo      src/x86/ffi.lo src/x86/win32.lo

So cut and paste all the commands starting from that line. Take note that you
have to execute one command at a time. The commands are separated by ":". Make
sure each command executes without error.

Once you have finished (exclude running make[3]:...). You have the library
installed in "result" folder.

Writing First Program using LibFFI
==================================
Then create "PutFFI.c" file in /result folder:

  #include <stdio.h>
  #include "lib/libffi-3.1/include/ffi.h"
  int main() {
    ffi_cif cif;
    ffi_type *args[1];
    void *values[1];
    char *s;
    ffi_arg rc;
    /* Initialize the argument info vectors */
    args[0] = &ffi_type_pointer;
    values[0] = &s;
    /* Initialize the cif */
    if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1,              \
                     &ffi_type_sint, args) == FFI_OK) {
      s = "Hello World!";
      ffi_call(&cif, puts, &rc, values);
      /* rc now holds the result of the call to puts */
      /* values holds a pointer to the function's arg, so to
         call puts() again all we need to do is change the
         value of s */
      s = "This is cool!";
      ffi_call(&cif, puts, &rc, values);
    }
    return 0;
  }

The program is taken from the LibFFI documentation. The include path to
"ffi.h" has been modified to reflect where it is, relative to where
PutFFI.c is located. Then create "compile_n_run_it" file /result folder:

  #! /bin/sh
  gcc -o bin/PutFFI.exe ./PutFFI.c -llibffi -Llib
  ./bin/PutFFI.exe

After running "compile_n_run_it", you will see the following error:

  In file included from ./PutFFI.c:2:0:
  ./lib/libffi-3.1/include/ffi.h:67:23: fatal error: ffitarget.h: No such file or
  directory
   #include <ffitarget.h>
                         ^
  compilation terminated.
  ./compile_n_run_it: line 3: ./bin/PutFFI.exe: No such file or directory

You have to change (at the location specified) from
  #include <ffitarget.h>

to
  #include "ffitarget.h"

After running "compile_n_run_it", you will see the following warning and output:

  ./PutFFI.c: In function 'main':
  ./PutFFI.c:16:5: warning: passing argument 2 of 'ffi_call' from incompatible poin
  ter type [enabled by default]
       ffi_call(&cif, puts, &rc, values);
       ^
  In file included from ./PutFFI.c:2:0:
  ./lib/libffi-3.1/include/ffi.h:436:6: note: expected 'void (*)(void)' but argumen
  t is of type 'int (__attribute__((__cdecl__)) *)(const char *)'
   void ffi_call(ffi_cif *cif,
        ^
  ./PutFFI.c:22:5: warning: passing argument 2 of 'ffi_call' from incompatible poin
  ter type [enabled by default]
       ffi_call(&cif, puts, &rc, values);
       ^
  In file included from ./PutFFI.c:2:0:
  ./lib/libffi-3.1/include/ffi.h:436:6: note: expected 'void (*)(void)' but argumen
  t is of type 'int (__attribute__((__cdecl__)) *)(const char *)'
   void ffi_call(ffi_cif *cif,
        ^
  Hello World!
  This is cool!

Take note that the compilation/linking and running is successful. It produces the
expected output:
  Hello World!
  This is cool!

To suppress the warning go to the line and file specified and replace:
  void (*fn)(void),

with
  int (__attribute__((__cdecl__)) *fn)(const char *),

There should be no more warning after that change. Take note that you will see
similar warning elsewhere if you use the rest of the LibFFI functions. Do similar
replacement and all are good.
