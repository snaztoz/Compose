#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "value.h"



/* The raw value read from the script file will be
 * like:
 *   ""style.css"" (yes, quotes inside quotes)
 * 
 * so the purpose of this function is to turn it
 * into:
 *   "style.css" (normal string)
 */
static void extract_value(Token *tok)
{
	/* replace the closing quote with NULL value */
	tok->buff[(tok->buffpos)-1] = '\0';

	/* skip the opening quote */
	memcpy(tok->token, ((tok->buff)+1), tok->_SIZE);
}

/* read all char inside surrounding quotes and then
 * use extract_value() to copy the value to token without
 * the opening and closing quote
 */
void get_value(Files *file, Token *tok)
{
	char c;

	while (c = fgetc(file->script)) {
/* resizing feature
		if (tok->buffpos + 5 == tok->_SIZE) {
			tok->_SIZE *= 2;
			tok->buff = (char *)realloc(tok->buff, tok->_SIZE);
			tok->token = (char *)realloc(tok->token, tok->_SIZE);	
		}
*/
		if (c == EOF) {
			puts("Error: No closing tag found");
			exit(1);
		}

		/* Has not encountered any quote char but already
		   receiving another kind of char */
		else if (is_buff_empty(tok->buff)
			  && !is_quote(c) ) 
		{
			puts("Error: No opening of value statement");
			exit(1);
		}

		/* opening quote */
		else if (is_buff_empty(tok->buff)
			  && is_quote(c))
		{
			insert_to_buff(c, tok);
		}

		/* probably the end of value */
		else if (is_quote(c)
			  && !is_buff_empty(tok->buff)) 
		{
			insert_to_buff(c, tok);
			/* if the quote is escaped, then it is not a
			   closing tag (a.k.a. part of the value) */
			if (is_escaped_char(tok)) {
				handle_escaped_char(c, tok);
				continue;
			}

			/* make sure the opening and closing is the same
			   kind of quote */
			else if (c == tok->buff[0]) {
				extract_value(tok);
				reset_buffpos_of(tok);
				return;
			}

			/* different kind of quote between opening and the
			   encountered one */
			else {
				continue;
			}
		}
		
		/* ordinary chars */
		else {
			insert_to_buff(c, tok);
		}
	}
}



