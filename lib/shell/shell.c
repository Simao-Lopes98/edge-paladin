/*
shell.c

Baisc shell based on the design made by Stephen Brennan.
See: https://brennan.io/2015/01/16/write-a-shell-in-c/

*/
#include <shell.h>
#include <uart.h>

static int shellLineIo(void);
static int shellWriteNewLine (void);
static char ** shellSplitLine(char * line);
static int shellExecute(char ** args);
static size_t shellGetNumbCmd(void);
static void shellPrintBanner(void);

/* CMDs*/
static int shellHelp (char **);
static int shellClear (char **);

/* Register CMDs */

CMD_REGISTER(help, &shellHelp, SHELL_HELP_DESC);
CMD_REGISTER(clear, &shellClear, SHELL_CLEAR_DESC);

static char line [UART_RX_BUFF_SIZE];

static const char *banner =
"\r\n"
"  ____   _    _      _    ____ ___ _   _ \r\n"
" |  _ \\ / \\  | |    / \\  |  _ \\_ _| \\ | |\r\n"
" | |_) / _ \\ | |   / _ \\ | | | | ||  \\| |\r\n"
" |  __/ ___ \\| |__/ ___ \\| |_| | || |\\  |\r\n"
" |_| /_/   \\_\\____/_/   \\_\\____/___|_| \\_|\r\n"
"\r\n"
"   Embedded Systems Command Interface\r\n"
"\r\n";

/* Shell Service */
void shellSrv(void * pvParameters )
{
	ESP_LOGI (SHELL_LOG_TAG, "Starting service");

	char ** args = NULL;
	// Helps not writing ">" when no new input was added
	bool writeNeeded = true;
	vTaskDelay(pdMS_TO_TICKS(150));

	shellPrintBanner();
	
	while (true)
	{
		if (writeNeeded == true)
		{
			shellWriteNewLine();
			shellWriteLine("> ");
		}

		shellLineIo();

		shellWriteNewLine();

		ESP_LOGI (SHELL_LOG_TAG, "Incoming command %s", line);

		args = shellSplitLine(line);
		if (args == NULL)
		{
			// Error reading let user know
			ESP_LOGW (SHELL_LOG_TAG, "Error shellSplitLine");
			// Tap task
			vTaskDelay(pdMS_TO_TICKS(100));
			continue;
		}

		(void) shellExecute(args);

		// Clean up
		free(args);

		writeNeeded = true; // New line needed
	}
  
}

int shellWriteLine (char * line)
{
  return uartWrite (UART_PORT, line);
}

static int shellWriteNewLine (void)
{
	char newLine [] = "\r\n";
	return uartWrite (UART_PORT, newLine);
}
/*
Function reads each char and echos it until \r or \n 
*/
static int shellLineIo(void)
{
	char c = 0;
	int count = 0;
	int ret = 0;
	do
	{
		ret = uartRead(UART_NUM_1, &c);
		if (ret < 0)
		{
			// Error reading let user know
			ESP_LOGE (SHELL_LOG_TAG, "Error readUartDrv");
			return -1;
		} else if (ret == 0)
		{
			// Nothing to read continue
			continue;
		}

		// Backspace
		if (c == 8)
		{
			if (count > 0)
			{
				shellWriteLine("\b \b");
				count--;
				line[count] = '\0';
			}
			continue;
		}

		// Other than Enter
		if (c != '\n' && c != '\r')
		{
			// echo char
			uartWriteByte(UART_NUM_1, &c);
			line[count] = c;
			count++;
		}
		
	} while (c != '\n' && c != '\r' && count < (UART_RX_BUFF_SIZE - 1));

	// Add EOS
	line[count] = '\0';
	return count;
}

static char ** shellSplitLine(char * line)
{
  int bufsize = SHELL_TOKEN_BUF_SIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (tokens == NULL) {
    return NULL;
  }

  token = strtok(line, SHELL_TOKEN_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SHELL_TOKEN_BUF_SIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (tokens == NULL) {
        return NULL;
      }
    }

    token = strtok(NULL, SHELL_TOKEN_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

static size_t shellGetNumbCmd(void)
{
    return CMD_COUNT();
}

static int shellExecute(char ** args)
{
	char unkownCmdMsg [SHELL_UNKOWN_CMD_MSG_SZ];
	if (args[0] == NULL) {
		// An empty command was entered.
		return 1;
	}

    for (const shellCmd_t *c = _my_cmds_start; c < _my_cmds_end; c++)
	{	
		if (strcmp(args[0], c->name) == 0) {
			return (c->function(args));
		}
	}

	// Unknown command
	snprintf (unkownCmdMsg, SHELL_UNKOWN_CMD_MSG_SZ, "%s : %s",
	SHELL_UNKOWN_CMD_STR, args[0]);
	ESP_LOGI(SHELL_LOG_TAG, "Invalid cmd: %d", shellGetNumbCmd());
	shellWriteLine (unkownCmdMsg);

	return 1;
}

static void shellPrintBanner(void)
{
	uartWrite (UART_PORT, (char *) banner);
}

/* Built Ins */
static int shellHelp (char **)
{
	shellWriteLine (SHELL_HELP_DESC);
	shellWriteNewLine();
	/* Cycle through memory region where cmds are registerd */
    for (const shellCmd_t *c = _my_cmds_start; c < _my_cmds_end; c++)
	{
		// Skip "help" it self
		if (strncmp ((char *)c->name, "help", SHELL_BUILTIN_MAX_NAME) == 0)
		{
			continue;
		}
		shellWriteLine ((char *)c->name);
		shellWriteLine (" - ");
		shellWriteLine ((char *)c->desc);
		shellWriteNewLine();
	}
	
    return 1;
}

static int shellClear (char **)
{
    uartWrite (UART_PORT, "\033[2J\033[H");
    return 1;
}