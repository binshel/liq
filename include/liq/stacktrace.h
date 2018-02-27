/**
 * @file stacktrace.h
 * @brief 打印进程堆栈
 */


#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unwind.h>
#include <bfd.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <string.h>
#include <limits.h>

#include <map>


struct call_site 
{
    bool found;
    void *addr;             // 函数地址
    uint64_t foffset;       // 在文件的偏移

    const char *filename;
    char *funcname;
    uint32_t line;

    call_site();
};

struct call_stack 
{
    uint32_t size;
    struct call_site sites[256];

    explicit call_stack();
    ~call_stack();
};

class Bfd
{
public:
    bfd *abfd;
    struct bfd_symbol **symtab;
    struct bfd_symbol **dsymtab;

    Bfd() = delete;
    explicit Bfd(const char *file_path);

    struct find_symbol_content
    {
        Bfd *self;
        call_site *site;
    };
    void find_symbol(struct call_site *site);
    void find_in_section(asection *section, struct call_site *site);

    static void find_in_section_trampolin(bfd *abfd, asection *section, void *data);
};

class BfdFactory
{
public:
    static inline BfdFactory& instance()
    {
        if (!factory) {
            factory = new BfdFactory();
        }
        return *factory;
    }
    static inline Bfd* get_bfd(const char *path)
    {
        return BfdFactory::instance()._get_bfd(path);
    }
    Bfd* _get_bfd(const char *path);
private:
    std::map<std::string, Bfd*> bfds;

    static BfdFactory *factory;
};


class StackTrace 
{
public:
    static void get(call_stack *stack);

    static _Unwind_Reason_Code unwind_trace_cb
        (struct _Unwind_Context * context, void *_stack);

    static void print();
};

