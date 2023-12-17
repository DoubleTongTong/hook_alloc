#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void* (*original_malloc)(size_t) = NULL;
void* (*original_calloc)(size_t, size_t) = NULL;
__thread int in_hook = 0;  // 线程局部存储
__thread int in_dlsym = 0; // 标志dlsym调用中
const char* log_file_path = "/home/tim/logfile.log";  // 日志文件路径

int log_fd = -1;

#define TEMP_BLOCK_SIZE 10240  // 临时缓冲区大小
static char temp_malloc_dlsym_block[TEMP_BLOCK_SIZE];  // 临时缓冲区
static char temp_calloc_dlsym_block[TEMP_BLOCK_SIZE];  // 临时缓冲区

void open_log_file() {
    if (log_fd == -1) {
        log_fd = open(log_file_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
}

void write_to_log(const char* str) {
    if (log_fd != -1) {
        write(log_fd, str, strlen(str));
        //fsync(log_fd); // 确保日志写入到磁盘
    }
}

void print_backtrace() {
    if (in_dlsym) {
        return; // 在dlsym调用中避免使用backtrace
    }

    void *buffer[100];
    int nptrs = backtrace(buffer, 100);
    char **strings = backtrace_symbols(buffer, nptrs);
    if (strings != NULL) {
        for (int i = 0; i < nptrs; i++) {
            write_to_log(strings[i]);
            write_to_log("\n");
        }
        free(strings);
    }
}

void* malloc(size_t size) {
    if (original_malloc == NULL) {
        if (!in_dlsym) {
            in_dlsym = 1;
            original_malloc = dlsym(RTLD_NEXT, "malloc");
            in_dlsym = 0;
        } else {
            return temp_malloc_dlsym_block; // 在dlsym调用中提供临时缓冲区
        }
    }

    if (!in_hook) {
        in_hook = 1;
        open_log_file();
        print_backtrace();
        in_hook = 0;
    }

    void *result = original_malloc(size);

    if (!in_hook) {
        in_hook = 1;
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Malloc returned: %p\n", result);
        write_to_log(buffer);
        in_hook = 0;
    }

    return result;
}

void* calloc(size_t nmemb, size_t size) {
    if (original_calloc == NULL) {
        if (!in_dlsym) {
            in_dlsym = 1;
            original_calloc = dlsym(RTLD_NEXT, "calloc");
            in_dlsym = 0;
        } else {
            return temp_calloc_dlsym_block; // 在dlsym调用中提供临时缓冲区
        }
    }

    if (!in_hook) {
        in_hook = 1;
        open_log_file();
        print_backtrace();
        in_hook = 0;
    }

    void *result = original_calloc(nmemb, size);

    if (!in_hook) {
        in_hook = 1;
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Calloc returned: %p\n", result);
        write_to_log(buffer);
        in_hook = 0;
    }

    return result;
}

