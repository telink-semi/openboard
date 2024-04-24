set toolpath=C:\TelinkIoTStudio\RDS\V5.1.2\toolchains\nds32le-elf-mculib-v5\bin\
set cygwinpath=C:\TelinkIoTStudio\RDS\V5.1.2\cygwin\bin\
set PATH=%toolpath%;%cygwinpath%;%PATH%

rm -f tlua_lib.a

find ./vendor/Mars_B91_Model_B_V1.0/telink_lua_lib/lua -name "*.o" -type f -print -exec riscv32-elf-ar -r tlua_lib.a  {} ;
find ./vendor/Mars_B91_Model_B_V1.0/telink_lua_lib/ymodem -name "*.o" -type f -print -exec riscv32-elf-ar -r tlua_lib.a  {} ;
find ./vendor/Mars_B91_Model_B_V1.0/telink_lua_lib/ff15 -name "*.o" -type f -print -exec riscv32-elf-ar -r tlua_lib.a  {} ;

