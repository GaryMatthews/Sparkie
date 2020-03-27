#ifndef LOCOMOTION_H_
#define LOCOMOTION_H_

#include "../lib/Timer/src/Timer.h"
#include "LegMovement.h"
#include "Globals.h"
#include "OdriveParameters.h"

LegMovement legMovement;

/* Variable for jump fuction*/
Timer airTime;
Timer groundTime;
bool runned = false;
uint8_t jump = 0;

/**
 * 
*/
void mod_constrain(float *val, float period)
{
    if (*val * 2 * PI > period || *val < 0)
    {
#ifdef DEBUG1
        Serial.print("PI : ");
        Serial.println(PI);
#endif
        *val = fmodf(*val, period);
        //Serial.println("MOD");
        //Serial.println(*val);
    }
}

/**
 * Shifts the x value in positive or negative direction
*/
void shift(float dx, p *par)
{
    par->dx = dx;
    par->x += dx; // add increment or decrement
    mod_constrain(&par->x, par->period);
}

/**
 * Sets the frequency to your choosing
*/
void set_frequency(float freq, p *par)
{
    par->period = (1 / freq) * 2 * PI; // calculate period
    par->frequency = freq;             // set frequency
}

/**
 * Sets the motors in idle position
*/
void setIdlePosition()
{
    armMotors();
    delay(100);
    double x = 0;
    double y = -160;
    for (int Odrive = 0; Odrive < 4; Odrive++)
    {
        for (int motor = 0; motor < 2; motor++)
        {
            double angle = legMovement.compute(x, y, motor, Odrive);
            double motorCount = map(angle, -360, 360, -6000, 6000);
            setMotorPosition(Odrive, motor, motorCount);
        }
    }
    delay(10);
    disarmMotors();
    idlePosition = true;
}

/**
 * Makes the robot walk,run,forwards,backwards etc
*/
void locomotion(p *params)
{
    int Odrive = 0;
    double x = legMovement.stepX(params->x, params->step_left, params->frequency, PHASESHIFT0X);
    double y = legMovement.stepY(params->x, params->amplitude_over, params->amplitude_under, params->height, params->frequency, PHASESHIFT0Y);
    for (int motor = 0; motor < 2; motor++)
    {
        double angle = legMovement.compute(x, y, motor, Odrive);
        double motorCount = map(angle, -360, 360, -6000, 6000);
        setMotorPosition(Odrive, motor, motorCount);
    }
    Odrive = 1;
    x = legMovement.stepX(params->x, params->step_right, params->frequency, PHASESHIFT1X);
    y = legMovement.stepY(params->x, params->amplitude_over, params->amplitude_under, params->height, params->frequency, PHASESHIFT1Y);
    for (int motor = 0; motor < 2; motor++)
    {
        double angle = legMovement.compute(x, y, motor, Odrive);
        double motorCount = map(angle, -360, 360, -6000, 6000);
        setMotorPosition(Odrive, motor, motorCount);
    }
    Odrive = 2;
    x = legMovement.stepX(params->x, params->step_left, params->frequency, PHASESHIFT2X);
    y = legMovement.stepY(params->x, params->amplitude_over, params->amplitude_under, params->height, params->frequency, PHASESHIFT2Y);
    for (int motor = 0; motor < 2; motor++)
    {
        double angle = legMovement.compute(x, y, motor, Odrive);
        double motorCount = map(angle, -360, 360, -6000, 6000);
        setMotorPosition(Odrive, motor, motorCount);
    }
    Odrive = 3;
    x = legMovement.stepX(params->x, params->step_right, params->frequency, PHASESHIFT3X);
    y = legMovement.stepY(params->x, params->amplitude_over, params->amplitude_under, params->height, params->frequency, PHASESHIFT3Y);
    for (int motor = 0; motor < 2; motor++)
    {
        double angle = legMovement.compute(x, y, motor, Odrive);
        double motorCount = map(angle, -360, 360, -6000, 6000);
        setMotorPosition(Odrive, motor, motorCount);
    }
    shift(val, params);
}

/**
 * Making the robot jump
*/
void jumpCommand()
{
    double x = 0;
    double y;
    switch (jump)
    {
    case 0:
        y = -80;

        if (!runned)
        {
            for (int Odrive = 0; Odrive < 4; Odrive++)
            {
                for (int motor = 0; motor < 2; motor++)
                {
                    double angle = legMovement.compute(x, y, motor, Odrive);
                    double motorCount = map(angle, -360, 360, -6000, 6000);
                    setMotorPosition(Odrive, motor, motorCount);
                }
            }
            groundTime.startTimer(500);
            runned = true;
        }
        if (groundTime.hasTimerExpired())
        {
            jump = 1;
            runned = false;
        }
        break;

    case 1:
        y = -200;
        if (!runned)
        {
            for (int Odrive = 0; Odrive < 4; Odrive++)
            {
                for (int motor = 0; motor < 2; motor++)
                {
                    double angle = legMovement.compute(x, y, motor, Odrive);
                    double motorCount = map(angle, -360, 360, -6000, 6000);
                    setMotorPosition(Odrive, motor, motorCount);
                }
            }
            airTime.startTimer(1000);
            runned = true;
        }
        if (airTime.hasTimerExpired())
        {
            jump = 0;
            runned = false;
        }
        break;
    }
}
#endif // LOCOMOTION_H_