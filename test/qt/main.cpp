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
