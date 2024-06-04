set toolpath=C:\TelinkIoTStudio\RDS\V5.1.2\toolchains\nds32le-elf-mculib-v5\bin\
set cygwinpath=C:\TelinkIoTStudio\RDS\V5.1.2\cygwin\bin\
set PATH=%toolpath%;%cygwinpath%;%PATH%

rm -f tlua_lib.a

find ./lua -name "*.o" -type f -print -exec riscv32-elf-ar -r tlua_lib.a  {} ;
find ./ymodem -name "*.o" -type f -print -exec riscv32-elf-ar -r tlua_lib.a  {} ;
find ./ff15 -name "*.o" -type f -print -exec riscv32-elf-ar -r tlua_lib.a  {} ;

