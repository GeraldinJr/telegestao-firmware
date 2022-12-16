#define CURRENT_PIN 34
#define CURRENT_SENSIBILITY 0.2
#define MAX_CURRENT 1.0
#define CURRENT_DELAY_MS 1000
#define CURRENT_STACK_DEPTH 4096
#define CURRENT_TASK_PRIORITY 1

void initCurrent();
void resumeCurrentTask();
void suspendCurrentTask();
float getCurrent();