# Software PWM
This lab is a software implementation of PWM(Pulse Width Modulation). The LED will initially blink at 50% duty cycle. The user can change the duty cycle by 10% increments by pressing the button. When the button is pressed, it will trigger the port interrupt and CCR1 will decrease by a value of 10% of CCR0. The value decreases because this will allow the timer iterrupt for CCR1 to trigger sooner. Because, in this implementation, CCR1 is like set, it will leave the LED on for longer. When the counter counts to CCR0, it will turn off the LED. When the duty cycle reaches 100%, the next button press will cause the LED to turn off. 

After implementation, no further action is needed except button presses.
