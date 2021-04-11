// Copyright 2020-2021 The jdh99 Authors. All rights reserved.
// ʵʱ����־��.�������ڴ��ڵ�����־��ӡ
// Authors: jdh99 <jdh821@163.com>
// laganȡ���������˼ҵ�ˮ��ͷ"����"

#include "lagan.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static bool gIsPause = false;
static LaganLevel gFilterLevel = LAGAN_FILTER_LEVEL_DEAFULT;
static char gLevelCh[] = {'O', 'D', 'I', 'W', 'E'};

static LaganPrintFunc gOutput = NULL;
static LaganGetTimeFunc gGetTime = NULL;

// LaganLoad ģ������
void LaganLoad(LaganPrintFunc print, LaganGetTimeFunc getTime) {
    gOutput = print;
    gGetTime = getTime;
}

// LaganSetFilterLevel ���ù�����־�ȼ�
void LaganSetFilterLevel(LaganLevel level) {
    gFilterLevel = level;
}

// PaxGetFilterLevel ��ʾ������־�ȼ�
LaganLevel LaganGetFilterLevel(void) {
    return gFilterLevel;
}

// LaganPrint ��־��ӡ
void LaganPrint(char* tag, LaganLevel level, char *format, ...) {
    if (tag == NULL || gIsPause) {
        return;
    }
    if (gFilterLevel == LAGAN_LEVEL_OFF || level < gFilterLevel || tag == NULL || 
        strlen(tag) > LAGAN_MODULE_NAME_LEN_MAX - 1) {
        return;
    }

    char buf[LAGAN_RECORD_MAX_SIZE_DEFAULT] = {0};

    // ǰ׺
    LaganTime time = gGetTime();
    sprintf(buf, "%02d/%02d/%02d %02d:%02d:%02d.%06d %c/%s ", time.Year, time.Month, time.Day, time.Hour, time.Minute,
        time.Second, time.Us, gLevelCh[level], tag);
    gOutput((uint8_t*)buf, (int)strlen(buf));

    // ����
    va_list args;
	va_start(args, format);

    int len = vsnprintf(buf, LAGAN_RECORD_MAX_SIZE_DEFAULT - 1, format, args);
    if (len > LAGAN_RECORD_MAX_SIZE_DEFAULT || len < 0) {
        len = LAGAN_RECORD_MAX_SIZE_DEFAULT;
    }
    gOutput((uint8_t*)buf, (int)strlen(buf));

    va_end(args);

    // ��׺
    gOutput((uint8_t*)"\n", 1);
}

// LaganPrintHex ��ӡ�ֽ���
void LaganPrintHex(char* tag, LaganLevel level, uint8_t* bytes, int size) {
    if (gIsPause) {
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

// PaxPause ��ͣ��־��ӡ
void LaganPause(void) {
    gIsPause = true;
}

// PaxResume �ָ���־��ӡ
void LaganResume(void) {
    gIsPause = false;
}

// PaxIsPause �Ƿ���ͣ
bool LaganIsPause(void) {
    return gIsPause;
}
