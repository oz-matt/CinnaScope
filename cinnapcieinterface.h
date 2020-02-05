#ifndef CINNAPCIEINTERFACE_H
#define CINNAPCIEINTERFACE_H

#include "pcie_lib/PCIE.h"
#include <QtGlobal>
#include <QVector>

#define DEMO_PCIE_USER_BAR			PCIE_BAR0
#define MEM_SIZE			(128*1024) //128KB

class CinnaPcieInterface
{
public:
    CinnaPcieInterface();

    BOOL Get_BRAM_Address_Pointer(DWORD* address);
    BOOL updateOscData();

    PCIE_HANDLE getHandle();
    void* getLibHandle();

    bool pcieSuccess;
    QVector<quint64>* pcie_read_data;

private:

    void* lib_handle;
    PCIE_HANDLE hPCIE;
};


#endif // CINNAPCIEINTERFACE_H
