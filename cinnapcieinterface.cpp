
#include "cinnapcieinterface.h"
#include <stdlib.h>
#include <stdio.h>
#include "QDebug"
#include <QElapsedTimer>

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
         this->pcie_read_data = new quint64[32768];
         this->pcieSuccess = true;
         this->pcie_address = 0;
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

void CinnaPcieInterface::ConvertAndAppendData(quint64 data)
{
    WORD y = (data) >> 48;
    qint16 ytc = (qint16)(y << 2) / 4 ; // convert 2s comp to signed int
    double yvolts = ytc * (double)0.01220703125;

    mutex.lock();
    newpts.append(yvolts);
    mutex.unlock();
    //const QPointF s( curr_time, yvolts);
    //SignalData::instance().append( s );
    //curr_time += TIMESTEP;
    //qDebug("Address: %u, Data: %X, Volts: %f", address, data, yvolts);
}

int CinnaPcieInterface::exec()
{
 while(pcieSuccess)
 {

     DWORD address = 0;
     this->Get_BRAM_Address_Pointer(&address);
     //this->updateOscData();
     this->pcie_address = address;

     bool wrap = false;
     int wrap_spacing = 0;

     int numNewPoints = 0;

     if((signed int)(this->pcie_address - this->pcie_lastaddress) < 0)
     {
         wrap = true;
         wrap_spacing = 32767 - this->pcie_lastaddress;
         numNewPoints = wrap_spacing + this->pcie_address + 1;
     }
     else
     {
         numNewPoints = this->pcie_address - this->pcie_lastaddress;
     }

     qDebug("Address: %i, Newpts: %d, nanos: %d", address, numNewPoints, myTimer.nsecsElapsed());

     if(numNewPoints > 0)
     {

     if (wrap)
     {
         if (wrap_spacing > 0)
         {


             unsigned int start_address = (this->pcie_lastaddress + 1) * 8;
             unsigned int rlen = wrap_spacing * 8;

             quint64* bpts = new quint64[wrap_spacing];

             if(PCIE_DmaRead(this->hPCIE, 0x100000 + start_address, bpts, rlen))
             {
             int j;
             for(j=0;j<wrap_spacing;j++)
             {
                 ConvertAndAppendData(bpts[j]);
             }
             delete bpts;
         }
              else
              {
                  qDebug("dmaread fail1");
              }

         }

         unsigned int start_address2 = 0;
         unsigned int rlen2 = address * 8;

         quint64* bpts2 = new quint64[address];

         if(PCIE_DmaRead(this->hPCIE, 0x100000 + start_address2, bpts2, rlen2))
         {
         int j;
         for(j=0;j<address;j++)
         {
             ConvertAndAppendData(bpts2[j]);
         }
         delete bpts2;
     }
          else
          {
              qDebug("dmaread fail2");
          }

     }
     else
     {

         unsigned int start_address = (this->pcie_lastaddress + 1) * 8;
         unsigned int rlen = numNewPoints * 8;

         quint64* bpts = new quint64[numNewPoints];

         if(PCIE_DmaRead(this->hPCIE, 0x100000 + start_address, bpts, rlen))
{
         int j;
         for(j=0;j<numNewPoints;j++)
         {
             ConvertAndAppendData(bpts[j]);
         }
         delete bpts;
}
     else
     {
         qDebug("dmaread fail3");
     }
     }
     //sleep(.00001);
    }
     this->pcie_lastaddress = address;

 }
}

