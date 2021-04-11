# lagan

## 介绍
lagan是C语言日志库,取名来自于宜家的水龙头"拉根"。

本包是golang编写的lagan的C语言移植版本。


## 开源
- [github上的项目地址](https://github.com/jdhxyy/lagan-clang)
- [gitee上的项目地址](https://gitee.com/jdhxyy/lagan-clang)

## 功能
- 支持日志在终端实时打印
- 支持二进制流打印

## API
```c
// 日志级别
typedef enum {
    LAGAN_LEVEL_OFF = 0,
    LAGAN_LEVEL_DEBUG,
    LAGAN_LEVEL_INFO,
    LAGAN_LEVEL_WARN,
    LAGAN_LEVEL_ERROR
} LaganLevel;

// 模块名最大长度
#define LAGAN_MODULE_NAME_LEN_MAX 100

// 默认的日志记录最大字节数
#define LAGAN_RECORD_MAX_SIZE_DEFAULT 512

// 默认过滤级别
#define LAGAN_FILTER_LEVEL_DEAFULT LAGAN_LEVEL_DEBUG

#define LD(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LI(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_INFO, format, ##__VA_ARGS__)
#define LW(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_WARN, format, ##__VA_ARGS__)
#define LE(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_ERROR, format, ##__VA_ARGS__)

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

// LaganLoad 模块载入
void LaganLoad(LaganPrintFunc print, LaganGetTimeFunc getTime);

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
```

## 测试
```c
#include <QCoreApplication>
#include <QDateTime>

extern "C" {
    #include "lagan.h"
}

static void print(uint8_t* bytes, int size);
static LaganTime getTime(void);

static void case1(void);
static void case2(void);
static void case3(void);

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    LaganLoad(print, getTime);

    case1();
    case2();
    case3();

    return a.exec();
}

static void print(uint8_t* bytes, int size) {
    printf("%s", bytes);
}

static LaganTime getTime(void) {
    QDate date = QDateTime::currentDateTime().date();
    QTime now = QTime::currentTime();

    LaganTime time;
    time.Year = date.year();
    time.Month = date.month();
    time.Day = date.day();
    time.Hour = now.hour();
    time.Minute = now.minute();
    time.Second = now.second();
    time.Us = now.msec() * 1000;
    return time;
}

static void case1(void) {
    LD("case1", "debug test print:%d", 1);
    LI("case1", "info test print:%d", 2);
    LW("case1", "warn test print:%d", 3);
    LE("case1", "error test print:%d", 4);
}

static void case2(void) {
    LaganSetFilterLevel(LAGAN_LEVEL_WARN);
    // 过滤不会打印
    LD("case2", "debug test print:%d", 1);
    // 过滤不会打印
    LI("case2", "info test print:%d", 2);

    LW("case2", "warn test print:%d", 3);
    LE("case2", "error test print:%d", 4);
}

static void case3(void) {
    LaganSetFilterLevel(LAGAN_LEVEL_DEBUG);
    uint8_t arr[100] = {0};
    for (int i = 0; i < 100; i++) {
        arr[i] = (uint8_t)i;
    }

    LI("case3", "print hex");
    LaganPrintHex("case3", LAGAN_LEVEL_INFO, arr, 100);
}
```

输出:
```text
2021/04/12 06:37:06.583000 D/case1 debug test print:1
2021/04/12 06:37:06.588000 I/case1 info test print:2
2021/04/12 06:37:06.592000 W/case1 warn test print:3
2021/04/12 06:37:06.596000 E/case1 error test print:4
2021/04/12 06:37:06.603000 W/case2 warn test print:3
2021/04/12 06:37:06.606000 E/case2 error test print:4
2021/04/12 06:37:06.611000 I/case3 print hex
2021/04/12 06:37:06.632000 I/case3
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
60 61 62 63
```
