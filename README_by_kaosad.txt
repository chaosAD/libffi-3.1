To configure type:
  ./configure --prefix=/C/Vennie/Projects/C/libffi-3.1/result CC=/C/MinGW/bin/gcc.exe CXX=/C/MinGW/bin/g++.exe

which will use gcc and g++ from the path provided and install into
'/result' folder

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
