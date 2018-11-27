#ifndef DEVICEINC_H
#define DEVICEINC_H

#ifdef IDE
    #include "devicescene.h"
#elif DISPLAY
    #include "devicemanager.h"
#elif SETTING
    #include "../BST_IDE/setting/devicesetbase.h"
    #ifdef SODIMAS
        #include "setting_sodimas/devicesetting.h"
    #else
        #include "devicesetting.h"
    #endif
#elif SERVER
    #include "../BST_IDE/setting/devicesetbase.h"
    #include "serverform.h"
#elif APP
    #include "../BST_IDE/setting/devicesetbase.h"
    #include "appform.h"
#elif PRODUCT
    #include "../BST_PRODUCT/product/mainwindow.h"
#endif

#endif // DEVICEINC_H

