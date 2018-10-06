# Software Debouncing
Debouncing is problem of switches such as buttons when the button is pressed once it will actually trigger several times. This is due to the nature of the button "bouncing" when it is pressed. Therefore, in this lab I debounce the button by implementing timer module and button interrupt.

On the first positive edge of a button press, I want to wait a designated time before allowing the button to trigger again. I accomplish this by using a boolean timerDelayed. Initialially, timerDelayed is true. Once the button is pressed, for the first positive edge, the code in the button interrupt is executed. A timer module is initialized and starts counting from 0 and timerDelayed is set to false. The code in the interrupt only executed when timerDelayed is true. Therefore, even if the button interrupt occurs several times, the code will not execute. Only after the timer module counts to CCR0 will the timer interrupt occur and set timerDelayed to be true. After this occurs, the button can be pressed again to trigger the interrupt.

No further action is needed after implementation.
