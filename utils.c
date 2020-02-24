#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"



/* check for argument provided in the call of
   the program */
void check_argument(int argc, char **argv)
{
	char *dot;

	/* wrong usage */
	if (argc != 2) {
		puts("Error: Usage is 'compose [filename].cpss'");
		exit(1);
	}

	/* argument is not a Compose file */
	dot = strrchr(argv[1], '.');
	if ((dot == NULL)
		|| (strcmp(dot+1, "cpss") != 0))
	{
		puts("Error: Argument is not a Compose file");
		exit(1);
	}
}

/* set when the program is invoked */
void set_scriptfile(Files *file, const char *argv)
{
	file->script = fopen(argv, "r");

	if (file->script == NULL) {
		puts("Error: Failed at opening the scriptfile");
		exit(1);
	}
}

void close_files(Files *file)
{
	if (file->script != NULL) {
		fclose(file->script);
	}
	if (file->out != NULL) {
		fclose(file->out);
	}
	if (file->in != NULL) {
		fclose(file->in);
	}
}

/* Add char c to the buff at given buffpos, and increment
 * the value of buffpos.
 */
void insert_to_buff(char c, Token *tok)
{
	tok->buff[(tok->buffpos)++] = c;
	tok->buff[tok->buffpos] = '\0';
}

void reset_buffpos_of(Token *tok)
{
	tok->buffpos = 0;
	tok->buff[tok->buffpos] = '\0';
}

int is_buff_empty(char buff[])
{
	return (buff[0] == '\0');
}

int is_blankspace(char c)
{	
	return ( (c == ' ') 
		|| (c == '\t') 
		|| (c == '\n') );
}

int is_opening_tag(char *token)
{
	return (strcmp(token, "{%") == 0)? TRUE: FALSE; 
}

int is_closing_tag(char *token)
{
	return (strcmp(token, "%}") == 0)? TRUE: FALSE;
}

int is_quote(char c)
{
	return ( (c == '\'') || (c == '\"') );
}

int is_escaped_char(Token *tok)
{
	return (tok->buff[(tok->buffpos)-2] == '\\');	
}

void handle_escaped_char(char c, Token *tok)
{
	/* PROBABLY WRONG LOGIC OR IMPLEMENTATION  */
	tok->buff[(tok->buffpos)-2] = tok->buff[(tok->buffpos)-1];
	tok->buffpos -= 1;
	
	tok->buff[tok->buffpos] = '\0';
}

/* check if a command is available or no. Return the code for a command
 * if the command exists, or -1 if command is not valid.
 */
int is_valid_command(char token[])
{
	char available_commands[][20] = {
		"compose", 
		"insertfile"
	};
	
	for (int i = 0; i < 2; i++)
	{
		if (strcmp(token, available_commands[i]) == 0)
			return i; 
	}
	// command is not recognized
	return -1;
}
