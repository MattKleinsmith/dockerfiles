import numpy as np
import cv2
import cv2.cv as cv
import facetracker
from video import create_capture
from common import clock, draw_str
import itertools
import scipy.io as sio
import enthought.mayavi.mlab as mlab


help_message = '''
USAGE: facedetect.py [--cascade <cascade_fn>] [--nested-cascade <cascade_fn>] [<video_source>]
'''

if __name__ == '__main__':
    import sys, getopt
    print help_message
    
    args, video_src = getopt.getopt(sys.argv[1:], '', ['face=', 'con=', 'tri='])
    try: video_src = video_src[0]
    except: video_src = 0
    args = dict(args)
    face_fn = args.get('--con', r"..\external\FaceTracker\model\face.tracker")
    con_fn = args.get('--con', r"..\external\FaceTracker\model\face.con")
    tri_fn  = args.get('--tri', r"..\external\FaceTracker\model\face.tri")

    tracker = facetracker.FaceTracker(face_fn)
    conns = facetracker.LoadCon(con_fn)
    trigs = facetracker.LoadTri(tri_fn)

    cam = create_capture(video_src)
    tracker.setWindowSizes((7,))

    shape3D = np.random.randn(3, 66)
    l = mlab.points3d(shape3D[0, :], shape3D[1, :], shape3D[2, :])
    ms = l.mlab_source
                    
    try:
        while True:
            t = clock()
            
            ret, img = cam.read()
            gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            gray = cv2.equalizeHist(gray)
            
            if tracker.update(gray):
                draw_str(img, (20, 40), 'pos: %.1f, %.1f' % tracker.getPosition())
                draw_str(img, (20, 60), 'scale: %.1f ' % tracker.getScale())
                draw_str(img, (20, 80), 'orientation: %.1f, %.1f, %.1f' % tracker.getOrientation())
                tracker.getScale()
                tracker.getOrientation()
                img = tracker.draw(img, conns, trigs)
                
                shape3D = tracker.get3DShape().reshape((3, 66))
                print shape3D.min(), shape3D.max()
                ms.set(x=shape3D[0, :] , y=shape3D[1, :], z=shape3D[2, :])
            else:
                tracker.setWindowSizes((11, 9, 7))
                
            dt = clock() - t
    
            draw_str(img, (20, 20), 'time: %.1f ms' % (dt*1000))
            cv2.imshow('facedetect', img)
    
            if 0xFF & cv2.waitKey(5) == 27:
                break
    except:
        pass
        
    cv2.destroyAllWindows() 			
