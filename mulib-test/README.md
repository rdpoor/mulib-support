# Running the mulib tests

## From scratch:

Overview:

Clone the mulib-examples repository, then clone a copy of mulib inside the
mulib-examples directory.  Running the makefile will first compile mulib, then
compile mulib-tests and run the tests.

1. `cd ProjectHome`
2. `git clone https://github.com/rdpoor/mulib-examples.git`
3. `cd mulib-examples`
4. `git clone https://github.com/rdpoor/mulib.git`
5. `cd mulib-test/tools`
6. `make test`
7. `make clean`

## To retest

If you have already cloned the mulib-examples repository, to retest using fresh
files from the remote repository, you need to pull fresh copies of both the
mulib repository and the mulib-examples repository:

1. `cd ProjectHome/mulib-examples`
2. `git fetch ; git pull`
3. `cd mulib`
4. `git fetch ; git pull`
5. `cd ../mulib-test/tools`
6. `make test`
7. `make clean`

## To fix a bug

If the unit tests report a bug, such as:

```
starting mu_test...
Assertion 'mu_pstore_items(p)[2] == &s_item2' failed at ../test/core/mu_pstore_test.c:259
```

On a linux-y system:

```
mulib-examples/mulib-test/tools% gdb ../build/mu_test
```

On macOS:

```
mulib-examples/mulib-test/tools% lldb ../build/mu_test
```

Then:

```
(gdb) list mu_pstore_test.c:259
259 	  ASSERT(mu_pstore_items(p)[2] == &s_item2);
260 	  ASSERT(mu_pstore_items(p)[3] == &s_item1);
(gdb) b 259
Breakpoint 1: where = mu_test`mu_pstore_test + 6712 at mu_pstore_test.c:259:3 ...
(gbd) r
Process 21418 launched: '.../mulib-examples/mulib-test/build/mu_test' (x86_64)

Process 21418 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
    frame #0: 0x000000010000ca08 mu_test`mu_pstore_test at mu_pstore_test.c:259:3
   256 	  ASSERT(mu_pstore_sort(p, sort_dn) == MU_PSTORE_ERR_NONE); // [d c b a]
   257 	  ASSERT(mu_pstore_items(p)[0] == &s_item4);
   258 	  ASSERT(mu_pstore_items(p)[1] == &s_item3);
-> 259 	  ASSERT(mu_pstore_items(p)[2] == &s_item2);
   260 	  ASSERT(mu_pstore_items(p)[3] == &s_item1);
   261 	
```

Now single step into the offending function, and/or examine variables, etc.
