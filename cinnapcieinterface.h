#ifndef CINNAPCIEINTERFACE_H
#define CINNAPCIEINTERFACE_H

#include "pcie_lib/PCIE.h"
#include <QtGlobal>
#include <QVector>
#include <QThread>
#include <QMutex>

#define DEMO_PCIE_USER_BAR			PCIE_BAR0
#define MEM_SIZE			(128*2048) //128KB

class CinnaPcieInterface : public QThread
{
public:
    CinnaPcieInterface();

    BOOL Get_BRAM_Address_Pointer(DWORD* address);
    BOOL updateOscData();

    PCIE_HANDLE getHandle();
    void* getLibHandle();
    void run();
    int exec();
    bool pcieSuccess;
    DWORD pcie_lastaddress;

    void lockDataMutex();
    void unlockDataMutex();

    //Mutex protected

    BYTE* pcie_read_data;
    DWORD* pcie_address;
    DWORD* pcie_num_new_pts;

private:

    void AppendData(DWORD radd, DWORD buff_offset, DWORD n, DWORD end_address);

    QMutex mutex;
    void* lib_handle;
    PCIE_HANDLE hPCIE;
};


#endif // CINNAPCIEINTERFACE_H
