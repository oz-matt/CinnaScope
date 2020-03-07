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
    DWORD pcie_address;
    DWORD pcie_lastaddress;
    quint64* pcie_read_data;


    void ConvertAndAppendData(quint64 data);

    QVector<double> newpts;

private:
QMutex mutex;
    void* lib_handle;
    PCIE_HANDLE hPCIE;
};


#endif // CINNAPCIEINTERFACE_H
