from tkinter import *

#!/usr/bin/env python3

#Run "pyinstaller --onefile --windowed MENG_GUI.py" in Windows Command Prompt (from program directory) to make .executable (.exe initially in dist folder)

#Importing Necessary Packages (must have pyserial/tkinter installed)
import serial
import time
#from tkinter import *

print("Starting connection")

#Change 'COM_' to match according to Arduino properties in your computer's devices menu
ser = serial.Serial('/dev/cu.usbmodem14201', 115200, timeout=1.5)
time.sleep(1)

#Initialized valve lists used in loops below
valves = range(1, 7)
valveVar = []


###Functions
#Communicates to Arduino by sending a string converted to Unicode format
def command(string):
    ser.flush()
    ser.write(string.encode('utf-8'))
    data_received = ser.readline()
    return data_received


#Pump Pin = 0
def pump_off():
    reply = str(command("PUMP OFF").decode('unicode_escape'))
    reply = reply[:-2]
    return reply


#Pump Pin = 128
def pump_on_low():
    reply = str(command("PUMP LOW").decode('unicode_escape'))
    reply = reply[:-2]
    return reply


#Pump Pin = 255
def pump_on_high():
    reply = str(command("PUMP HIGH").decode('unicode_escape'))
    reply = reply[:-2]
    return reply


#Closes Valve Number (___) from 1-6
def close_valve(val_num):
    reply = str(command("CLOSE V" + str(val_num)).decode('unicode_escape'))
    reply = reply[:-2]
    return reply


#Opens Valve Number (___) from 1-6
def open_valve(val_num):
    reply = str(command("OPEN V" + str(val_num)).decode('unicode_escape'))
    reply = reply[:-2]
    return reply



#Closes Control Window
def close_window():
    for m in valves:
        if valveVar[m-1].get() == 1:
            close_valve(m)
    pump_off()
    
    
def valve_arr(val_num):
    reply = str(command("VALVE " + str(val_num)).decode('unicode_escape'))
    reply = reply[:-2]
    return reply    

def pump_speed(speed):
    reply = str(command("SPEED " + str(speed)).decode('unicode_escape'))
    reply = reply[:-2]
    return reply



class GUI:
    def __init__(self, master):
        
       
        self.master = master
        master.title("Fluid Cell Control Software")
        
        self.label1=Frame()
        self.label1.pack()
        
        self.valve_label = Label(self.label1,text="Valve Control")
        self.valve_label.pack()
        
        self.frame1 = Frame(height=2, bd=1, relief=SUNKEN)
        self.frame1.pack(fill=X, padx=5, pady=5)

        
        self.var1 = IntVar()
        self.check1 = Checkbutton(self.frame1, text = "1", variable = self.var1,command = self.update_valve)
        self.check1.grid(row=0,column=0)
        
        self.var2 = IntVar()
        self.check2 = Checkbutton(self.frame1, text = "2", variable = self.var2,command = self.update_valve)
        self.check2.grid(row=0,column=1)
        
        self.var3 = IntVar()
        self.check3 = Checkbutton(self.frame1, text = "3", variable = self.var3,command = self.update_valve)
        self.check3.grid(row=0,column=2)
        
        self.var4 = IntVar()
        self.check4 = Checkbutton(self.frame1, text = "4", variable = self.var4,command = self.update_valve)
        self.check4.grid(row=1,column=0)
        
        self.var5 = IntVar()
        self.check5 = Checkbutton(self.frame1, text = "5", variable = self.var5,command = self.update_valve)
        self.check5.grid(row=1,column=1)
        
        self.var6 = IntVar()
        self.check6 = Checkbutton(self.frame1, text = "6", variable = self.var6,command = self.update_valve)
        self.check6.grid(row=1,column=2)
        
        self.apply_button = Button(self.frame1, text="Apply", command=self.print_valve)
        self.apply_button.grid(row= 2,column=1)
        
        self.label2 = Frame()
        self.label2.pack()
    
        
        self.speed_label = Label(self.label2, text="Speed Control")
        self.speed_label.pack()
        
        self.frame2 = Frame(height=2, bd=1, relief=SUNKEN)
        self.frame2.pack(fill=X, padx=5, pady=5)
        
        self.entry_label = Label(self.frame2,text='Enter Speed (0-255):')
        self.entry_label.pack()
        
        self.speed = StringVar()
        self.speed_entry = Entry(self.frame2, textvariable=self.speed)
        self.speed_entry.pack()
        
        
        self.speed_scale = Scale(self.frame2, from_=0,to=255, variable=self.speed,orient=HORIZONTAL)
        self.speed_scale.pack()
        
        self.apply_speed = Button(self.frame2, text="Apply", command = self.print_speed)
        self.apply_speed.pack()
        
        self.stop_button = Button(self.frame2,text="Stop",command=self.stop)
        self.stop_button.pack()
        
        

        self.close_button = Button(master, text="Close", command=master.quit)
        self.close_button.pack()

    
    
        
    def update_valve(self):
        return(str(self.var1.get())+str(self.var2.get())+str(self.var3.get())+str(self.var4.get())+str(self.var5.get())+str(self.var6.get()))
    
    def print_valve(self):
        valve_arr(GUI.update_valve(self))
        
    def print_speed(self):
        s = format(int(self.speed.get()),"08b")
        pump_speed(s)

    def stop(self):
        self.speed.set("0")
        s = format(0,"08b")
        pump_speed(s)


        


root = Tk()
my_gui = GUI(root)
my_gui.print_speed()
root.mainloop()

print(my_gui.update_valve)


