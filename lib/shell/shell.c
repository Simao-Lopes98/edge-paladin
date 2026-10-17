/*
shell.c

Baisc shell based on the design made by Stephen Brennan.
See: https://brennan.io/2015/01/16/write-a-shell-in-c/

*/
#include <shell.h>

static char * shellReadLine(void);
static char ** shellSplitLine(char * line);
static int shellExecute(char ** args);
static size_t shellGetFunctNumb(void);

/* Built Ins */
static int shellHelp (char **);


char *builtInStr[] = {
  "help"
};

static int (* builtInFunc[]) (char **) = {
    &shellHelp
};

/* Shell Service */
void shellSrv(void * pvParameters )
{
    char * line;
    char ** args;
    int status;

  do {
    printf("> ");
    line = shellReadLine();
    args = shellSplitLine(line);
    status = shellExecute(args);

    free(line);
    free(args);
  } while (status);
}

static char * shellReadLine(void)
{
  int bufsize = SHELL_LINE_MAX_BUFF_SIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (buffer == NULL) {
    ESP_ERROR_CHECK (ESP_ERR_NO_MEM);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += SHELL_LINE_MAX_BUFF_SIZE;
      buffer = realloc(buffer, bufsize);
      if (buffer == NULL) {
        ESP_ERROR_CHECK (ESP_ERR_NO_MEM);
      }
    }
  } 
}

static char ** shellSplitLine(char * line)
{
  int bufsize = SHELL_TOKEN_BUF_SIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (tokens == NULL) {
    ESP_ERROR_CHECK (ESP_ERR_NO_MEM);
  }

  token = strtok(line, SHELL_TOKEN_DELIM);
  while (token != NULL) {
    // ESP_LOGI(LOG_LOCAL_LEVEL,"Token %d - %s", position, token);
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SHELL_TOKEN_BUF_SIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (tokens == NULL) {
        ESP_ERROR_CHECK (ESP_ERR_NO_MEM);
      }
    }

    token = strtok(NULL, SHELL_TOKEN_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

static size_t shellGetFunctNumb(void)
{
    return sizeof(builtInFunc)/sizeof(char*);  
}

static int shellExecute(char ** args)
{
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (int i = 0; i < shellGetFunctNumb(); i++) {
    if (strcmp(args[0], builtInStr[i]) == 0) {
      return (*builtInFunc[i])(args);
    }
  }

  return 1;
}

/* Built Ins */
static int shellHelp (char **)
{
    printf ("Hello form the SHELL :)");
    
    return 1;
}