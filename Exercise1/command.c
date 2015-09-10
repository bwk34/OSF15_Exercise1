#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "command.h"

#define MAX_CMD_COUNT 50
#define MAX_CMD_LEN 25


	//TODO FUNCTION COMMENT(finished)

/*
 * PURPOSE:
 * 	First check if there's error with the user input, if no error
 *	then parse out the input into words and store in an array cmd.
 *	Otherwise exit the program.
 * INPUTS:
 * 	input: A command string input by user.
 *	cmd: An array that used to stroe the parsed out input.
 * RETURN:
 *	Return true if the process of parse is success.
 *	Otherwise, return false.
 **/


bool parse_user_input (const char* input, Commands_t** cmd) {
	
	//TODO ERROR CHECK INCOMING PARAMETERS(finished)

	if(!input){
		printf("No input while parse user input.\n");
		return false;
	}
	else if(!cmd){
		printf("Command array is null.\n");
		return false;
	}

	char *string = strdup(input);
	
	*cmd = calloc (1,sizeof(Commands_t));
	(*cmd)->cmds = calloc(MAX_CMD_COUNT,sizeof(char*));

	unsigned int i = 0;
	char *token;
	token = strtok(string, " \n");
	for (; token != NULL && i < MAX_CMD_COUNT; ++i) {
		(*cmd)->cmds[i] = calloc(MAX_CMD_LEN,sizeof(char));
		if (!(*cmd)->cmds[i]) {
			perror("Allocation Error\n");
			return false;
		}	
		strncpy((*cmd)->cmds[i],token, strlen(token) + 1);
		(*cmd)->num_cmds++;
		token = strtok(NULL, " \n");
	}
	free(string);
	return true;
}

	//TODO FUNCTION COMMENT(finished)

/*
 * PURPOSE:
 *      After the command processed, this function can deallocate the memory so the cmd array can
 *	hold new commands.
 * INPUTS:
 *      cmd: An array that previously used and need to free.
 * RETURN:
 *      No return.
 **/




void destroy_commands(Commands_t** cmd) {

	//TODO ERROR CHECK INCOMING PARAMETERS(finished)

	if(!cmd){
		printf("No commands while trying to destroy commands\n");
		return;
	}
	for (int i = 0; i < (*cmd)->num_cmds; ++i) {
		free((*cmd)->cmds[i]);
	}
	free((*cmd)->cmds);
	free((*cmd));
	*cmd = NULL;
}

