path %psth%;C:\Xilinx\Vivado\2018.3\bin
start "C:\Windows\System32\cmd.exe" 
@echo Produce the vivado project.
 
set cache_floder=.Xil
cd  %~dp0
if exist %~dp0%cache_floder% ( 
    echo The floder is exist.
    pause
) else (
    vivado -cd F:/jiang/worked_project/git_DMA_UDP_BRAM/rebuild 
    vivado -pwd
    vivado -source ./DMA_BRAM.tcl
)
exit