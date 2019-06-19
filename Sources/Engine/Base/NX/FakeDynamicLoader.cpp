/* Copyright (c) 2002-2012 Croteam Ltd. All rights reserved. */

#include <Engine/Engine.h>
#include <Engine/Base/DynamicLoader.h>
#include <Engine/Base/NX/FakeExports.h>

// this is used to statically build a map of exports
// see lib sources for more

// CFakeLib

CFakeLib::CFakeLib(const char *libname, std::initializer_list<std::pair<std::string const, void*>> syms) :
    name(libname), symtab(syms)
{
    CFakeExportRegistry::AddLib(name, this);
}

void CFakeLib::AddExport(const char *sym, void *val)
{
    symtab[sym] = val;
}

void *CFakeLib::GetExport(const char *sym)
{
    auto pos = symtab.find(sym);
    if (pos == symtab.end())
        return NULL;
    return pos->second;
}

// CFakeExportRegistry

std::map<std::string, CFakeLib*> CFakeExportRegistry::libmap;

void CFakeExportRegistry::AddLib(const char *libname, CFakeLib *lib)
{
    libmap[libname] = lib;
}

CFakeLib *CFakeExportRegistry::GetLib(const char *libname)
{
    auto pos = libmap.find(libname);
    if (pos == libmap.end())
        return NULL;
    return pos->second;
}

// CFakeDynamicLoader

class CFakeDynamicLoader : public CDynamicLoader
{
public:
    CFakeDynamicLoader(const char *libname);
    virtual ~CFakeDynamicLoader(void);
    virtual void *FindSymbol(const char *sym);
    virtual const char *GetError(void);

protected:
    void DoOpen(const char *lib);
    void SetError(const char *err);
    CFakeLib *module;
    const char *err;
};


CDynamicLoader *CDynamicLoader::GetInstance(const char *libname)
{
    return(new CFakeDynamicLoader(libname));
}


void CFakeDynamicLoader::SetError(const char *msg)
{
    err = msg;
}


const char *CFakeDynamicLoader::GetError(void)
{
    return err;
}


void *CFakeDynamicLoader::FindSymbol(const char *sym)
{
    if (!module)
    {
        SetError("lib not loaded");
        return NULL;
    }
    return module->GetExport(sym);
}


void CFakeDynamicLoader::DoOpen(const char *lib)
{
    module = CFakeExportRegistry::GetLib(lib);
    if (!module)
    {
        SetError("unknown lib name");
        return;
    }
}


CTFileName CDynamicLoader::ConvertLibNameToPlatform(const char *libname)
{
    // for the fake loader, platform filename is just the filename
    CTFileName fnm = CTString(libname);
    return fnm.FileName();
}


CFakeDynamicLoader::CFakeDynamicLoader(const char *libname)
    : module(NULL),
      err(NULL)
{
    if (libname == NULL)
        DoOpen("Internal");
    else
    {
        CTFileName fnm = ConvertLibNameToPlatform(libname);
        DoOpen(fnm);
    }
}


CFakeDynamicLoader::~CFakeDynamicLoader(void)
{

}


// end of FakeDynamicLoader.cpp ...


