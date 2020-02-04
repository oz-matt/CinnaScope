#ifndef CINNAPCIEINTERFACE_H
#define CINNAPCIEINTERFACE_H

#include "pcie_lib/PCIE.h"

#define DEMO_PCIE_USER_BAR			PCIE_BAR0

class CinnaPcieInterface
{
public:
    CinnaPcieInterface();

    BOOL Get_BRAM_Address_Pointer(DWORD* address);

    PCIE_HANDLE getHandle();
    void* getLibHandle();

private:

    void* lib_handle;
    PCIE_HANDLE hPCIE;
};


#endif // CINNAPCIEINTERFACE_H
