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
what "make" would have done. But, the "makefile" is too cyptic! What can be
done is to run the make install in dry-run mode, which means running but not
executing. This avoids the freezing and it will dump all commands to build
the library:
  make clean
  make install --dry-run > build.log
  make install

The last step above is to really "make install". Then Crtl-C to break out
from the process. Open the "build.log". Then "cd i686-pc-mingw32". Up to
this point, we have executed until the following command at line 154 in
"build.log":
  C:/ProgramFiles_x86/Git/bin/sh.exe ./libtool  --tag=CC   --mode=link c:/MinGW/bin/gcc.exe  -O3 -fomit-frame-pointer -fstrict-aliasing -ffast-math -march=core2  -Wall -fexceptions -no-undefined -version-info `grep -v '^#' ../libtool-version`  -no-undefined -bindir "c:/Vennie/Projects/C/libffi-3.1/result/bin"  -o libffi.la -rpath c:/Vennie/Projects/C/libffi-3.1/result/lib src/prep_cif.lo src/types.lo src/raw_api.lo src/java_raw_api.lo src/closures.lo      src/x86/ffi.lo src/x86/win32.lo

So cut and paste all the commands starting from that line. Take note that
you have to execute one command at a time. The commands are separated by
":". Make sure each command executes without error.

Once you have finished (exclude running make[3]:...). You have the library
installed in "result" folder.

Writing First Program using LibFFI
==================================
Then create "PutFFI.c" file in /result folder:

  #include <stdio.h>
  #include <ffi.h>

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
      ffi_call(&cif, FFI_FN(puts), &rc, values);
      /* rc now holds the result of the call to puts */
      /* values holds a pointer to the function's arg, so to
         call puts() again all we need to do is change the
         value of s */
      s = "This is cool!";
      ffi_call(&cif, FFI_FN(puts), &rc, values);
    }
    return 0;
  }

The program is taken from the LibFFI documentation with a modification.
The FFI_FN() macro is used to cast the function address to proper type to
suppress warnings.

Create "compile_n_run_it" file /result folder:

  #! /bin/sh
  gcc -o bin/PutFFI.exe ./PutFFI.c -llibffi -Llib -Ilib/libffi-3.1/include
  ./bin/PutFFI.exe

Issue "compile_n_run_it" and the following output should be
printed on the console:
  Hello World!
  This is cool!

User Defined Type
=================
The "UserTypeFFI.c" contains code example of how to create user defined type
and how it can be passed by value. To build and run the program, type:
  test_it
