#define BUTTON 4
#define BUTTON_DEBOUNCE 1000 //ms
# define STACK_DEPTH 4096
void initButton();
void buttonTask(void *arg);