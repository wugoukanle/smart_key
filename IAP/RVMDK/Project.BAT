SET PATH=C:\Keil\ARM\ARMCC\bin\;C:\PROGRAM FILES\NVIDIA GPU COMPUTING TOOLKIT\CUDA\V6.0\BIN;C:\PROGRAM FILES\NVIDIA GPU COMPUTING TOOLKIT\CUDA\V6.0\LIBNVVP;C:\PROGRAM FILES\COMMON FILES\MICROSOFT SHARED\WINDOWS LIVE;C:\PROGRAM FILES (X86)\COMMON FILES\MICROSOFT SHARED\WINDOWS LIVE;C:\WINDOWS\SYSTEM32;C:\WINDOWS;C:\WINDOWS\SYSTEM32\WBEM;C:\WINDOWS\SYSTEM32\WINDOWSPOWERSHELL\V1.0\;C:\PROGRAM FILES (X86)\WINDOWS LIVE\SHARED;C:\PROGRAM FILES (X86)\MICROSOFT SQL SERVER\100\TOOLS\BINN\;C:\PROGRAM FILES (X86)\MICROSOFT SQL SERVER\100\DTS\BINN\;C:\CTEX\USERDATA\MIKTEX\BIN;C:\CTEX\MIKTEX\MIKTEX\BIN;C:\CTEX\CTEX\CTEX\BIN;C:\CTEX\CTEX\CCT\BIN;C:\CTEX\CTEX\TY\BIN;C:\CTEX\GHOSTSCRIPT\GS9.05\BIN;C:\CTEX\GSVIEW\GSVIEW;C:\CTEX\WINEDT;C:\PROGRAM FILES\MATLAB\R2011B\RUNTIME\WIN64;C:\PROGRAM FILES\MATLAB\R2011B\BIN;C:\Cadence\SPB_16.5\OpenAccess\bin\win32\opt;C:\Cadence\SPB_16.5\tools\Capture;C:\Cadence\SPB_16.5\tools\PSpice\Library;C:\Cadence\SPB_16.5\tools\PSpice;C:\Cadence\SPB_16.5\tools\specctra\bin;C:\Cadence\SPB_16.5\tools\fet\bin;C:\Cadence\SPB_16.5\tools\libutil\bin;C:\Cadence\SPB_16.5\tools\bin;C:\Cadence\SPB_16.5\tools\pcb\bin;D:\Program Files\opencv\build\x86\vc10\bin;D:\Program Files\opencv2.3.1\opencv\build\x86\vc10\bin;D:\Program Files\opencv1.0\bin
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Project
SET CPU_CLOCK=0x007A1200
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\main.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_it.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\upload.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\ymodem.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\common.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\download.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\oled.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\delay.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\spi_flash.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\misc.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_flash.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_rcc.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_gpio.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_usart.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_spi.__i"
"C:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\output\startup_stm32f10x_md._ia"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\Output\Project.lnp"
C:\Keil\ARM\ARMCC\bin\fromelf.exe ".\Output\Project.axf" --i32combined --output ".\Output\Project.hex"
