#ifndef COMMANDS_H
#define COMMANDS_H

#include "utils.h"


typedef enum {
	COMPOSE,
	INSERTFILE,
} Command;

Command get_command(Files *, Token *);

void COMMAND_compose(Files *, Token *);
void COMMAND_insertfile(Files *, Token *);

#endif
