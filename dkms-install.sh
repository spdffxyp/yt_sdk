#!/bin/bash

if [[ $EUID -ne 0 ]]; then
    echo "You must run this with superuser privileges.  Try \"sudo ./dkms-install.sh\"" 2>&1
    exit 1
fi

echo "About to run dkms install steps..."

# 确保脚本切换到当前所在目录（即 /yt_sdk 根目录）
cd "$(dirname "$0")"

echo "$(pwd)"

# 核心修改：cd 进入子目录进行清理，然后再 cd 回来，确保 PWD 变量正确
# cd make && make clean && cd ..

echo "$(pwd)"

DRV_DIR="$(pwd)"
DRV_NAME=yt9125
DRV_VERSION=1.1.008
KERNEL_VERSION="${KERNEL_VERSION:-$(uname -r)}"

# 完整拷贝整个 /yt_sdk 目录（包含子目录和位于根部的 dkms.conf）
cp -r ${DRV_DIR} /usr/src/${DRV_NAME}-${DRV_VERSION}

dkms add -m ${DRV_NAME} -v ${DRV_VERSION} -k ${KERNEL_VERSION}
dkms build -m ${DRV_NAME} -v ${DRV_VERSION} -k ${KERNEL_VERSION}
dkms install -m ${DRV_NAME} -v ${DRV_VERSION} -k ${KERNEL_VERSION} --force
RESULT=$?

echo "Finished running dkms install steps."

exit $RESULT