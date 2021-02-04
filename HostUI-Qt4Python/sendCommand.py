# -*- coding: utf-8 -*-
"""
Created on Wed Feb  3 16:37:14 2021

@author: 23731
"""

BYTEORDER = 'big'

def sendCmd(serialDevice,cmd,arg1,arg2):
    sendRes = (0xa9).to_bytes(length = 1,byteorder = BYTEORDER,signed = False)
    
    cmd = str(cmd)[0]
    sendRes += cmd.encode('ascii')
    
    arg1List = list( int(arg1).to_bytes(length = 2, 
                                        byteorder= BYTEORDER, 
                                        signed = False))
    arg1List.reverse()
    for i in arg1List:
        sendRes += i.to_bytes(length = 1,
                              byteorder = BYTEORDER,
                              signed = False)
    
    arg2List = list( int(arg2).to_bytes(length = 2, 
                                  byteorder= BYTEORDER, 
                                  signed = False) )
    arg2List.reverse()
    for i in arg2List:
        sendRes += i.to_bytes(length = 1,
                              byteorder = BYTEORDER,
                              signed = False)
    
    
    sendRes += 'a'.encode('ascii')
    
    sendRes += (0xa6).to_bytes(length = 1,byteorder = BYTEORDER,signed = False)
    
    print(sendRes)
    
    serialDevice.write(sendRes)
    
    print('发送uart命令')
