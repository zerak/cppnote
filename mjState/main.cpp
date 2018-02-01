
#include <unistd.h>
#include "mjDesk.h"

int main()
{
    for (int i = 0; i < 100; ++i)
    {
        MjDesk desk;
        desk.start();

        // loop
        while (true)
        {
            desk.loop();
            sleep(1);
        }
    }

    return 0;
}
