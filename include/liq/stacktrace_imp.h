#pragma once

#include "stacktrace.h"


call_site::call_site()
{
    memset(this, 0, sizeof(*this));
}

call_stack::call_stack() :size(0) {}
call_stack::~call_stack()
{
    for (uint32_t i = 0; i < size; ++i)
    {
        if (sites[i].funcname) {
            free(sites[i].funcname);
        }
    }
}

Bfd::Bfd(const char *file_path)
    :abfd(NULL), symtab(NULL), dsymtab(NULL)
{
    abfd = bfd_openr(file_path, "default");
    if ( !abfd        // 可以打开
            || !bfd_check_format(abfd,  bfd_object)   // 有效的对象文件
            || !(bfd_get_file_flags(abfd) & HAS_SYMS))  // 包含 debug 段
    {
        return;
    }
    uint64_t sym_size = bfd_get_symtab_upper_bound(abfd);
    if (sym_size > 0) {
        symtab = (decltype(symtab))malloc(sym_size);
        bfd_canonicalize_symtab(abfd, symtab);
    }

    uint64_t dsym_size = bfd_get_dynamic_symtab_upper_bound(abfd);
    if (dsym_size > 0) {
        dsymtab = (decltype(symtab))malloc(dsym_size);
        bfd_canonicalize_dynamic_symtab(abfd, dsymtab);
    }
}

void Bfd::find_symbol(struct call_site *site)
{
    if (!symtab && ! dsymtab) return ;

    find_symbol_content context;
    context.self = this;
    context.site = site;
    bfd_map_over_sections(abfd, find_in_section_trampolin, &context);
}
void Bfd::find_in_section(asection *section, struct call_site *site)
{
    if (site->found) return;
    if (!(bfd_get_section_flags(abfd, section) & SEC_ALLOC)) {
        return;   // 非 debug 段
    }
    uint64_t sec_addr = (decltype(sec_addr))bfd_get_section_vma(abfd, section);
    uint64_t sec_size = bfd_get_section_size(section);
    uint64_t addr;
    if (site->foffset >= sec_addr && site->foffset <= sec_addr + sec_size) {
        addr = site->foffset;
    } else if((uint64_t)site->addr >= sec_addr && (uint64_t)site->addr <= sec_addr + sec_size) {
        addr = (uint64_t)site->addr;    // 地址重定向
    } else {
        return;     // 不在此地址范围
    }

    const char *funcname;
    if (symtab) {
        site->found = bfd_find_nearest_line(abfd, section, symtab,  addr - sec_addr,
                &site->filename, &funcname, &site->line);                       
    }
    if (!site->found && dsymtab) {
        site->found = bfd_find_nearest_line(abfd, section, dsymtab, addr - sec_addr, 
                &site->filename,  &funcname, &site->line);                       
    }
    if (site->found) {
        site->funcname = __cxxabiv1::__cxa_demangle(funcname, NULL, NULL, NULL);
        if (!site->funcname) {
            site->funcname = strdup(funcname);
        }
    }
}

void Bfd::find_in_section_trampolin(bfd *abfd, asection *section, void *data)
{
    find_symbol_content *context = (decltype(context))data;
    context->self->find_in_section(section, context->site);
}

Bfd* BfdFactory::_get_bfd(const char *path)
{
    char ab_path[PATH_MAX];
    char *ret = realpath(path, ab_path);
    if (!ret) return NULL;

    auto bfd_it = bfds.find(ab_path);
    if (bfd_it == bfds.end()) {
        Bfd *abfd = new Bfd(ab_path);
        bfds[ab_path] = abfd;
        return abfd;
    } else {
        return bfd_it->second;
    }
}
BfdFactory* BfdFactory::factory = NULL;


void StackTrace::get(call_stack *stack)
{
    _Unwind_Backtrace(unwind_trace_cb, stack);
}

_Unwind_Reason_Code StackTrace::unwind_trace_cb
    (struct _Unwind_Context * context, void *_stack)
{
    
    struct call_stack *stack = (struct call_stack*)_stack;
    int ip_before_instruction = 0;  // IP的值是在第一个尚未完全执行的指令之前还是之后。
    _Unwind_Ptr ip = _Unwind_GetIPInfo(context, &ip_before_instruction);

    if (ip > 0 && !ip_before_instruction) {
        ip -= 1;
    }
    call_site *site = &stack->sites[stack->size];
    ++stack->size;
    site->addr = (void*)ip;

    Dl_info symbol_info;
    dladdr(site->addr, &symbol_info);
    site->foffset = (uint8_t*)site->addr - (uint8_t*)symbol_info.dli_fbase;

    Bfd *abfd = BfdFactory::get_bfd(symbol_info.dli_fname);
    if (abfd) {
        abfd->find_symbol(site);
    }

    if (stack->size >= sizeof(stack->sites) / sizeof(stack->sites[0])) {
        return _URC_END_OF_STACK;
    } else {
        return _URC_NO_REASON;
    }
}

void StackTrace::print()
{
    // 拿到调用栈地址
    struct call_stack stack;
    StackTrace::get(&stack);
    
    for (uint32_t i = 0; i < stack.size; ++i)
    {
        printf("%03d: %p  %s:%d (%s)\n", i, stack.sites[i].addr,
            stack.sites[i].filename,
            stack.sites[i].line,
            stack.sites[i].funcname);
    }
}

