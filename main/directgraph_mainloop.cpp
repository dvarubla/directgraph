#include <directgraph_mainloop.h>
#include <stdint.h>
#include <stdio.h>

WPARAM directgraph_mainloop() {
    MSG msg;
    uint_fast32_t num_windows = 0;
    while (GetMessageW(&msg, NULL, 0, 0)){
        if(msg.message == DIRECTGRAPH_WND_CREATE){
            num_windows++;
        } else if(msg.message == DIRECTGRAPH_WND_QUIT){
            num_windows--;
            if(num_windows == 0){
                break;
            }
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}
