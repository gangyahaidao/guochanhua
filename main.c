#include "common.h"
#include "device.h"

int main(int argc, char* argv[]) {
	//ULONG SKF_WaitForDevEvent(LPSTR szDevName, ULONG* pulDevNameLen, ULONG* pulEvent);
    ULONG pulDevNameLen = 50;
    LPSTR szDevName = calloc(1, pulDevNameLen);
    ULONG pulEvent = -1;
    ULONG ret = SKF_WaitForDevEvent(szDevName, &pulDevNameLen, &pulEvent); // 阻塞接收数据
    if(ret == SAR_OK) {
        printf("阻塞设备返回, 事件类型 = %d, name = %s\n", pulEvent, szDevName); // "Haitai HaiKey HID 0"返回设备名称
    }

    return 0;
}