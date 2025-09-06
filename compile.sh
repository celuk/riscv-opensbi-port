set -e

make distclean;
dtc -I dts -O dtb -o ./platform/template/custom.dtb ./platform/template/custom.dts;
make ARCH=riscv PLATFORM_RISCV_XLEN=32 CROSS_COMPILE=/media/shc/0EDEBC4906059163/tools/riscv-toolchain-linux/_install/bin/riscv32-unknown-linux-gnu- PLATFORM_RISCV_ISA=rv32imac_zicsr_zifencei PLATFORM=template FW_TEXT_START=0x80000000 FW_FDT_PATH=./platform/template/custom.dtb FW_FDT_PADDING=4 FW_PAYLOAD=y FW_PAYLOAD_OFFSET=0x00400000 FW_PAYLOAD_PATH=../riscv-linux-ue/arch/riscv/boot/Image;
python3 /home/shc/projects/cva-soc/tools/bin2hex.py ./build/platform/template/firmware/fw_payload.bin > ./build/platform/template/firmware/fw_dynamic.hex;
/media/shc/0EDEBC4906059163/tools/riscv-toolchain-linux/_install/bin/riscv32-unknown-linux-gnu-objdump -m riscv:rv32 -d -M numeric,no-aliases ./build/platform/template/firmware/fw_payload.elf > ./build/platform/template/firmware/fw_dynamic.dump;
/media/shc/0EDEBC4906059163/tools/riscv-toolchain-linux/_install/bin/riscv32-unknown-linux-gnu-objcopy -O verilog ./build/platform/template/firmware/fw_payload.elf ./build/platform/template/firmware/fw_dynamic.vmem;
python3 /home/shc/projects/cva-soc/tools/vmem_to_ddr3_init.py -i ./build/platform/template/firmware/fw_dynamic.vmem -o ./build/platform/template/firmware/fw_dynamic_mem_init.txt;
