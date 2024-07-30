export DISK_NAME=disk.dmg
# 每个块都是 512 字节
# 写boot区，定位到磁盘开头，写1个块，由系统自动加载
dd if=boot.bin of=$DISK_NAME bs=512 conv=notrunc count=1

# 写kernel区，定位到磁盘第2个块，加载全部kernel，这部分需要手动加载  大小要看kernel.bin不要看elf文件
dd if=kernel.bin of=$DISK_NAME bs=512 conv=notrunc seek=1

# 写入图片资源
dd if=img.data of=$DISK_NAME bs=512 conv=notrunc seek=262144