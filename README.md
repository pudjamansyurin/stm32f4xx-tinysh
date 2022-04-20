# STM32F4xx Tiny Shell using USART in DMA mode

## How to use the module

#### **`main.c`**
```c
#include "stm32f4xx-tinysh/serial.h"

#define DMA_SZ (32)

/* Private function declarations */
static void foo_fnt(int argc, char **argv);
static void item_fnt(int argc, char **argv);
static void atoxi_fnt(int argc, char **argv);

/* Private variables */
static uint8_t dma_buffer[DMA_SZ];

static tinysh_cmd_t myfoocmd = { 
  0, "foo", "foo command", "[args]", foo_fnt, 0, 0, 0 };
static tinysh_cmd_t ctxcmd = { 
  0, "ctx", "contextual command", "item1|item2", 0, 0, 0, 0 };
static tinysh_cmd_t item1 = { 
  &ctxcmd, "item1", "first item", "[args]", item_fnt, (void*) 1, 0, 0 };
static tinysh_cmd_t item2 = { 
  &ctxcmd, "item2", "second item", "[args]", item_fnt, (void*) 2, 0, 0 };
static tinysh_cmd_t atoxi_cmd = { 
  0, "atoxi", "demonstrate atoxi support", "[args-to-convert]", atoxi_fnt, 0, 0, 0 };

/* Entry Point */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  
  /* Change the prompt */
  tinysh_set_prompt("STM32F4-Tinysh-v0.1$ ");

  /* Add the commands */
  tinysh_add_command(&myfoocmd);
  tinysh_add_command(&ctxcmd);
  tinysh_add_command(&item1);
  tinysh_add_command(&item2);
  tinysh_add_command(&atoxi_cmd);

  /* Initialize the serial using DMA */
  serial_init(&huart2, dma_buffer, DMA_SZ);
  serial_read();

  /* Super loop */
  while(1) {
    // do what you want here
    // command is handled in background
  }
}

/* Private functions definitions */
static void display_args(int argc, char **argv)
{
  for (int i = 0; i < argc; i++)
  {
    printf("argv[%d]=\"%s\"\n", i, argv[i]);
  }
}

static void foo_fnt(int argc, char **argv)
{
  printf("foo command called\n");
  display_args(argc, argv);
}

static void item_fnt(int argc, char **argv)
{
  printf("item%d command called\n", (int) tinysh_get_arg());
  display_args(argc, argv);
}

static void atoxi_fnt(int argc, char **argv)
{
  for (int i = 1; i < argc; i++)
  {
    printf("\"%s\"-->%lu (0x%lx)\n", argv[i],
           tinysh_atoxi(argv[i]),
           tinysh_atoxi(argv[i]));
  }
}
```