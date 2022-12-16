#define VOLTAGE_PIN 32
#define VOLTAGE_SENSIBILITY 0.5
#define MAX_VOLTAGE 5
#define VOLTAGE_DELAY_MS 1000
#define VOLTAGE_STACK_DEPTH 4096
#define VOLTAGE_TASK_PRIORITY 1

void initVoltage();
void resumeVoltageTask();
void suspendVoltageTask();
float getVoltage();
