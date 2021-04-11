// Copyright 2020-2021 The jdh99 Authors. All rights reserved.
// ʵʱ����־��.�������ڴ��ڵ�����־��ӡ
// Authors: jdh99 <jdh821@163.com>
// laganȡ���������˼ҵ�ˮ��ͷ"����"

#ifndef LAGAN_H
#define LAGAN_H

#include <stdint.h>
#include <stdbool.h>

// ��־����
typedef enum {
    LAGAN_LEVEL_OFF = 0,
    LAGAN_LEVEL_DEBUG,
    LAGAN_LEVEL_INFO,
    LAGAN_LEVEL_WARN,
    LAGAN_LEVEL_ERROR
} LaganLevel;

// ģ������󳤶�
#define LAGAN_MODULE_NAME_LEN_MAX 100

// Ĭ�ϵ���־��¼����ֽ���
#define LAGAN_RECORD_MAX_SIZE_DEFAULT 512

// Ĭ�Ϲ��˼���
#define LAGAN_FILTER_LEVEL_DEAFULT LAGAN_LEVEL_DEBUG

#define LD(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LI(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_INFO, format, ##__VA_ARGS__)
#define LW(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_WARN, format, ##__VA_ARGS__)
#define LE(tag, format, ...) LaganPrint(tag, LAGAN_LEVEL_ERROR, format, ##__VA_ARGS__)

#pragma pack(1)

// ��ȡʱ��.24Сʱ��
typedef struct {
    // ����4λ��,��ʽ��2020
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int Us;
} LaganTime;

#pragma pack()

// LaganPrintFunc ��ӡ��������
typedef void (*LaganPrintFunc)(uint8_t* bytes, int size);

// LaganTime ��ȡʱ��
typedef LaganTime (*LaganGetTimeFunc)(void);

// LaganLoad ģ������
void LaganLoad(LaganPrintFunc print, LaganGetTimeFunc getTime);

// LaganSetFilterLevel ���ù�����־�ȼ�
void LaganSetFilterLevel(LaganLevel level);

// PaxGetFilterLevel ��ʾ������־�ȼ�
LaganLevel LaganGetFilterLevel(void);

// LaganPrint ��־��ӡ
void LaganPrint(char* tag, LaganLevel level, char *format, ...);

// LaganPrintHex ��ӡ�ֽ���
void LaganPrintHex(char* tag, LaganLevel level, uint8_t* bytes, int size);

// PaxPause ��ͣ��־��ӡ
void LaganPause(void);

// PaxResume �ָ���־��ӡ
void LaganResume(void);

// PaxIsPause �Ƿ���ͣ
bool LaganIsPause(void);

#endif
