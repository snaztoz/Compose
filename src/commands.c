#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "commands.h"
#include "value.h"
#include "utils.h"



/* Processing the COMMAND token. Return the code for a command
 * from the available lists if success, or will raise error and
 * exit immediately.
 */
Command get_command(Files *file, Token *tok)
{
        char c;
	Command command;

	int _IGNORE_BLANKSPACE = TRUE;

	while (c = fgetc(file->script))
	{
		if (c == EOF) {
			puts("Error: No closing tag found");
			exit(1);
		}

		else if (_IGNORE_BLANKSPACE
			&& is_blankspace(c)) {
			continue;
		}

		/* not expecting any opening or closing tag.
		   Raise error. */
		else if ( (c == '{')
			 || (c == '}')
			 || (c == '%') ) 
		{
			printf("Error: Found an unexpected '%c' symbol\n", c);
			exit(1);	
		}

		/* at this point, the buff is already filled with some char 
		 * so that it will treat blankspace or quote as the end of
		 * some command
		 */
		else if (is_blankspace(c) || is_quote(c)) 
		{
			memcpy(tok->token, tok->buff, tok->_SIZE);
			reset_buffpos_of(tok);
				
			command = is_valid_command(tok->token);
			/* invalid command */
			if (command == -1) {
				printf("Error: Unknown '%s' command", tok->token);
			       	exit(1);	
			}
			
			/* insert to buff if the received char is quote */			
			if (is_quote(c)) {
				insert_to_buff(c, tok);
			}
			
			return command;
		}

		/* any other char than blankspace or quotes */
		else { 
			_IGNORE_BLANKSPACE = FALSE;
			insert_to_buff(c, tok); 
		}
	}
}

void COMMAND_compose(Files *file, Token *tok)
{
	/* raise error if compose command is already called before */
	if (file->out != NULL) {
		puts("Error: Can't have multiple output file");
		exit(1);
	}
	
	/* delete previous file */
	remove(tok->token);

	file->out = fopen(tok->token, "a");
	
	if (file->out == NULL) {
		puts("Error: Can't create output file");
		exit(1);
	}
}

void COMMAND_insertfile(Files *file, Token *tok)
{
	if (file->out == NULL) {
		puts("Error: Output file is not defined");
		exit(1);
	}

	char line[256];
	file->in = fopen(tok->token, "r");

	if (file->in == NULL) {
		printf("Error: Can't open %s file\n", tok->token);
		exit(1);
	}

	while (fgets(line, 256, file->in)) {
		fprintf(file->out, "%s", line);
	}

	fclose(file->in);
}




