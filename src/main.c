#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "box86version.h"
#include "debug.h"
#include "box86context.h"
#include "fileutils.h"
#include "elfloader.h"
#include "stack.h"
#include "x86emu.h"
#include "x86run.h"
#include "x86trace.h"

int box86_debug = DEBUG_INFO;//DEBUG_NONE;

void LoadDebugEnv()
{
    const char *p = getenv("BOX86_DEBUG");
    if(p) {
        if(strlen(p)==1) {
            if(p[0]>='0'+DEBUG_NONE && p[1]<='0'+DEBUG_DEBUG)
                box86_debug = p[0]-'0';
        } else {
            if(!strcasecmp(p, "NONE"))
                box86_debug = DEBUG_NONE;
            else if(!strcasecmp(p, "INFO"))
                box86_debug = DEBUG_INFO;
            else if(!strcasecmp(p, "DEBUG"))
                box86_debug = DEBUG_DEBUG;
            else if(!strcasecmp(p, "DUMP"))
                box86_debug = DEBUG_DUMP;
        }
        printf_debug(DEBUG_INFO, "Debug level is %d\n", box86_debug);
    }
}

void LoadEnvPath(path_collection_t *col, const char* defpath, const char* env)
{
    const char* p = getenv(env);
    if(p) {
        printf_debug(DEBUG_INFO, "%s: ", env);
        ParseList(p, col);
    } else {
        printf_debug(DEBUG_INFO, "Using default %s: ", env);
        ParseList(defpath, col);
    }
    if(DEBUG_INFO<=box86_debug) {
        for(int i=0; i<col->size; i++)
            printf("%s%s", col->paths[i], (i==col->size-1)?"\n":":");
    }
}

void PrintHelp() {
    printf("\n\nThis is Box86, the Linux 86 emulator with a twist\n");
    printf("\nUsage is box86 path/to/software [args]\n");
    printf("to launch x86 software\n");
    printf("You can also set some env. var.:\n");
    printf(" BOX86_PATH to set the PATH used by box86 to find w86 programs (default is '.:bin')\n");
    printf(" BOX86_LD_LIBRARY_PATH to set the path were x86 lib are searched (default is '.:lib')\n");
    printf(" BOX86_DEBUG with 0/1/2 or NONE/INFO/DEBUG to set printed debug info\n");
}

int main(int argc, const char **argv) {
    printf("Box86 v%d.%d.%d\n", BOX86_MAJOR, BOX86_MINOR, BOX86_REVISION);

    // trying to open and load 1st arg
    if(argc==1) {
        PrintHelp();
        return 1;
    }

    // init random seed
    srandom(time(NULL));

    // check BOX86_DEBUG debug level
    LoadDebugEnv();
    
    // Create a new context
    box86context_t *context = NewBox86Context(argc - 1);

    const char *p;
    // check BOX86_LD_LIBRARY_PATH and load it
    LoadEnvPath(&context->box86_ld_lib, ".:lib", "BOX86_LD_LIBRARY_PATH");
    // check BOX86_PATH and load it
    LoadEnvPath(&context->box86_path, ".:bin", "BOX86_PATH");

    p = getenv("BOX86_TRACE");
    if(p) {
        if (strcmp(p, "1")==0)
            context->x86trace = 1;
    }
    if(context->x86trace) {
        printf_debug(DEBUG_INFO, "Initializing Zydis lib\n", p);
        if(InitX86Trace(context)) {
            printf_debug(DEBUG_INFO, "Zydis init failed, no x86 trace activated\n", p);
            context->x86trace = 0;
        }
    }

    // lets build argc/argv stuff
    p=argv[1];
    printf_debug(DEBUG_INFO, "Looking for %s\n", p);
    if(strchr(p, '/'))
        context->argv[0] = strdup(p);
    else
        context->argv[0] = ResolveFile(p, &context->box86_path);
    for(int i=1; i<context->argc; ++i)
        context->argv[i] = strdup(argv[i+1]);
    // check if file exist
    if(!context->argv[0]) {
        printf_debug(DEBUG_NONE, "Error, file is not found (check BOX86_PATH)\n", p);
        FreeBox86Context(&context);
        return -1;
    }
    if(!FileExist(context->argv[0], IS_FILE|IS_EXECUTABLE)) {
        printf_debug(DEBUG_NONE, "Error, file %s is not found\n", context->argv[0]);
        FreeBox86Context(&context);
        return -1;
    }
    FILE *f = fopen(context->argv[0], "rb");
    if(!f) {
        printf_debug(DEBUG_NONE, "Error, Cannot open %s\n", context->argv[0]);
        FreeBox86Context(&context);
        return -1;
    }
    elfheader_t *elf_header = LoadAndCheckElfHeader(f, context->argv[0], 1);
    if(!elf_header) {
        printf_debug(DEBUG_NONE, "Error, reading elf header of %s\n", context->argv[0]);
        fclose(f);
        FreeBox86Context(&context);
        return -1;
    }
    int mainelf = AddElfHeader(context, elf_header);

    if(CalcLoadAddr(elf_header)) {
        printf_debug(DEBUG_NONE, "Error, reading elf header of %s\n", context->argv[0]);
        fclose(f);
        FreeBox86Context(&context);
        return -1;
    }
    // allocate memory
    if(AllocElfMemory(elf_header)) {
        printf_debug(DEBUG_NONE, "Error, allocating memory for elf %s\n", context->argv[0]);
        fclose(f);
        FreeBox86Context(&context);
        return -1;
    }
    // Load elf into memory
    if(LoadElfMemory(f, elf_header)) {
        printf_debug(DEBUG_NONE, "Error, loading in memory elf %s\n", context->argv[0]);
        fclose(f);
        FreeBox86Context(&context);
        return -1;
    }
    // can close the file now
    fclose(f);
    // Call librarian to load all dependant elf
    // finalize relocations
    if(RelocateElf(elf_header)) {
        printf_debug(DEBUG_NONE, "Error, relocating symbols in elf %s\n", context->argv[0]);
        FreeBox86Context(&context);
        return -1;
    }
    // get and alloc stack size and align
    if(CalcStackSize(context)) {
        printf_debug(DEBUG_NONE, "Error, allocating stack\n");
        FreeBox86Context(&context);
        return -1;
    }
    // set entrypoint
    context->ep = GetEntryPoint(elf_header);
    // init x86 emu
    context->emu = NewX86Emu(context, context->ep, (uintptr_t)context->stack, context->stacksz);
    SetEAX(context->emu, context->argc);
    SetEBX(context->emu, (uint32_t)context->argv);
    // emulate!
    printf_debug(DEBUG_DEBUG, "Start x86emu on Main\n");
    Run(context->emu);
    // Get EAX
    int ret = GetEAX(context->emu);
    printf_debug(DEBUG_DEBUG, "Emulation finished, EAX=%d\n", ret);


    // all done, free context
    FreeBox86Context(&context);

    return ret;
}