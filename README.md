# STM32F4xx Tiny Shell using USART in DMA mode

## How to use the module
You should enable <b>USE_HAL_UART_REGISTER_CALLBACKS</b> macro

#### **`main.c`**
```c
#include "stm32f4xx-tinysh/serial.h"

#define USE_HAL_UART_REGISTER_CALLBACKS 1
#define BUF_SZ (32)

/* Private function declarations */
static void command_init(void);
static void foo_fnt(int argc, char **argv);
static void item_fnt(int argc, char **argv);
static void atoxi_fnt(int argc, char **argv);

/* Private variables */
static stdin_t hstdin;
static uint8_t Buffer[BUF_SZ];

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
  
  /* Initialize the tinysh*/
  command_init();
  
  /* Initialize serial layer */  
  stdout_init(&huart2, NULL);
  stdin_init(&hstdin, &huart2, Buffer, BUF_SZ);
  stdin_set_callback(&hstdin, tinysh_chars_in);
  stdin_start(&hstdin);

  /* Super loop */
  while(1) {
    
  }
}

/* Private functions definitions */
static void command_init(void) {
  /* Add default prompt */
  tinysh_set_prompt("STM32F4-Tinysh-v3.5.4$ ");

  /* Add the commands */
  tinysh_add_command(&myfoocmd);
  tinysh_add_command(&ctxcmd);
  tinysh_add_command(&item1);
  tinysh_add_command(&item2);
  tinysh_add_command(&atoxi_cmd);
}

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


#### **`stm32f4xx_it.c`**
```c
/* USER CODE BEGIN Includes */
#include "stm32f4xx-tinysh/serial.h"
/* USER CODE END Includes */

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */
  stdin_irq_dma(&hstdin);
  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
  stdin_irq_uart(&hstdin);
  /* USER CODE END USART2_IRQn 1 */
}

```