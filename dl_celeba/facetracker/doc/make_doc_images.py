from __future__ import division
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from mpl_toolkits.mplot3d import Axes3D
import facetracker
import Image
import os

IMAGES_BASE = 'images'


def savefig(file_name):
    plt.savefig(os.path.join(IMAGES_BASE, file_name), bbox_inches='tight', pad_inches=0)


def tutorial1():
    
    img = Image.open(os.path.join(IMAGES_BASE, 'MonaLisa.jpg'))
    gray = img.convert('L')
    img = np.asanyarray(img)
    gray = np.asarray(gray)

    #
    # Load face model
    #
    conns = facetracker.LoadCon(r'..\external\FaceTracker\model\face.con')
    trigs = facetracker.LoadTri(r'..\external\FaceTracker\model\face.tri')
    tracker = facetracker.FaceTracker(r'..\external\FaceTracker\model\face.tracker')
    
    #
    # Search for faces in the image
    #
    tracker.setWindowSizes((11, 9, 7))
    if tracker.update(gray):
        img = tracker.draw(img, conns, trigs)

        obj3D = tracker.get3DShape()
    
        fig3d = plt.figure()
        ax = fig3d.add_subplot(111, projection='3d')
        ax.scatter(obj3D[:66, 0], obj3D[66:132, 0], obj3D[132:, 0])
        for i in range(66):
            ax.text(obj3D[i], obj3D[i+66], obj3D[i+132], str(i))
        ax.view_init(-75, -90)
        plt.title('3D Model of Mona Lisa\'s face')
        savefig('3DMonaLisa')
    else:
        print 'Failed tracking face in image'
        
    plt.figure()
    plt.imshow(img)
    plt.title('Face of the Mona Lisa')
    savefig('MonaLisaFace')

    

if __name__ == '__main__':
    tutorial1()    
    plt.show()