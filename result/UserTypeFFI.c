#include <stdio.h>
#include <assert.h>
#include "lib/libffi-3.1/include/ffi.h"

#define DELTA 0.0000001

typedef struct {
  short int           shortIntVal;
  double              doubleVal;
  char                charVal;
  float               floatVal;
  long                longVal;
  long                *longPtr;
} TestStruct;

double test(TestStruct ts) {
  printf("%d, %f, %d, %f, %d, %d\n",
                    ts.shortIntVal,
                    ts.doubleVal,
                    ts.charVal,
                    ts.floatVal,
                    ts.longVal,
                    *(ts.longPtr));
  assert(ts.shortIntVal == 1);
  assert(ts.doubleVal >= 2.3456 - DELTA && ts.doubleVal <= 2.3456 + DELTA);
  assert(ts.charVal == 7);
  assert(ts.floatVal >= 8.901 - DELTA && ts.floatVal <= 8.901 + DELTA);
  assert(ts.longVal == 234567891);
  assert(*(ts.longPtr) == 1234567890);
}

int main() {
  ffi_cif cif;
  ffi_type testStruct;
  ffi_type *args[7];
  ffi_type *funcArgTypes[1];
  void *values[1];
  char *s;
  ffi_arg rc;

  // Define user type TestStruct
  args[0] = &ffi_type_sint;
  args[1] = &ffi_type_double;
  args[2] = &ffi_type_schar;
  args[3] = &ffi_type_float;
  args[4] = &ffi_type_slong;
  args[5] = &ffi_type_pointer;
  args[6] = NULL;
  testStruct.size = testStruct.alignment = 0;
  testStruct.type = FFI_TYPE_STRUCT;
  testStruct.elements = args;

  // Prepare the argument type of test(), which is TestStruct
  // defined above
  funcArgTypes[0] = &testStruct;

  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1,              \
                   &ffi_type_double, funcArgTypes) == FFI_OK) {
    long myLongVal = 1234567890;
    TestStruct ts = { 1, 2.3456, 7, 8.901, 234567891, &myLongVal};
    values[0] = &ts;
    ffi_call(&cif, FFI_FN(test), &rc, values);
  }
  return 0;
}
