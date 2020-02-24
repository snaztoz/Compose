#ifndef VALUE_H
#define VALUE_H

#include "utils.h"

/* Compose command is only able to be called once. 
 * If another is found, the program will raise error. 
 */
void get_value(Files *file, Token *tok);

#endif
