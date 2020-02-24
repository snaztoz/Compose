#ifndef UTILS_H
#define UTILS_H

#define TRUE 1
#define FALSE 0


typedef struct {
	FILE *script;
	FILE *out;
        FILE *in;	
} Files;

typedef struct {
	char *buff;
	char *token;
	int buffpos;
	int _SIZE;
} Token;


/* setup the outfile */
void check_argument(int, char **);
void set_scriptfile(Files *, const char *);

/* close all opened files */
void close_files(Files *);

/* Add char c to the buff at given buffpos, and increment
 * the value of buffpos.
 */
void insert_to_buff(char, Token *);

void reset_buffpos_of(Token *);

int is_buff_empty(char buff[]);

int is_blankspace(char);

int is_opening_tag(char *);

int is_closing_tag(char *);

int is_quote(char );

int is_escaped_char(Token *);

void handle_escaped_char(char, Token *);

/* check if a command is available or no. Return the code for a command
 * if the command exists, or -1 if command is not valid.
 */
int is_valid_command(char token[]);

#endif
