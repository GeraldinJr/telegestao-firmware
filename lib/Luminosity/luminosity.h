// PINS
#define LDR_PIN 35

// GAMMA
#define GAMMA 0.7
#define INVERSE_GAMMA 70

// PHYSICAL PARAMS
#define VOLTAGE_COEFFICIENT 0.001220703125
#define SENSOR_RESISTANCE 2000
#define VIN 5
#define RL10 50000

// GENERICS PARAMS
#define SENSIBILIDADE_LUMINOSIDADE 50 //lux
#define LUX_DELAY_MS 2000
#define LUX_STACK_DEPTH 4096
#define LUX_TASK_PRIORITY 1
#define MIN_LUX 500


void initLux();
void resumeLuminosityTask();
void suspendLuminosityTask();