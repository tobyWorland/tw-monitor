# ARM CPU Exceptions

## Exception number / ISR_NUMBER
This number can be found in the IPSR (which composes the xPSR) and indicates if the processor is in normal thread mode, or is handing an exception in handler mode. It is also useful in the vector table which holds the pointers to the exception handlers. This number is also reported in this project's hardfault handler which when unwinding exceptions will display the exception being unwound.

List taken from "Cortex-M4 Devices Generic User Guide" Table 2-5 IPSR bit assignments
 * 0    = Thread mode
 * 1    = Reserved
 * 2    = NMI
 * 3    = HardFault
 * 4    = MemManage
 * 5    = BusFault
 * 6    = UsageFault
 * 7-10 = Reserved
 * 11   = SVCall
 * 12   = Reserved for Debug
 * 13   = Reserved
 * 14   = PendSV
 * 15   = SysTick
 * 16   = IRQ0
 * 17   = IRQ1
 * 18   = IRQ2
 * 19   = IRQ3
 * ...
 * N+15 = IRQ(N-1)
