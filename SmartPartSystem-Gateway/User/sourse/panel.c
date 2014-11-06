#include "GUI.H"
/***************************************************************************
*  展示存储设备的使用 *
***************************************************************************/
void DemoMemDev(void)
{
GUI_MEMDEV_Handle hMem;
while(1)
{
/* 建立存储设备…… */
hMem = GUI_MEMDEV_Create(0, 0, 110, 18);
/* ……然后选择其用于绘图操作 */
GUI_MEMDEV_Select(hMem);
/* 向存储设备绘一个文本*/
GUI_SetFont(&GUI_FontComic18B_ASCII);
GUI_DispStringAt("Memory device", 0, 0);
/* 将存储设备的内容拷贝到 LCD */
GUI_MEMDEV_CopyToLCD(hMem);
/* 选择 LCD 并清除存储设备 */
GUI_MEMDEV_Select(0);
GUI_MEMDEV_Delete(hMem);
GUI_Delay(1000);
GUI_Clear();
GUI_Delay(500);
}
}