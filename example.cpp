#include "mbed.h"
#include "ikarashiCAN_mk2.h"
#include <button_state.h>

ikarashiCAN_mk2 can1(D15, D14, 1);
DigitalOut led1(LED1);
DigitalIn button(USER_BUTTON);
Button_state btn_state;
Thread thread1;
Ticker ticker;

uint8_t counter = 0;
unsigned int target_id = 0;
int counter_ms = 0;
int btn_in = 0;

void send()
{
    while (1)
    {
        if (can1.write(target_id))
        {
            counter++;
        }
        else
        {
        }
        ThisThread::sleep_for(10ms);
    }
}

void main_update();

int main()
{
    btn_state.set(&btn_in);
    led1 = 0;
    printf("main()\n");
    thread1.start(send);
    ticker.attach(&main_update, 1ms);
    can1.read_start();

    while (1)
    {
        printf("\nsf:%d  rf:%d  rmsg:%d  cnt:%d  id:%d  tar:%d  this:%d", can1.get_send_flag(), can1.get_read_flag(), can1.get_byte(0), counter, can1.get_id(), can1.get_target_id(), can1.get_this_id());
    }
}

void main_update()
{
    btn_in = button.read();
    led1 = can1.get_read_flag();
    can1.set(&counter, 1);
    if (btn_state.state() == 1)
    {
        target_id++;
        can1.set_this_id(target_id % 10 + 1);
    }
}