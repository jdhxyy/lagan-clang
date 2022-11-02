// Copyright 2020-2021 The jdh99 Authors. All rights reserved.
// 实时流日志库.可以用于串口等流日志打印
// Authors: jdh99 <jdh821@163.com>
// lagan取名来自于宜家的水龙头"拉根"

#ifndef LAGAN_H
#define LAGAN_H

#include <stdint.h>
#include <stdbool.h>

// 日志级别
typedef enum {
    LAGAN_LEVEL_OFF = 0,
    LAGAN_LEVEL_DEBUG,
    LAGAN_LEVEL_INFO,
    LAGAN_LEVEL_WARN,
    LAGAN_LEVEL_ERROR
} LaganLevel;

// 模块名最大长度
#define LAGAN_MODULE_NAME_LEN_MAX 64

// 默认的日志记录最大字节数
#define LAGAN_RECORD_MAX_SIZE_DEFAULT 128

// 默认过滤级别
#define LAGAN_FILTER_LEVEL_DEAFULT LAGAN_LEVEL_DEBUG

#define LD(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LI(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_INFO, format, ##__VA_ARGS__)
#define LW(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_WARN, format, ##__VA_ARGS__)
#define LE(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_ERROR, format, ##__VA_ARGS__)
#define LR(format, ...) LaganRaw(format, ##__VA_ARGS__)

#pragma pack(1)

// 读取时间.24小时制
typedef struct {
    // 年是4位数,格式如2020
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int Us;
} LaganTime;

#pragma pack()

// LaganPrintFunc 打印数据类型
typedef void (*LaganPrintFunc)(uint8_t* bytes, int size);

// LaganTime 读取时间
typedef LaganTime (*LaganGetTimeFunc)(void);

// LaganGetLocalTimeFunc 读取本地时间.返回值是us级精度的本地时间
typedef uint64_t (*LaganGetLocalTimeFunc)(void);

// LaganLoad 模块载入
// getTime是读取北京时间,getLocalTime是读取本地时间,进度是us
// 如果不需要使用哪个时间,就将其设置为NULL.如果两个都有效,则使用的是北京时间
void LaganLoad(LaganPrintFunc print, LaganGetTimeFunc getTime, LaganGetLocalTimeFunc getLocalTime);

// LaganSetFilterLevel 设置过滤日志等级
void LaganSetFilterLevel(LaganLevel level);

// PaxGetFilterLevel 显示过滤日志等级
LaganLevel LaganGetFilterLevel(void);

// LaganPrint 日志打印
void LaganPrint(char* tag, LaganLevel level, char *format, ...);

// LaganPrintHex 打印字节流
void LaganPrintHex(char* tag, LaganLevel level, uint8_t* bytes, int size);

// PaxPause 暂停日志打印
void LaganPause(void);

// PaxResume 恢复日志打印
void LaganResume(void);

// PaxIsPause 是否暂停
bool LaganIsPause(void);

// LaganRaw 原始打印.无时间戳和模块信息
void LaganRaw(char *format, ...);

#endif
