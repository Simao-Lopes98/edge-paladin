/*
shell.c

Baisc shell based on the design made by Stephen Brennan.
See: https://brennan.io/2015/01/16/write-a-shell-in-c/

*/
#include <shell.h>
#include <uart.h>

static int shellLineIo(void);
static char ** shellSplitLine(char * line);
static int shellExecute(char ** args);
static size_t shellGetFunctNumb(void);
static int shellWriteLine (char * line);

/* Built Ins */
static int shellHelp (char **);


char *builtInStr[] = {
  "help"
};

static int (* builtInFunc[]) (char **) = {
    &shellHelp
};

static char line [UART_RX_BUFF_SIZE];

/* Shell Service */
void shellSrv(void * pvParameters )
{
    ESP_LOGI (SHELL_LOG_TAG, "Starting service");

    int ret = 0;
    char ** args = NULL;
    bool writeNeeded = true; // Helps not writing ">" when no new input was added

  while (true)
  {
    if (writeNeeded == true)
    {
      ret = shellWriteLine("> ");
      if (ret < 0)
      {
        // Error writing let user know
        ESP_LOGW (SHELL_LOG_TAG, "Error shellWriteLine");
        // Tap task
        vTaskDelay(pdMS_TO_TICKS(100));
        continue;
      }
    }

    ret = shellLineIo();

    ESP_LOGI (SHELL_LOG_TAG, "Read %s", line);

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
    vTaskDelay(pdMS_TO_TICKS(50)); // Some time to avoid the watchdog
  }
  
}

static int shellWriteLine (char * line)
{
  return uartWrite (UART_PORT, line);
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
      // Tap task
      continue;
    } else if (ret == 0)
    {
      // Nothing to read continue
      continue;
    }
    // echo char
    uartWriteByte(UART_NUM_1, &c);
    line[count] = c;
    count++;
    ESP_LOGI(SHELL_LOG_TAG, "%c, %d", c, c);
  } while (c != '\r' || c != '\n');
  

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
    // ESP_LOGI(LOG_LOCAL_LEVEL,"Token %d - %s", position, token);
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
    uartWrite (UART_PORT, "Hello form the SHELL :)");
    
    return 1;
}