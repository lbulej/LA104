#include "Bios.h"
#include "../../lib/STM32F10x_StdPeriph_Driver/inc/misc.h"
//      NVIC_SetVectorTable(NVIC_VectTab_RAM, (uint32_t)(g_pfnRamVectorTable - NVIC_VectTab_RAM));

namespace BIOS
{
  namespace OS
  {
    char strGlobalArgument[32];

    void SetArgument(char* argument)
    {
      _ASSERT(strlen(argument) < sizeof(strGlobalArgument)-1);
      strcpy(strGlobalArgument, argument);
    }

    char* GetArgument()
    {
      return strGlobalArgument;
    }


    extern "C" void (*g_pfnVectors[])(void);
    extern "C" TInterruptHandler g_pfnRamVectorTable[];

    void CopyVectorTable()
    {
      if ((uint32_t)g_pfnVectors[0] == (uint32_t)g_pfnRamVectorTable[0])
        return;

      memcpy(g_pfnRamVectorTable, g_pfnVectors, 76*4);

      NVIC_SetVectorTable(NVIC_VectTab_RAM, (uint32_t)(g_pfnRamVectorTable - NVIC_VectTab_RAM));
    }

    TInterruptHandler GetInterruptVector(EInterruptVector i)
    {
      CopyVectorTable();
      return g_pfnRamVectorTable[i];
    }

    void SetInterruptVector(EInterruptVector i, TInterruptHandler handler)
    {
      CopyVectorTable();
      g_pfnRamVectorTable[i] = handler;
    }

  }
}