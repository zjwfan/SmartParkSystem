#include "GUI.H"
/***************************************************************************
*  չʾ�洢�豸��ʹ�� *
***************************************************************************/
void DemoMemDev(void)
{
GUI_MEMDEV_Handle hMem;
while(1)
{
/* �����洢�豸���� */
hMem = GUI_MEMDEV_Create(0, 0, 110, 18);
/* ����Ȼ��ѡ�������ڻ�ͼ���� */
GUI_MEMDEV_Select(hMem);
/* ��洢�豸��һ���ı�*/
GUI_SetFont(&GUI_FontComic18B_ASCII);
GUI_DispStringAt("Memory device", 0, 0);
/* ���洢�豸�����ݿ����� LCD */
GUI_MEMDEV_CopyToLCD(hMem);
/* ѡ�� LCD ������洢�豸 */
GUI_MEMDEV_Select(0);
GUI_MEMDEV_Delete(hMem);
GUI_Delay(1000);
GUI_Clear();
GUI_Delay(500);
}
}