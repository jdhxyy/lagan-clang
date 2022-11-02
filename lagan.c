// Copyright 2020-2021 The jdh99 Authors. All rights reserved.
// 实时流日志库.可以用于串口等流日志打印
// Authors: jdh99 <jdh821@163.com>
// lagan取名来自于宜家的水龙头"拉根"

#include "lagan.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static bool gIsPause = false;
static LaganLevel gFilterLevel = LAGAN_FILTER_LEVEL_DEAFULT;
static char gLevelCh[] = {'O', 'D', 'I', 'W', 'E'};

static LaganPrintFunc gOutput = NULL;
static LaganGetTimeFunc gGetTime = NULL;
static LaganGetLocalTimeFunc gGetLocalTime = NULL;
static bool gIsLoad = false;

// LaganLoad 模块载入
// getTime是读取北京时间,getLocalTime是读取本地时间,进度是us
// 如果不需要使用哪个时间,就将其设置为NULL.如果两个都有效,则使用的是北京时间
void LaganLoad(LaganPrintFunc print, LaganGetTimeFunc getTime, LaganGetLocalTimeFunc getLocalTime) {
    if (gIsLoad) {
        return;
    }

    gOutput = print;
    gGetTime = getTime;
    if (gGetTime == NULL) {
        gGetLocalTime = getLocalTime;   
    }
    gIsLoad = true;
}

// LaganSetFilterLevel 设置过滤日志等级
void LaganSetFilterLevel(LaganLevel level) {
    gFilterLevel = level;
}

// PaxGetFilterLevel 显示过滤日志等级
LaganLevel LaganGetFilterLevel(void) {
    return gFilterLevel;
}

// LaganPrint 日志打印
void LaganPrint(char* tag, LaganLevel level, char *format, ...) {
    if (gIsLoad == false || gIsPause) {
        return;
    }
    if (gFilterLevel == LAGAN_LEVEL_OFF || level < gFilterLevel || tag == NULL || 
        strlen(tag) > LAGAN_MODULE_NAME_LEN_MAX - 1) {
        return;
    }

    char buf[LAGAN_RECORD_MAX_SIZE_DEFAULT] = {0};

    // 前缀
    if (gGetTime != NULL) {
        LaganTime time = gGetTime();
        sprintf(buf, "%02d/%02d/%02d %02d:%02d:%02d.%06d %c/%s ", time.Year, time.Month, time.Day, time.Hour, time.Minute,
            time.Second, time.Us, gLevelCh[level], tag);
    } else {
        uint64_t us = gGetLocalTime();
        int second = (int)(us / 1000000);
        us = us % 1000000;
        int ms = (int)(us / 1000);
        us = us % 1000;
        sprintf(buf, "%06d/%03d/%03d %c/%s ", second, ms, (int)us, gLevelCh[level], tag);
    }
    gOutput((uint8_t*)buf, (int)strlen(buf));

    // 正文
    va_list args;
	va_start(args, format);

    int len = vsnprintf(buf, LAGAN_RECORD_MAX_SIZE_DEFAULT - 1, format, args);
    if (len > LAGAN_RECORD_MAX_SIZE_DEFAULT || len < 0) {
        len = LAGAN_RECORD_MAX_SIZE_DEFAULT;
    }
    gOutput((uint8_t*)buf, (int)strlen(buf));

    va_end(args);

    // 后缀
    gOutput((uint8_t*)"\n", 1);
}

// LaganPrintHex 打印字节流
void LaganPrintHex(char* tag, LaganLevel level, uint8_t* bytes, int size) {
    if (gIsLoad == false || gIsPause) {
        return;
    }
    if (gFilterLevel == LAGAN_LEVEL_OFF || level < gFilterLevel) {
        return;
    }

    LaganPrint(tag, level, "");

    char buf[LAGAN_RECORD_MAX_SIZE_DEFAULT] = {0};

    char temp[16] = {0};
    int len = 0;
    for (int i = 0; i < size; i++) {
        if (len + 3 > LAGAN_RECORD_MAX_SIZE_DEFAULT - 1) {
            gOutput((uint8_t*)buf, len);
            buf[0] = '\0';
            len = 0;
        }

        if (i >= size - 1) {
            sprintf(temp, "%02x\n", bytes[i]);
        } else if (i != 0 && (i + 1) % 16 == 0) {
            sprintf(temp, "%02x\n", bytes[i]);
        } else {
            sprintf(temp, "%02x ", bytes[i]);
        }
        strcat(buf, temp);
        len += 3;
    }

    if (len > 0) {
        gOutput((uint8_t*)buf, len);
    }
}

// PaxPause 暂停日志打印
void LaganPause(void) {
    gIsPause = true;
}

// PaxResume 恢复日志打印
void LaganResume(void) {
    gIsPause = false;
}

// PaxIsPause 是否暂停
bool LaganIsPause(void) {
    return gIsPause;
}

// LaganRaw 原始打印.无时间戳和模块信息
void LaganRaw(char *format, ...) {
    char buf[LAGAN_RECORD_MAX_SIZE_DEFAULT] = {0};

    // 正文
    va_list args;
    va_start(args, format);

    int len = vsnprintf(buf, LAGAN_RECORD_MAX_SIZE_DEFAULT - 1, format, args);
    if (len > LAGAN_RECORD_MAX_SIZE_DEFAULT || len < 0) {
        len = LAGAN_RECORD_MAX_SIZE_DEFAULT;
    }
    gOutput((uint8_t*)buf, (int)strlen(buf));

    va_end(args);
}
