/* 
 *
 * Notes:
 * 1. If a tag is like this: 
 * 	{% compose 'style.css' %},
 *    It will be rendered as:
 *      {%compose'style.css'%}
 *    
 *    IT WILL REMOVE EVERY BLANKSPACES INSIDE TAG
 * 
 * 2. _SIZE value is not changeable yet
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "commands.h"
#include "value.h"







void execute(Command command, Files *file, Token *tok)
{
	switch(command)
	{
		case COMPOSE:
			COMMAND_compose(file, tok);
			break;
		case INSERTFILE:
			COMMAND_insertfile(file, tok);
			break;
	}
	
}


int look_for_opening_tag(Files *file, Token *tok)
{
	char c;
	int _IGNORE_BLANKSPACE = TRUE;

	while ( c = fgetc(file->script) ) {
                
		/* the correct exit way of the program */
		if (c == EOF) {
			return FALSE;
		}
		/* ignore blanklines before encounter any '{' sign */
		else if (_IGNORE_BLANKSPACE
			&& is_blankspace(c)) {
			continue;
		}

		/* handle the possibility of an opening tag*/
		else if (c == '{') {
			_IGNORE_BLANKSPACE = FALSE;
			insert_to_buff(c, tok);
		}
		else if (c == '%') {
			insert_to_buff(c, tok);

			memcpy(tok->token, tok->buff, tok->_SIZE);
			
			reset_buffpos_of(tok);	

			/* make sure the token is an opening tag */
			if (is_opening_tag(tok->token)) {
				return TRUE;
			} else {
				puts("Error: Can't have any expression outside tag");
				exit(1);
			}
		}

		/* raise error */
		else {
			puts("Error: Can't have any expression outside tag");
			exit(1);
		}
	}
}

void look_for_closing_tag(Files *file, Token *tok)
{
	char c;
	int _IGNORE_BLANKSPACE = TRUE;

	while (c = fgetc(file->script)) {
		if (c == EOF) {
			puts("Error: No closing tag found");
			exit(1);
		}

		else if (is_blankspace(c)
			 && _IGNORE_BLANKSPACE)
		{
			continue;
		}

		/* possibly a closing tag */
		else if (c == '%') {
			insert_to_buff(c, tok);
			_IGNORE_BLANKSPACE = FALSE;
		}
		else if (c == '}') {
			insert_to_buff(c, tok);
			memcpy(tok->token, tok->buff, tok->_SIZE);

			reset_buffpos_of(tok);

			if (is_closing_tag(tok->token)) {
				return;
			}
			else {
				puts("Error: Can't have any expression outside tag");
				exit(1);
			}
		}

		/* raise error */
		else {
			puts("Error: Can't have any expression outside tag");
			exit(1);
		}
	}
}

void render_tag(Files *file, Token *tok)
{
	Command command;

	command = get_command(file, tok);
	get_value(file, tok);
	execute(command, file, tok);
	look_for_closing_tag(file, tok);
}

int main(int argc, char **argv)
{
	check_argument(argc, argv);

	Files file;
	set_scriptfile(&file, argv[1]);
	file.out = NULL;
	file.in = NULL;

	Token tok;
	tok._SIZE = 256;
	tok.buff = (char *) malloc(tok._SIZE);
	tok.token = (char *) malloc(tok._SIZE);
	tok.buffpos = 0;

	tok.buff[tok.buffpos] = '\0';
	
	/* does it encounter (another) opening tag? */
	int _FOUND_OPENING_TAG = FALSE;	
	

	/* main loop */
	while (TRUE) {
		_FOUND_OPENING_TAG = look_for_opening_tag(&file, &tok);
		
		if (!_FOUND_OPENING_TAG) {
			puts("File successfully composed");
			break;
		}
		
		render_tag(&file, &tok);
	}

	close_files(&file);
	free(tok.buff);
	free(tok.token);

	return 0;
}
