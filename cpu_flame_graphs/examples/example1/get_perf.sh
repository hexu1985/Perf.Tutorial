#!/bin/bash

# 创建存放数据和结果的目录
mkdir -p perf_result

# 以 99Hz 频率采样 CPU 事件，记录调用栈，持续30秒
sudo perf record -F 99 -g -o perf_result/example.perf.data -- ./example

# 或者对已运行的进程采样（找到PID后执行）
# sudo perf record -F 99 -g -p <PID> -- sleep 30

# 将 perf.data 转换为可折叠的调用栈格式
sudo perf script -i perf_result/example.perf.data > perf_result/out.perf

# 折叠调用栈
./stackcollapse-perf.pl perf_result/out.perf > perf_result/out.folded

# 生成 SVG 火焰图
./flamegraph.pl perf_result/out.folded > perf_result/flamegraph.svg
