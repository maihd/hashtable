#pragma once

#include <stdint.h>

typedef enum Type
{
    Type_I8,
    Type_U8,
    Type_I16,
    Type_U16,
    Type_I32,
    Type_U32,
    Type_I64,
    Type_U64,
    Type_Float,
    Type_Double,
    Type_String,
    Type_Bundle,
    Type_Custom,
} Type;

typedef struct Variant
{
    Type type;
    union
    {
        int8_t          asI8;
        uint8_t         asU8;
        int16_t         asI16;
        uint16_t        asU16;
        int32_t         asI32;
        uint32_t        asU32;
        int64_t         asI64;
        uint64_t        asU64;
        float           asFloat;
        double          asDouble;
        const char*     asString;
        struct Bundle*  asBundle;
        void*           asCustom;
    };
} Variant;

typedef struct BundleNode
{
    const char* key;
    Variant     value;

    struct BundleNode* next;
} BundleNode;

typedef struct Bundle
{
    int size;
    int count;

    BundleNode* nodes[1];
} Bundle;

Bundle*         newBundle(int size);
void            freeBundle(Bundle* bundle);
void            removeBundleNode(Bundle* bundle, const char* key);

int8_t          getI8(const Bundle* bundle, const char* key);
uint8_t         getU8(const Bundle* bundle, const char* key);
int16_t         getI16(const Bundle* bundle, const char* key);
uint16_t        getU16(const Bundle* bundle, const char* key);
int32_t         getI32(const Bundle* bundle, const char* key);
uint32_t        getU32(const Bundle* bundle, const char* key);
int64_t         getI64(const Bundle* bundle, const char* key);
uint64_t        getU64(const Bundle* bundle, const char* key);
float           getFloat(const Bundle* bundle, const char* key);
double          getDouble(const Bundle* bundle, const char* key);
const char*     getString(const Bundle* bundle, const char* key);
Bundle*         getBundle(const Bundle* bundle, const char* key);
void*           getCustom(const Bundle* bundle, const char* key);

void            setI8(Bundle* bundle, const char* key, int8_t value);
void            setU8(Bundle* bundle, const char* key, uint8_t value);
void            setI16(Bundle* bundle, const char* key, int16_t value);
void            setU16(Bundle* bundle, const char* key, uint16_t value);
void            setI32(Bundle* bundle, const char* key, int32_t value);
void            setU32(Bundle* bundle, const char* key, uint32_t value);
void            setI64(Bundle* bundle, const char* key, int64_t value);
void            setU64(Bundle* bundle, const char* key, uint64_t value);
void            setFloat(Bundle* bundle, const char* key, float value);
void            setDouble(Bundle* bundle, const char* key, double value);
void            setString(Bundle* bundle, const char* key, const char* value);
void            setBundle(Bundle* bundle, const char* key, Bundle* value);
void            setCustom(Bundle* bundle, const char* key, void* value);
