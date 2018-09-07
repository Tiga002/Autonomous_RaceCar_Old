# -*- coding:utf8 -*-

import os

class BatchRename():
    '''
    Change name of jpegs

    '''
    def __init__(self):
        # Path of the files 
        self.path = 'JPEGImages'

    def rename(self):
        filelist = os.listdir(self.path)
        total_num = len(filelist)
        # 0000001 , ... , 0000010
        i = 1
        n = 6
        for item in filelist:
            if item.endswith('.jpg'):
                    n = 6-len(str(i))
                    src = os.path.join(os.path.abspath(self.path), item)
                    dst = os.path.join(os.path.abspath(self.path), str(0)*n + str(i) + '.jpg')
                    try:
                        os.rename(src, dst)
                        print('converting %s to %s ...' % (src, dst))
                        i = i + 1
                    except:
                        continue
        print ('total %d to rename & converted %d .jpg' % (total_num, i))

if __name__ == '__main__':
    demo = BatchRename()
    demo.rename()
