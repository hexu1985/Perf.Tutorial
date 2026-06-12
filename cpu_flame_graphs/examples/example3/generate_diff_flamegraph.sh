#!/bin/bash

# 设置参数
SAMPLE_FREQ=99

# 函数：采样并生成折叠文件
sample_and_collapse() {
    local binary=$1
    local output_dir=$2
    local output_prefix=$3
    
    echo "Sampling $binary..."
    
    # 运行程序并采样
    sudo perf record -F $SAMPLE_FREQ -g --call-graph dwarf \
        -o "${output_dir}/${output_prefix}.data" -- ./$binary
    
    # 生成perf script输出
    sudo perf script -i "${output_dir}/${output_prefix}.data" > "${output_dir}/${output_prefix}.perf"
    
    # 折叠调用栈
    ./stackcollapse-perf.pl "${output_dir}/${output_prefix}.perf" > "${output_dir}/${output_prefix}.folded"
    
    echo "Generated ${output_prefix}.folded"

    # 生成 SVG 火焰图
    ./flamegraph.pl "${output_dir}/${output_prefix}.folded" > "${output_dir}/${output_prefix}_flamegraph.svg"
}

# 创建存放数据和结果的目录
rm -rf perf_result
mkdir -p perf_result

# 采样优化前版本
sample_and_collapse "example_before" "perf_result" "before"

# 采样优化后版本
sample_and_collapse "example_after" "perf_result" "after"

# 生成差分火焰图
echo "Generating differential flame graph..."
./difffolded.pl perf_result/before.folded perf_result/after.folded | \
    ./flamegraph.pl --title="Performance Optimization: Before vs After" \
    --colors=diff > perf_result/diff_flamegraph.svg

echo "Differential flame graph generated: perf_result/diff_flamegraph.svg"
