# hook_alloc

`hook_alloc` 是一个基于 PRELOAD 内存钩子库，它提供了一个模板示例，可以基于此模板进行定制开发，实现特定的内存监控或统计功能。

## 目前的功能

- **内存分配拦截**：通过挂钩 `malloc` 和 `calloc` 函数，拦截所有相关的内存分配请求。
- **调用栈追踪**：为每次内存分配记录完整的调用栈，便于后续分析内存分配来源。
- **日志记录**：将内存分配和调用栈信息输出到日志文件，支持后期分析。

## 使用指南

1. **克隆仓库**
   
   ```bash
   git clone https://github.com/DoubleTongTong/hook_alloc.git
   cd hook_alloc
   ```

2. **编译**
   
   ```bash
   make
   ```

3. **使用**
   
   ```bash
   LD_PRELOAD=/path/to/libhookalloc.so ./your_application
   ```