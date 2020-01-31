#ifndef CINNAPCIEINTERFACE_H
#define CINNAPCIEINTERFACE_H

#include "pcie_lib/PCIE.h"

class CinnaPcieInterface
{
public:
    CinnaPcieInterface();

    PCIE_HANDLE getHandle();
    void* getLibHandle();

private:

    void* lib_handle;
    PCIE_HANDLE hPCIE;
};

#endif // CINNAPCIEINTERFACE_H
