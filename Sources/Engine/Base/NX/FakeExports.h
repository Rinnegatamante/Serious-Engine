#ifndef SE_INCL_FAKEEXPORTS_H
#define SE_INCL_FAKEEXPORTS_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <string>
#include <map>
#include <initializer_list>

#ifdef NDEBUG
#define DLLPOSTFIX
#else
#define DLLPOSTFIX "D"
#endif

class CFakeLib
{
public:
    CFakeLib(const char *libname, std::initializer_list<std::pair<std::string const, void*>> syms);
    void AddExport(const char *sym, void *val);
    void *GetExport(const char *sym);

private:
    const char *name;
    std::map<std::string, void*> symtab;
};

class CFakeExportRegistry
{
public:
    static CFakeLib *GetLib(const char *libname);
    static void AddLib(const char *libname, CFakeLib *lib);

private:
    static std::map<std::string, CFakeLib*> libmap;
};

#endif // SE_INCL_FAKEEXPORTS_H
