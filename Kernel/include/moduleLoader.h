#ifndef _MODULELOADER_H
#define _MODULELOADER_H

#include <stdint.h>

uint64_t loadModules(void *payloadStart, void **targetModuleAddress);

#endif