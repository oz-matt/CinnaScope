#include "cinnapcieinterface.h"
#include "QDebug"

CinnaPcieInterface::CinnaPcieInterface()
{
    this->lib_handle = PCIE_Load();
    if (this->lib_handle)
    {
        qDebug("PCIE_Load Success!!!");
    }
    else
    {
        qDebug("PCIE_Load Failed!");
    }
}

BOOL CinnaPcieInterface::Get_BRAM_Address_Pointer(DWORD* address)
{
    BOOL bPass = TRUE;

    bPass = PCIE_Read32(this->hPCIE, DEMO_PCIE_USER_BAR, 0x20, address);

    return bPass;
}

PCIE_HANDLE CinnaPcieInterface::getHandle()
{
    return this->hPCIE;
}

void* CinnaPcieInterface::getLibHandle()
{
    return this->lib_handle;
}
