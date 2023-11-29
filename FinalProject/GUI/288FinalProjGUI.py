# Author: Phillip Jones
# Date: 10/30/2023
# Description: Client starter code that combines: 1) Simple GUI, 2) creation of a thread for
#              running the Client socket in parallel with the GUI, and 3) Simple recieven of mock sensor 
#              data for a server/cybot.for collecting data from the cybot.

# General Python tutorials (W3schools):  https://www.w3schools.com/python/

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
import serial
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket
import tkinter as tk # Tkinter GUI library
# Thread library: https://www.geeksforgeeks.org/how-to-use-thread-in-tkinter-python/
import threading
import os  # import function for finding absolute path to this python script

##### START Define Functions  #########



# Main: Mostly used for setting up, and starting the GUI
def main():


        global window  # Made global so quit function (send_quit) can access
        window = tk.Tk() # Create a Tk GUI Window
        window.title("Purchase Ice Cream")

        # Submit
        submit_Button = tk.Button(text = "Submit", command = show_cart)

        # Last command label  
        global Last_command_Label  # Made global so that Client function (socket_thread) can modify
        Last_command_Label = tk.Label(text="Last Command Sent: ") # Create a Label
        Last_command_Label.grid(row=13) # Pack the label into the window for display

        # Quit command Button
        quit_command_Button = tk.Button(text ="Press to Quit", command = send_quit)
        quit_command_Button.grid(row=16)  # Pack the button into the window for display

        # Cybot Scan command Button
        scan_command_Button = tk.Button(text ="Press to Scan", command = send_scan)
        scan_command_Button.grid(row=14) # Pack the button into the window for display

        # Create a Thread that will run a fuction assocated with a user defined "target" function.
        # In this case, the target function is the Client socket code
        my_thread = threading.Thread(target=socket_thread) # Creat the thread
        my_thread.start() # Start the thread

        # Start event loop so the GUI can detect events such as button clicks, key presses, etc.
        window.mainloop()

       



# Quit Button action.  Tells the client to send a Quit request to the Cybot, and exit the GUI
def send_quit():
        global gui_send_message # Command that the GUI has requested be sent to the Cybot
        global window  # Main GUI window
        
        gui_send_message = "quit\n"   # Update the message for the Client to send
        time.sleep(1)
        window.destroy() # Exit GUI


# Scan Button action.  Tells the client to send a scan request to the Cybot
def send_scan():
        global gui_send_message # Command that the GUI has requested sent to the Cybot

        
        gui_send_message = "M\n"   # Update the message for the Client to send



# Client socket code (Run by a thread created in main)
def socket_thread():
        # Define Globals
        global Last_command_Label # GUI label for displaying the last command sent to the Cybot
        global gui_send_message   # Command that the GUI has requested be sent to the Cybot

        # A little python magic to make it more convient for you to adjust where you want the data file to live
        # Link for more info: https://towardsthecloud.com/get-relative-path-python 
        absolute_path = os.path.dirname(__file__) # Absoult path to this python script
        relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
        full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
        filename = 'sensor-scan.txt' # Name of file you want to store sensor data from your sensor scan command

        # Choose to create either a UART or TCP port socket to communicate with Cybot (Not both!!)
        # UART BEGIN
        #cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!)
        # UART END

        # TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
        HOST = "192.168.1.1"  # The server's hostname or IP address
        PORT = 288        # The port used by the server
        cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
        cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
                      
        cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
        # TCP Socket END

        # Send some text: Either 1) Choose "Hello" or 2) have the user enter text to send
        # send_message = "Hello\n"                            # 1) Hard code message to "Hello", or
        send_message = input("Enter a message:") + '\n'   # 2) Have user enter text
        # send_message = cybot.readline() + '\n'
        # print(send_message.encode())
        gui_send_message = "wait\n"  # Initialize GUI command message to wait                                

        cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

        print("Sent to server: " + send_message) 

        # Send messges to server until user sends "quit"
        while send_message != 'quit\n':
                
                # Update the GUI to display command being sent to the CyBot
                command_display = "Last Command Sent:\t" + send_message
                Last_command_Label.config(text = command_display)  
        
                # Check if a sensor scan command has been sent
                if (send_message == "M\n") or (send_message == "m\n"):

                        print("Requested Sensor scan from Cybot:\n")
                        rx_message = bytearray(1) # Initialize a byte array

                        # Create or overwrite existing sensor scan data file
                        file_object = open(full_path + filename,'w') # Open the file: file_object is just a variable for the file "handler" returned by open()

                        while (rx_message.decode() != "END\n"): # Collect sensor data until "END" recieved
                                rx_message = cybot.readline()   # Wait for sensor response, readline expects message to end with "\n"
                                file_object.write(rx_message.decode())  # Write a line of sensor data to the file
                                print(rx_message.decode()) # Convert message from bytes to String (i.e., decode), then print

                        file_object.close() # Important to close file once you are done with it!!                

                else:                
                        # test value for order
                        order = 'c4'

                        while order != 'x0': # x0 means that current order ends
                                print("Waiting for server reply\n")

                                # start = cybot.readline().decode()     # Wait for a message, readline expects message to end with "\n"
                                # start = start[1:len(start) - 1]
                                start = 'g'

                                print("Got a message from server: " + start) # Convert message from bytes to String (i.e., decode)

                                # cybot.readline() # skip dumb stupid unicode arrow thing

                                # order = cybot.readline().decode()
                                # order = order[:len(order) - 1]
                                # order = 'c4'


                                print("order: " + order)
                                flavor = order[:1]
                                print("flavor: " + flavor)
                                qty = order[1:].strip()
                                print("quantity: " + qty + '\n')
                                add_item(flavor, qty)
                                order = input("Enter order: ") # for testing

                                # while (1):
                                #         order = cybot.readline().decode()
                                #         print(order)
                                #         flavor = order[:1]
                                #         qty = order[1:].strip()
                                #         add_item(flavor, qty)
                        print("End of order")
                        reset_cart()
                        

                # Choose either: 1) Idle wait, or 2) Request a periodic status update from the Cybot
                # 1) Idle wait: for gui_send_message to be updated by the GUI
                while gui_send_message == "wait\n": 
                        time.sleep(.1)  # Sleep for .1 seconds
                send_message = gui_send_message

                # 2) Request a periodic Status update from the Cybot:
                # every .1 seconds if GUI has not requested to send a new command
                #time.sleep(.1)
                #if(gui_send_message == "wait\n"):   # GUI has not requested a new command
                #        send_message = "status\n"   # Request a status update from the Cybot
                #else:
                #        send_message = gui_send_message  # GUI has requested a new command

                gui_send_message = "wait\n"  # Reset gui command message request to wait                        

                cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server
                
        print("Client exiting, and closing file descriptor, and/or network socket\n")
        time.sleep(2) # Sleep for 2 seconds
        cybot.close() # Close file object associated with the socket or UART
        cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)

##### END Define Functions  #########

user_cart = {
        "chocolate" : 0,
        "vanilla" : 0,
        "mint" : 0,
        "tea" : 0
}

total_sold = {
        "chocolate" : 0,
        "vanilla" : 0,
        "mint" : 0,
        "tea" : 0
}



def show_cart(qty):
        print("updating cart...")
        order_str = "Cart: \n"
        global total
        total = 0
        for key, val in user_cart.items():
                order_str += str(key + ": " + str(val) + '\n')
                if key == 'tea':
                        total += (user_cart["tea"] * 3.50)
                elif key == 'vanilla':
                        total += (user_cart["vanilla"] * 2.00)
                elif key == 'chocolate':
                        total += (user_cart["chocolate"] * 2.00)
                elif key == 'mint':
                        total += (user_cart["mint"] * 2.00)
        order_str += "Total: $" + str(total)
        cart_items = tk.Label(window, text="")
        cart_items.grid(row=10)
        cart_items.config(text=order_str)

def add_item(f, q):
        print("adding item...")
        q = int(q)
        if (f == 'c'):
                user_cart["chocolate"] += q
                total_sold["chocolate"] += q
        elif (f == 'v'):
                user_cart["vanilla"] += q
                total_sold["vanilla"] += q
        elif (f == 'm'):
                user_cart["mint"] += q
                total_sold["mint"] += q
        elif (f == 't'):
                user_cart["tea"] += q
                total_sold["tea"] += q
        show_cart(q)

def reset_cart():
        for v in user_cart.values():
                v = 0

### Run main ###
main()
