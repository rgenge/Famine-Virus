#!/bin/sh

ISO_URL="https://cloud.debian.org/images/cloud/bookworm/latest/debian-12-nocloud-amd64.qcow2"
ISO_PATH="./deb12.qcow2"

[ -f "$ISO_PATH" ] || wget "$ISO_URL" -O "$ISO_PATH"

qemu-system-x86_64 \
	-k pt-br -nographic \
	-drive file="$ISO_PATH",format=qcow2 \
	-smp 3 \
	-m 4G

exit;

#apt update
#apt upgrade
#apt install ssh git
# instalar chave pública para clonar o projeto
# dentro da VM tem mais coisas para fazer

#tar czf "$APKOVL_PATH" setup.sh
#python3 -m http.server &

qemu-system-x86_64 \
	-drive file="$ISO_PATH" \
	-k pt-br -nographic \
	-drive file=alpine-virt-3.20.2-x86_64.iso \
	-smp 3,maxcpus=3 \
	-m 2G

qemu-system-x86_64 \
	-k pt-br -nographic \
	-kernel "$KERNEL_PATH" \
	-append "apkovl=http://$APKOVL_PATH" \
	-netdev user,id=net0,hostfwd=tcp::2222-:22 \
	-device virtio-net-pci,netdev=net0 \
	-m 2G
