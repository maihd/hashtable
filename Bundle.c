#include "Bundle.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

static uint64_t hashString(const char* key)
{
    uint64_t result = 0;

    int c;
    while ((c = *key))
    {
        result ^= (result  | (uint64_t)c);
        result ^= (result << (uint64_t)32u);
        result ^= (result >> (uint64_t)32u);
        result ^= (result >> (uint64_t)32u);
    }

    return result;
}

static void freeVariantData(Variant* variant)
{
    switch (variant->type)
    {
        case Type_String:
            free(variant->asString);
            break;

        case Type_Bundle:
            freeBundle(variant->asBundle);
            break;

        default:
            break;
    }
}

Bundle* newBundle(int size)
{
    assert(size > 0);
    
    Bundle* bundle = malloc(sizeof(Bundle) + (size - 1) * sizeof(BundleNode*));
    bundle->size = size;
    bundle->count = 0;

    for (int i = 0; i < size; i++)
    {
        bundle->nodes[i] = NULL;
    }

    return bundle;
}

void freeBundle(Bundle* bundle)
{
    if (bundle)
    {
        for (int i = 0, n = bundle->size; i < n; i++)
        {
            BundleNode* node = bundle->nodes[i];
            if (node)
            {
                freeVariantData(&node->value);
                free(node);
            }
        }
    }
}

void removeBundleNode(Bundle* bundle, const char* key)
{
    int index = (int)(hashString(key) % (uint64_t)bundle->size);
    BundleNode* currNode = bundle->nodes[index];
    BundleNode* prevNode = NULL;
    while (currNode)
    {
        if (strcmp(currNode->key, key) == 0)
        {
            if (prevNode)
            {
                prevNode->next = currNode->next;
            }
            else
            {
                bundle->nodes[index] = 0;
            }
            
            freeVariantData(&currNode->value);
            free(currNode->key);
            free(currNode);
            return;
        }

        prevNode = currNode;
        currNode = currNode->next;
    }
}

static BundleNode* searchBunldeNode(Bundle* bundle, const char* key, int createNew)
{
    int index = (int)(hashString(key) % (uint64_t)bundle->size);
    BundleNode* node = bundle->nodes[index];
    while (node)
    {
        if (strcmp(node->key, key) == 0)
        {
            return node;
        }

        node = node->next;
    }

    if (createNew)
    {
        node = malloc(sizeof(BundleNode));
        node->key = strdup(key);
        node->value = (Variant){ 0 };
        return node;
    }
    
    return NULL;
}

int8_t getI8(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_I8 ? node->value.asI8 : 0;
}

uint8_t getU8(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_U8 ? node->value.asU8 : 0;
}

int16_t getI16(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_I16 ? node->value.asI16 : 0;
}

uint16_t getU8(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_U16 ? node->value.asU16 : 0;
}

int32_t getI32(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_I32 ? node->value.asI32 : 0;
}

uint8_t getU32(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_U32 ? node->value.asU32 : 0;
}

int64_t getI64(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_I64 ? node->value.asI64 : 0;
}

uint64_t getU64(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_U64 ? node->value.asU64 : 0;
}

float getFloat(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_Float ? node->value.asFloat : 0;
}

double getDouble(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_Double ? node->value.asDouble : 0;
}

const char* getString(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_String ? node->value.asString : "";
}

Bundle* getBundle(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_Bundle ? node->value.asBundle : NULL;
}

void* getCustom(const Bundle* bundle, const char* key)
{
    BundleNode* node = searchBunldeNode(bundle, key, 0);
    return node && node->value.type == Type_Custom ? node->value.asCustom : NULL;
}

void setI8(Bundle* bundle, const char* key, int8_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asI8 = value;
    }
}

void setU8(Bundle* bundle, const char* key, uint8_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asU8 = value;
    }
}

void setI16(Bundle* bundle, const char* key, int16_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asI16 = value;
    }
}

void setU16(Bundle* bundle, const char* key, uint16_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asU16 = value;
    }
}

void setI32(Bundle* bundle, const char* key, int32_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asI32 = value;
    }
}

void setU32(Bundle* bundle, const char* key, uint32_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asU32 = value;
    }
}

void setI64(Bundle* bundle, const char* key, int64_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asI64 = value;
    }
}

void setU64(Bundle* bundle, const char* key, uint64_t value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asU64 = value;
    }
}

void setFloat(Bundle* bundle, const char* key, float value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asFloat = value;
    }
}

void setDouble(Bundle* bundle, const char* key, double value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asDouble = value;
    }
}

void setString(Bundle* bundle, const char* key, const char* value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asString = strdup(value);
    }
}

void setBundle(Bundle* bundle, const char* key, Bundle* value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asBundle = value;
    }
}

void setCustom(Bundle* bundle, const char* key, void* value)
{
    BundleNode* node = searchBunldeNode(bundle, key, 1);
    if (node)
    {
        freeVariantData(&node->value);
        node->value.asCustom = value;
    }
}
