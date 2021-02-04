import sys
from PyQt5.QtWidgets import QApplication,QWidget,QMainWindow
from PyQt5 import QtCore
from dryerV1 import *
from functools import partial
from PyQt5.QtCore import QCoreApplication

import serial
import serial.tools.list_ports

import sendCommand

'''
串口参数设置
'''
BPS=115200 # 串口通信波特率

'''
内部参数
'''
deviceConnected = False

portChoose = None
portOpen = None

portList = []

serialDevice = None

deviceRunning = False

heaterSliderList = []
heaterLineEditList = []

def click_success():
    print('更新数据')
    
    
'''--电机页面数据互相绑定--'''    

def update_MotorDuty_slide(ui,channel):
    '''
    更新占空比
    槽函数

    Parameters
    ----------
    ui : TYPE
        界面对象.
    channel : i
        通道号.

    Returns
    -------
    None.

    '''
    res = 0
    if channel == 1:
        res = ui.horizontalSlider_Channel_M1.value() / 10
        ui.lineEdit_Channel_M1.setText(str(res)+'%')
        print('更新通道一占空比:'+str(res))
    elif channel == 2:
        res = ui.horizontalSlider_Channel_M2.value() / 10
        ui.lineEdit_Channel_M2.setText(str(res)+'%')
        print('更新通道二占空比:'+str(res))
    elif channel == 3:
        res = ui.horizontalSlider_Channel_M3.value() / 10
        ui.lineEdit_Channel_M3.setText(str(res)+'%')
        print('更新通道三占空比:'+str(res))
    elif channel == 4:
        res = ui.horizontalSlider_Channel_M4.value() / 10
        ui.lineEdit_Channel_M4.setText(str(res)+'%')
        print('更新通道四占空比:'+str(res))
    elif channel == 5:
        res = ui.horizontalSlider_Channel_M5.value() / 10
        ui.lineEdit_Channel_M5.setText(str(res)+'%')
        print('更新通道五占空比:'+str(res))
    elif channel == 6:
        res = ui.horizontalSlider_Channel_M6.value() / 10
        ui.lineEdit_Channel_M6.setText(str(res)+'%')
        print('更新通道六占空比:'+str(res))
    elif channel == 7:
        res = ui.horizontalSlider_Channel_M7.value() / 10
        ui.lineEdit_Channel_M7.setText(str(res)+'%')
        print('更新通道七占空比:'+str(res))
    elif channel == 8:
        res = ui.horizontalSlider_Channel_M8.value() / 10
        ui.lineEdit_Channel_M8.setText(str(res)+'%')
        print('更新通道八占空比:'+str(res))
        
    if deviceConnected:
        sendCommand.sendCmd(serialDevice,'M',channel,res*10);
        

def update_MotorDuty_text(ui,channel):
    if channel == 1:
        res = ui.lineEdit_Channel_M1.text()
        ui.horizontalSlider_Channel_M1.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道一占空比'+str(res))
    elif channel == 2:
        res = ui.lineEdit_Channel_M2.text()
        ui.horizontalSlider_Channel_M2.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道二占空比'+str(res))
    elif channel == 3:
        res = ui.lineEdit_Channel_M3.text()
        ui.horizontalSlider_Channel_M3.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道三占空比'+str(res))
    elif channel == 4:
        res = ui.lineEdit_Channel_M4.text()
        ui.horizontalSlider_Channel_M4.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道四占空比'+str(res))
    elif channel == 5:
        res = ui.lineEdit_Channel_M5.text()
        ui.horizontalSlider_Channel_M5.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道五占空比'+str(res))
    elif channel == 6:
        res = ui.lineEdit_Channel_M6.text()
        ui.horizontalSlider_Channel_M6.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道六占空比'+str(res))
    elif channel == 7:
        res = ui.lineEdit_Channel_M7.text()
        ui.horizontalSlider_Channel_M7.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道七占空比'+str(res))
    elif channel == 8:
        res = ui.lineEdit_Channel_M8.text()
        ui.horizontalSlider_Channel_M8.setValue( float( str(res).replace('%','') ) * 10)
        print('输入通道八占空比'+str(res))
        
    if deviceConnected:
        sendCommand.sendCmd(serialDevice,
                            'M',
                            channel,
                            float( str(res).replace('%','') ) * 10);
        
'''加热器的写法使用list改进型'''
def update_HeaterDuty_slide(channel):
    global heaterSliderList,heaterLineEditList;
    res = heaterSliderList[channel].value()
    heaterLineEditList[channel].setText(str(res/10)+'%')
    print('更新加热器通道'+str(channel+1)+'占空比：'+str(res/10)+'%')
    if deviceConnected:
        sendCommand.sendCmd(serialDevice,
                            'R',
                            channel,
                            res);

def update_HeaterDuty_text(channel):
    global heaterSliderList,heaterLineEditList;
    res = heaterLineEditList[channel].text();
    res = float( str(res).replace('%','') ) * 10;
    heaterSliderList[channel].setValue(res);
    print('输入加热器通道'+str(channel+1)+'占空比：'+str(res/10)+'%')
    if deviceConnected:
        sendCommand.sendCmd(serialDevice,
                            'R',
                            channel,
                            int(res));

'''-- 按钮槽函数 --'''
    
def updateDevice(ui):
    if deviceConnected:
        # 更新控制板上运行的参数至本地（上位机）
        pass
    else:
        # 未连接设备
        # 查找目前可用的串口设备
        global portList
        portList = list(serial.tools.list_ports.comports())    # 获得可用串口列表
        ui.comboBox_COM.clear()
        for i in portList:
            print(i)
            ui.comboBox_COM.addItem(i.name)

def chooseCOM(i):
    global portChoose
    print(i)    
    portChoose = portList[i]
    
def connectDevice(ui):
    global deviceConnected,portOpen,serialDevice
    if deviceConnected: # 如果设备已连接
        portOpen = None
        deviceConnected = False
        serialDevice.close()
        ui.label_connectStatus.setText('未连接')
        print('断开设备')
        return
    else:   # 如果设备未连接
        try:
            serialDevice = serial.Serial(portChoose.name,BPS,timeout=0)
        except Exception as e:
            print("串口打开失败")
            ui.label_connectStatus.setText('未连接')
            return
        else:
            print('连接串口：'+serialDevice.name)
            ui.label_connectStatus.setText('已连接'+ serialDevice.name)
            portOpen = portChoose
            deviceConnected = True
            return
        
def stopOrRun():
    global deviceRunning;
    if deviceConnected:
        if deviceRunning:
            sendCommand.sendCmd(serialDevice,'S',0,0);  # 发送停止命令
            deviceRunning = False;
        else:
            deviceRunning = True;
            sendCommand.sendCmd(serialDevice,'K',0,0);  # 发送开始命令

'''-- 主程序 --'''

if __name__ == '__main__':
    '''--创建窗口对象--'''
    app = QApplication(sys.argv)
    mainWindow = QMainWindow()
    ui = Ui_Dialog()
    ui.setupUi(mainWindow)
    mainWindow.show()
    
    '''-- 将ui中的对象以list组织 --'''
    heaterSliderList.append(ui.horizontalSlider_Channel_H1)
    heaterSliderList.append(ui.horizontalSlider_Channel_H2)
    heaterSliderList.append(ui.horizontalSlider_Channel_H3)
    heaterSliderList.append(ui.horizontalSlider_Channel_H4)
    heaterSliderList.append(ui.horizontalSlider_Channel_H5)
    heaterSliderList.append(ui.horizontalSlider_Channel_H6)
    heaterSliderList.append(ui.horizontalSlider_Channel_H7)
    heaterSliderList.append(ui.horizontalSlider_Channel_H8)
    
    heaterLineEditList.append(ui.lineEdit_Channel_H1)
    heaterLineEditList.append(ui.lineEdit_Channel_H2)
    heaterLineEditList.append(ui.lineEdit_Channel_H3)
    heaterLineEditList.append(ui.lineEdit_Channel_H4)
    heaterLineEditList.append(ui.lineEdit_Channel_H5)
    heaterLineEditList.append(ui.lineEdit_Channel_H6)
    heaterLineEditList.append(ui.lineEdit_Channel_H7)
    heaterLineEditList.append(ui.lineEdit_Channel_H8)
    
    '''--设置信号、槽--'''
    # 设置滑轨和输入框绑定
    ui.horizontalSlider_Channel_M1.sliderReleased.connect(partial(update_MotorDuty_slide, ui,1))
    ui.lineEdit_Channel_M1.editingFinished.connect(partial(update_MotorDuty_text,ui,1))
    
    ui.horizontalSlider_Channel_M2.sliderReleased.connect(partial(update_MotorDuty_slide, ui,2))
    ui.lineEdit_Channel_M2.editingFinished.connect(partial(update_MotorDuty_text,ui,2))
    
    ui.horizontalSlider_Channel_M3.sliderReleased.connect(partial(update_MotorDuty_slide, ui,3))
    ui.lineEdit_Channel_M3.editingFinished.connect(partial(update_MotorDuty_text,ui,3))
    
    ui.horizontalSlider_Channel_M4.sliderReleased.connect(partial(update_MotorDuty_slide, ui,4))
    ui.lineEdit_Channel_M4.editingFinished.connect(partial(update_MotorDuty_text,ui,4))
    
    ui.horizontalSlider_Channel_M5.sliderReleased.connect(partial(update_MotorDuty_slide, ui,5))
    ui.lineEdit_Channel_M5.editingFinished.connect(partial(update_MotorDuty_text,ui,5))
    
    ui.horizontalSlider_Channel_M6.sliderReleased.connect(partial(update_MotorDuty_slide, ui,6))
    ui.lineEdit_Channel_M6.editingFinished.connect(partial(update_MotorDuty_text,ui,6))
    
    ui.horizontalSlider_Channel_M7.sliderReleased.connect(partial(update_MotorDuty_slide, ui,7))
    ui.lineEdit_Channel_M7.editingFinished.connect(partial(update_MotorDuty_text,ui,7))
    
    ui.horizontalSlider_Channel_M8.sliderReleased.connect(partial(update_MotorDuty_slide, ui,8))
    ui.lineEdit_Channel_M8.editingFinished.connect(partial(update_MotorDuty_text,ui,8))
    
    for i in range(0,8):
        heaterSliderList[i].sliderReleased.connect( partial(update_HeaterDuty_slide,i) )
        heaterLineEditList[i].editingFinished.connect( partial(update_HeaterDuty_text,i) )
    
    
    
    # 设置按钮
    ui.comboBox_COM.currentIndexChanged.connect(chooseCOM)          # 选择端口
    ui.Button_update.clicked.connect(partial(updateDevice, ui) )    # 更新设备状态
    ui.Button_connect.clicked.connect(partial(connectDevice,ui))    # 连接&断开 按钮
    ui.Button_stop.clicked.connect(stopOrRun)   # 启动&停止 按钮
    
    # 默认开机自动更新可用设备
    updateDevice(ui)
    
    sys.exit(app.exec_())
    