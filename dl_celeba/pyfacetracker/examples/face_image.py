from __future__ import division
import numpy as np
import facetracker
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import Image
import argparse


def main(img_path):
    #
    # Load image
    #
    img = Image.open(img_path)
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
        ax.view_init(-90, -90)

    else:
        print 'Failed tracking face in image'
        
    plt.figure()
    plt.imshow(img)
    
    plt.show()
    
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Run pyfacetrack on an image')
    parser.add_argument('path', help='Path to image', default=None)
    
    args = parser.parse_args()
    
    main(args.path)
    