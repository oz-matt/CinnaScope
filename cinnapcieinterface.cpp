#include "cinnapcieinterface.h"
#include <stdlib.h>
#include <stdio.h>
#include "QDebug"

CinnaPcieInterface::CinnaPcieInterface()
{

    this->pcieSuccess = false;

    this->lib_handle = PCIE_Load();
    if (this->lib_handle)
    {
        qDebug("PCIE_Load Success!!!");
        this->hPCIE = PCIE_Open(0,0,0);
        if (this->hPCIE)
        {
            qDebug("PCIE_Load Success!!!");
            this->pcie_read_data = (quint64*)malloc(16384 * sizeof(quint64*));
            this->pcieSuccess = true;
        }
        else
        {
            qDebug("PCIE_Load Failed!");
        }
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

BOOL CinnaPcieInterface::updateOscData()
{
    BOOL bPass = TRUE;

    bPass = PCIE_DmaRead(this->hPCIE, 0x20000, this->pcie_read_data, MEM_SIZE);

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
