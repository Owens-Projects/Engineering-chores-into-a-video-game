import serial.tools.list_ports
import pygame
from numpy import interp


pygame.init()

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

serialInst.baudrate = 9600
serialInst.port = 'COM4'               #  COM4 for UNO 
serialInst.open()

leftCommand = '00000'
rightCommand = '00000'
StartButton = '0'        # start button on PS4 controller
wipperSnipper = '0'      # X button on PS4 controller
hedgerButton = '0' 

def main():

    ControllerInputs = {}
    clock = pygame.time.Clock()
    leftCommand = '00000'
    rightCommand = '00000'
    StartButton = '0'        # start button on PS4 controller
    wipperSnipper = '0'      # X button on PS4 controller
    hedgerButton = '0'

    
    done = False
    while not done:

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True  # Flag that we are done so we exit this loop.

            if event.type == pygame.JOYDEVICEADDED:
                # This event will be generated when the program starts for every
                # joystick, filling up the list without needing to create them manually.
                controller = pygame.joystick.Joystick(event.device_index)
                ControllerInputs[controller.get_instance_id()] = controller
                print(f"Joystick {controller.get_instance_id()} connected")

        # For each joystick:
        
        for contInputs in ControllerInputs.values():

            Leftaxis = contInputs.get_axis(1)
            #Drive left motor forward
            if Leftaxis < 0:
                Leftaxis = abs(Leftaxis)
                Leftaxis = int(interp(Leftaxis,[0,1],[0,255]))
                if Leftaxis > 9:
                    LeftFWD = '10'
                    leftCommand = LeftFWD + (str(Leftaxis).zfill(3))
                else:
                    leftCommand = '00000'
            #Drive left motor reverse
            elif Leftaxis > 0:
                Leftaxis = int(interp(Leftaxis,[0,1],[0,255]))
                if Leftaxis > 9:
                    LeftREV = '01'
                    leftCommand = LeftREV + (str(Leftaxis).zfill(3))
                else:
                    leftCommand = '00000'

            Rightaxis = contInputs.get_axis(3)
            #Drive right motor forward
            if Rightaxis < 0:
                Rightaxis = abs(Rightaxis)
                Rightaxis = int(interp(Rightaxis,[0,1],[0,255]))
                if Rightaxis > 9:
                    RightFWD = '10'
                    rightCommand = RightFWD + (str(Rightaxis).zfill(3))
                else:
                    rightCommand = '00000'
            #Drive motors in REV direction
            elif Rightaxis > 0:
                Rightaxis = int(interp(Rightaxis,[0,1],[0,255]))
                if Rightaxis > 9:
                    RightREV = '01'
                    rightCommand = RightREV + (str(Rightaxis).zfill(3))
                else:
                    rightCommand = '00000'
        

            StartButton = str(contInputs.get_button(5))          # start button on PS4 controller
            wipperSnipper = str(contInputs.get_button(0))        # X button on PS4 controller
            hedgerButton = str(contInputs.get_button(1))            # O button on Ps4 controller
                
            
        buttonCommand = StartButton + wipperSnipper + hedgerButton

        command = (leftCommand)+(rightCommand)+(buttonCommand) + "x"
    
        #print(command)
        serialInst.write(command.encode('utf-8'))

        # Limits to 7 cycles per second.
        clock.tick(7)


if __name__ == "__main__":
    main()
 
    pygame.quit()