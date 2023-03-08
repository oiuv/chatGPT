#!/bin/bash

# 如果 fluffos 目录不存在，则从 gitee 克隆 fluffos 仓库
if [ ! -d "fluffos" ]; then
    git clone https://gitee.com/mudren/fluffos.git
fi

# 进入 fluffos 目录并拉取最新代码
cd fluffos && git pull

# 如果 build 目录已存在，则删除
if [ -d "build" ]; then
    rm -rf build
fi

# 创建 build 目录并进入
mkdir build && cd build

# 记录开始时间
starttime=`date +'%Y-%m-%d %H:%M:%S'`

# 编译 fluffos，使用多线程编译，开启 SQLite 数据库和默认数据库支持
cmake -DPACKAGE_DB_SQLITE=2 -DPACKAGE_DB_DEFAULT_DB=2 .. && make -j$(nproc) install

# 记录结束时间
endtime=`date +'%Y-%m-%d %H:%M:%S'`

# 计算编译时间
start_seconds=$(date --date=" $starttime" +%s);
end_seconds=$(date --date="$endtime" +%s);

# 输出编译时间
echo Start: $starttime.
echo End: $endtime.
echo "Build Time: "$((end_seconds-start_seconds))"s."
