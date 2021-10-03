#pragma once

#include "port.h"

typedef unsigned char      uint8_t;

#ifdef __cplusplus
extern "C" {
#endif

    PENGUINAPI_PORT const char* PENGUIN_CALL get_info();
    void PENGUINAPI load_server(char* server);
    void PENGUINAPI load_json(char* stage_index, char* hash_index);
    void PENGUINAPI load_templ(char* itemId, uint8_t* buffer, size_t size);
    PENGUINAPI_PORT const char* PENGUIN_CALL recognize(uint8_t* buffer, size_t size);

#ifdef __cplusplus
}
#endif