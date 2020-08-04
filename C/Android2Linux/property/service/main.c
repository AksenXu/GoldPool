#include "property_service.h"

int main()
{
    //初始化property的共享内存区域
    property_init();
    
    //加载默认的prop文件, 创建socket, 等外部的进程来设置属性值
    start_property_service();
    
    while(true) {
        //select();
        //get_property_set_fd();
        
        handle_property_set_fd();
    }
    return 0;
}
