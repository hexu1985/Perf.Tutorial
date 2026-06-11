`perf` 是 Linux 内核自带的强大性能分析工具，可以对软件和硬件事件进行深入分析，帮你精准定位 CPU、内存等性能瓶颈。

以下是 `perf` 的核心用法：

### 💻 安装与基础

*   **安装**：多数发行版未预装，请使用包管理器安装（如 Ubuntu: `sudo apt install linux-tools-common`, CentOS: `sudo yum install perf`）。
*   **事件查看**：使用 `perf list` 查看当前系统支持的所有性能事件（硬件、软件、Tracepoint）。

### 📈 常用场景与命令

`perf` 提供了多个核心子命令，覆盖了从宏观统计到微观分析的各个环节。

**1. 宏观统计 (`perf stat`)**
快速评估整体性能，如CPU效率、Cache命中率等。
*   **默认运行程序**：`sudo perf stat [your_program]`。
*   **统计特定事件**：`sudo perf stat -e cache-misses,page-faults [your_program]`。
*   **全系统监控**：`sudo perf stat -a` 查看整个系统的性能计数器。
*   **输出示例解读**（类似）：
    *   `context-switches`：上下文切换次数。
    *   `cpu-migrations`：CPU迁移次数。
    *   `page-faults`：缺页次数。
    *   `cycles`：消耗的CPU周期数。
    *   `instructions`：执行的指令数。
    *   `branches` / `branch-misses`：分支指令数 / 分支预测错误数。

**2. 实时监控 (`perf top`)**
类似 `top`，实时显示当前系统/进程的“热点”函数（CPU消耗大户）。
*   **全系统监控**：`sudo perf top`。
*   **监控特定进程**：`sudo perf top -p [PID]`。
*   **监控指定事件**：`sudo perf top -e cache-misses` 查看谁导致了大量缓存未命中。
*   **输出解释**：
    *   `Overhead`：该函数占用CPU时间的百分比。
    *   `Shared Object`：函数所属的库或程序。
    *   `Symbol`：函数名。`[k]` 表示内核态，`[.]` 表示用户态。

**3. 采样分析 (`perf record` + `perf report`)**
采集指定时间的数据并保存到文件，用于后续深度分析。
*   **执行并录制**：`sudo perf record -g [your_program]`。
*   **录制运行中的进程**：`sudo perf record -p [PID] -g -- sleep [duration]`。
*   **生成并查看报告**：`sudo perf report`
*   **常用参数**：
    *   `-e`：指定事件，如 `-e cpu-clock`。
    *   `-a`：采集所有CPU数据。
    *   `-C`：采集指定CPU的数据。
    *   `-F [freq]`：设置采样频率（Hz）。
    *   `-c [count]`：设置采样周期（每N个事件采样一次）。
    *   `-g`：记录函数调用关系（调用图）。

**4. 高级分析**
*   **系统调用追踪**：`sudo perf trace -e syscalls [your_program]` 追踪并定位I/O瓶颈。
*   **内存访问分析**：`sudo perf mem record -a sleep 10` 和 `sudo perf mem report` 分析内存访问延迟。
*   **调度器分析**：`sudo perf sched record -a sleep 10` 和 `sudo perf sched latency --sort max` 分析线程等待和调度延迟。
*   **代码注释**：`sudo perf annotate [symbol_name]` 查看函数性能热点的汇编/源码行。
*   **性能对比**：`sudo perf diff perf.data.old perf.data` 对比优化前后的性能差异。
*   **自定义脚本**：`perf script -s [script.py]` 基于采集数据编写脚本进行定制化分析。

### 📌 常用参数速查

*   `-e, --event`：指定要监控的事件，可用 `perf list` 查看。
*   `-p, --pid`：只分析指定进程ID。
*   `-t, --tid`：只分析指定线程ID。
*   `-a, --all-cpus`：采集所有CPU上的数据。
*   `-C, --cpu`：只采集指定CPU上的数据。
*   `-g`：记录函数调用关系（调用图）。
*   `-F`：设置采样频率（Hz）。
*   `-c`：设置采样周期（每N个事件采样一次）。

### 💡 实用技巧与注意事项

*   **生成火焰图**：结合 `perf script` 和 `FlameGraph` 工具生成可视化火焰图，直观展示函数调用和CPU消耗。
*   **用户态权限**：普通用户使用 `perf` 可能受限，可临时通过命令 `sudo sysctl -w kernel.perf_event_paranoid=-1` 修改内核安全设置。
*   **内核符号支持**：若要解析内核函数名，建议安装 `kernel-debuginfo` 或 `linux-image-$(uname -r)-dbg` 等对应的内核调试符号包。

### 💡 FAQ

- 执行`perf record -F 99 -g ./sched 5000 100`  报如下错误:
```
WARNING: perf.back not found for kernel 5.4.0-121

  You may need to install the following packages for this specific kernel:
    linux-tools-5.4.0-121-generic
    linux-cloud-tools-5.4.0-121-generic

  You may also want to install one of the following packages to keep up to date:
    linux-tools-generic
    linux-cloud-tools-generic
```

需要安装`linux-tools-generic`
```
$ sudo apt install linux-tools-5.4.0-121-generic    # 根据ubuntu不同版本, 可能tools的版本也需要调整
$ export PATH=/usr/lib/linux-tools/5.4.0-120-generic:$PATH
$ perf --version
```

- 执行`perf record -F 99 -g ./sched 5000 100`  报如下错误:
```
perf_event_open(..., PERF_FLAG_FD_CLOEXEC) failed with unexpected error 13 (权限不够)
perf_event_open(..., 0) failed unexpectedly with error 13 (权限不够)
Error:
You may not have permission to collect stats.

Consider tweaking /proc/sys/kernel/perf_event_paranoid,
which controls use of the performance events system by
unprivileged users (without CAP_SYS_ADMIN).

The current value is 3:

  -1: Allow use of (almost) all events by all users
      Ignore mlock limit after perf_event_mlock_kb without CAP_IPC_LOCK
>= 0: Disallow ftrace function tracepoint by users without CAP_SYS_ADMIN
      Disallow raw tracepoint access by users without CAP_SYS_ADMIN
>= 1: Disallow CPU event access by users without CAP_SYS_ADMIN
>= 2: Disallow kernel profiling by users without CAP_SYS_ADMIN

To make this setting permanent, edit /etc/sysctl.conf too, e.g.:

        kernel.perf_event_paranoid = -1
```
执行如下命令:
```
$ sudo sh -c " echo -1 > /proc/sys/kernel/perf_event_paranoid"
```

- 执行`perf record -F 99 -g ./sched 5000 100`  报如下错误:
```
WARNING: Kernel address maps (/proc/{kallsyms,modules}) are restricted,
check /proc/sys/kernel/kptr_restrict and /proc/sys/kernel/perf_event_paranoid.

Samples in kernel functions may not be resolved if a suitable vmlinux
file is not found in the buildid cache or in the vmlinux path.

Samples in kernel modules won't be resolved at all.

If some relocation was applied (e.g. kexec) symbols may be misresolved
even with a suitable vmlinux or kallsyms file.

Couldn't record kernel reference relocation symbol
Symbol resolution may be skewed if relocation was used (e.g. kexec).
Check /proc/kallsyms permission or run as root.
```
执行如下命令:
```
$ sudo sh -c " echo 0 > /proc/sys/kernel/kptr_restrict"
```
