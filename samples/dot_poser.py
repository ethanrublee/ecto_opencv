#!/bin/python
import ecto
#import ecto_opencv.cv_bp as opencv
from ecto_opencv import highgui,calib,imgproc

debug = True

plasm = ecto.Plasm()
rows = 11
cols = 4
square_size = 40 # in millis

pattern_show = highgui.imshow(name="pattern", waitKey=10, autoSize=True)
rgb2gray = imgproc.cvtColor(flag=7)
video = highgui.VideoCapture(video_device=1)
circle_detector = calib.PatternDetector(rows=rows, cols=cols,pattern_type="acircles",square_size=square_size)
circle_drawer = calib.PatternDrawer(rows=rows, cols=cols)
poser = calib.FiducialPoseFinder()
pose_drawer = calib.PoseDrawer()


plasm.connect(video, "out", rgb2gray, "input")
plasm.connect(rgb2gray, "out", circle_detector, "input")
plasm.connect(video, "out", circle_drawer, "input")
plasm.connect(circle_detector, "out", circle_drawer, "points")
plasm.connect(circle_detector, "found", circle_drawer, "found")
plasm.connect(circle_drawer, "out", pattern_show, "input")
plasm.connect(circle_detector, "out", poser,"points")
plasm.connect(circle_detector, "ideal", poser,"ideal")
plasm.connect(circle_detector, "found", poser, "found")

ecto.view_plasm(plasm)

while(pattern_show.outputs.out != 27):
    plasm.execute()
    

