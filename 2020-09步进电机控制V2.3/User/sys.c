#include "sys.h"


__asm void WFI_SET(void)
{
	//__ASM volatile("wfi");		
	WFI;
}
//�ر������ж�
__asm void INTX_DISABLE(void)
{		  
	//__ASM volatile("cpsid i");
	CPSID i;
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	//__ASM volatile("cpsie i");		
	CPSIE i;	
}
//����ջ����ַ
//addr:ջ����ַ

__ASM void MSR_MSP(u32 addr)
{
	MSR MSP, r0 //set Main Stack value
	BX r14
}


/*void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
	__ASM volatile("MSR MSP, r0"); 
__ASM volatile("BX r14");
}
*/

