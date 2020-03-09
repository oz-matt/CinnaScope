
#include "cinnapcieinterface.h"
#include <stdlib.h>
#include <stdio.h>
#include "QDebug"
#include <QElapsedTimer>
#include <QQueue>

QElapsedTimer myTimer;



int nMilliseconds = myTimer.elapsed();

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
         this->pcie_read_data = new BYTE[262144];
         this->pcie_address = new DWORD;
         this->pcie_num_new_pts = new DWORD;
         *pcie_address = 0;
         *pcie_num_new_pts = 0;

         this->pcieSuccess = true;
         this->pcie_lastaddress = 0;
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

 bPass = PCIE_DmaRead(this->hPCIE, 0x100000, this->pcie_read_data, MEM_SIZE);

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


void CinnaPcieInterface::run()
{
exec();
myTimer.start();
}

QQueue<WORD> dataq;

void CinnaPcieInterface::AppendData(DWORD radd, DWORD buff_offset, DWORD n, DWORD end_address)
{
    PCIE_DmaRead(this->hPCIE, radd, (void*)(this->pcie_read_data + buff_offset), n);

    int nwords = n >> 2;
    int j;
    for(j=0;j<nwords;j=j+2)
    {
        WORD next = pcie_read_data[buff_offset + j] << 8 | pcie_read_data[buff_offset + j + 1];
        dataq.enqueue(next);
    }

    //*pcie_address = end_address;
    //*pcie_num_new_pts = *pcie_num_new_pts + n;
    //if (*pcie_num_new_pts > 12000) *pcie_num_new_pts = 12000;
}

void CinnaPcieInterface::lockDataMutex()
{
    mutex.lock();
}

void CinnaPcieInterface::unlockDataMutex()
{
    mutex.unlock();
}

qint64 ltime = 0;

int CinnaPcieInterface::exec()
{
 while(pcieSuccess)
 {

     DWORD address = 0;
     this->Get_BRAM_Address_Pointer(&address);
     //this->updateOscData();

     bool wrap = false;
     int wrap_spacing = 0;

     int numNewPoints = 0;

     int diff = address - this->pcie_lastaddress;

     if(diff < 0)
     {
         wrap = true;
         wrap_spacing = 32767 - this->pcie_lastaddress;
         numNewPoints = wrap_spacing + address + 1;
     }
     else
     {
         numNewPoints = address - this->pcie_lastaddress;
     }

     qint64 mtime = myTimer.nsecsElapsed();
     qDebug("Address: %i, Newpts: %d, nanos: %d, wrap: %d, diff: %d", address, numNewPoints, (int)(mtime-ltime), wrap, diff);
     ltime = mtime;

     if(numNewPoints > 0)
     {

     lockDataMutex();
     if (wrap)
     {
         if (wrap_spacing > 0)
         {

             DWORD start_address = (this->pcie_lastaddress + 1) << 3;
             DWORD rlen = wrap_spacing << 3;

             AppendData(0x100000 + start_address, start_address, rlen, address);
         }

         DWORD start_address2 = 0;
         DWORD rlen2 = address << 3;
         AppendData(0x100000 + start_address2, start_address2, rlen2, address);

     }
     else
     {
         DWORD start_address = (this->pcie_lastaddress + 1) << 3;
         DWORD rlen = numNewPoints << 3;

         AppendData(0x100000 + start_address, start_address, rlen, address);

     }
     unlockDataMutex();

    }
     this->pcie_lastaddress = address;

 }
}

