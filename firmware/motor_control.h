#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include <ch.h>
#include <hal.h>


void setMot1(float speed);
void setMot2(float speed);
void setMot3(float speed);

void start_motor_control_pid(void);

#endif // __MOTOR_CONTROL_H__
