# complex-fourier-series-visualizer
 One day made visualization of complex fourier series on svg files using SFML.

![res/demo.gif]

##Controls
- Up : add one epicycloid
- Down : remove one epicycloid
- S : let you type in the console the number of epicycloids used
- L : let you type in the console the name of the save you want to load

### How to use the save system
The saves are stored in the res/saves file. To create a new save, you need to create a new txt file, put the canvas x size on the first line, canvas y size on the second line and finally put the svg path on the third. To get the 'svg path', you need to fing a svg file, copy the code in the path section (This should start with an m and ends ith a z), paste it in the python programm 'svg_converter.py', copy the result of the programm and finally paste it in the third mine of the file. Now come back to main.exe, press L, type the name of your file and that's it.