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

PCIE_HANDLE CinnaPcieInterface::getHandle()
{
    return this->hPCIE;
}

void* CinnaPcieInterface::getLibHandle()
{
    return this->lib_handle;
}
