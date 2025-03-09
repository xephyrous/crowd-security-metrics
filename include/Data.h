#ifndef DATA_H_
#define DATA_H_

typedef struct PipeData {
    uint16_t castType;
    size_t size;
    char data[512];
} PipeData;

enum CastType {
    CAST_INT,
    CAST_STRING,
    CAST_BOOL
};

#endif //DATA_H_
