import tkinter as tk
import serial.tools.list_ports
import serial as ser
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import numpy as np

def RotMat(angs):
       g = angs[0] #gamma
       b = angs[1] #beta
       a = angs[2] #alpha
       sa = np.sin(a)
       ca = np.cos(a)
       sb = np.sin(b)
       cb = np.cos(b)
       sg = np.sin(g)
       cg = np.cos(g)
       m = np.zeros((3,3))
       m[0][0] = ca*cb
       m[0][1] = ca*sb*sg - sa*cg
       m[0][2] = ca*sb*cg + sa*sg
       m[1][0] = sa*cb
       m[1][1] = sa*sb*sg + ca*cg
       m[1][2] = sa*sb*cg - ca*sg
       m[2][0] = -sb
       m[2][1] = cb*sg
       m[2][2] = cb*cg
       return m
       
def rot(line):
       angles = [float(i) for i in line.split(":")]
       R = RotMat(angles)#Rotation Matrix
       for i in range(0,3):
              xn[i] = np.dot(R[i],x_vector)
              yn[i] = np.dot(R[i],y_vector)
              zn[i] = np.dot(R[i],z_vector)
       xplot.set_xdata([0,xn[0]])
       xplot.set_ydata([0,xn[1]])
       xplot.set_zdata([0,xn[2]])
       yplot.set_xdata([0,yn[0]])
       yplot.set_ydata([0,yn[1]])
       yplot.set_zdata([0,yn[2]])
       zplot.set_xdata([0,zn[0]])
       zplot.set_ydata([0,zn[1]])
       zplot.set_zdata([0,zn[2]])
       canvas.draw()

def update():
       rot(e.get())

root = tk.Tk()

x_vector = np.array([1,0,0])
y_vector = np.array([0,1,0])
z_vector = np.array([0,0,1])
xn = x_vector
yn = y_vector
zn = z_vector

list_ports = ["COM2",""]
list_baudrates = ["9600",""]
sel1 = tk.StringVar()
sel1.set(list_ports[0])
sel2 = tk.StringVar()
sel2.set(list_baudrates[0])

option_bar = tk.Frame(root)
op1 = tk.OptionMenu(option_bar,sel1,*list_ports)
op2 = tk.OptionMenu(option_bar,sel2,*list_baudrates)
bt1 = tk.Button(option_bar,text = "Connect")
bt2 = tk.Button(option_bar,text = "Desconnect")
ckbt1 = tk.Checkbutton(option_bar,text = "Orientation")
ckbt2 = tk.Checkbutton(option_bar,text = "Position")

#Temporário
e = tk.Entry(option_bar)
e.grid(row = 6,column = 0)
e.insert(0,"Pitch:Row:Yaw")
b = tk.Button(option_bar,text = "enviar",command = update)
b.grid(row = 6,column = 1)

op1.grid(row = 0,column = 0)
op2.grid(row = 1,column = 0)
bt1.grid(row = 2,column = 0)
bt2.grid(row = 3,column = 0)
ckbt1.grid(row = 4,column = 0)
ckbt2.grid(row = 5,column = 0)
option_bar.grid(row = 0,column = 0)
#gráficos

fig, ax = plt.subplots(subplot_kw={"projection": "3d"})
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
xplot, = ax.plot([0,1], [0,0], [0,0],color = 'r')
yplot, = ax.plot([0,0], [0,1], [0,0],color = 'b')
zplot, = ax.plot([0,0], [0,0], [0,1],color = 'g')
ax.set(xticklabels=[],
       yticklabels=[],
       zticklabels=[])
canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.draw()
canvas.get_tk_widget().grid(row = 0,column = 1)

root.mainloop()