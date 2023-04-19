#include <dlfcn.h>
#include <stdio.h>

namespace { //=================================================================

namespace _1 {

void test()
{
#if (1)
    const char* libname = "/usr/lib/x86_64-linux-gnu/libglut.so";
#else
    const char* libname = "/usr/lib/x86_64-linux-gnu/lib____.so";
#endif

    // RTLD_GLOBAL
    // RTLD_LOCAL (default)
    // RTLD_NODELETE
    //  check if load
    //  change flag (RTLD_LOCAL <-> RTLD_GLOBAL)
    // RTLD_DEEPBIND
    //  prefer your own symbols (Linux only)
    void* hlib = dlopen(libname, RTLD_NOW); //LD_BIND_NOW overwrite RTLD_LAZY
    if (NULL != hlib) {
        fprintf(stdout, "dlopen(%s) success.\n", libname);

        const char* funcname = "glutInit";
        void* glutInit = dlsym(hlib, funcname);
        if (NULL == glutInit) {
            fprintf(stderr, "dlsym(%s) error.\n", funcname);
        } else {
            fprintf(stdout, "dysum(%s) success.\n", funcname);
        }

        dlclose(hlib);
    } else {
        fprintf(stderr, "dlopen(%s) failed.\n", libname);
        fprintf(stderr, "dlerror(): %s\n", dlerror());

        // /usr/lib/x86_64-linux-gnu/lib____.so: cannot open shared object file: No such file or directory
    }
}

} //_1 --------------------------------------------------------------

/*

void __attribute__ ((constructor)) begin_lib()
{
}

void __attribute__ ((destructor)) end_lib()
{
}
*/

} //namespace =================================================================

void test_ch_37()
{
    _1::test();
}
