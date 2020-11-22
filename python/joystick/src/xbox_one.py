import evdev


class Joystick:
    """
    Class of the gamepad. Search for device with 'Xbox' in name. 
    It's assumed that there'll be only one such device and it'd be gamepad. 
    Should work with wired gamepad as well as with wireless one.
    Usage: dev = xbox_one.Joystick()
    """
    def __init__(self):
        devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
        for i in devices:
            if ('Xbox' in i.name) or ('X-Box' in i.name):
                self.device = i
                self.xStatus = False
                self.xFlag = False

    def close(self):
        self.device.close()

    def update_buttons(self):
        if (not self.xFlag) and (307 in self.device.active_keys()):
            self.xStatus = True
            self.xFlag = True
        elif self.xFlag and (not self.xStatus):
            if 307 not in self.device.active_keys():
                self.xFlag = False
        
    def left_x_axis(self):
        return -(self.device.absinfo(1).value / (self.device.absinfo(1).max + 1))
        
    def left_y_axis(self):
        return self.device.absinfo(0).value / (self.device.absinfo(0).max + 1)
        
    def right_y_axis(self):
        return self.device.absinfo(3).value / (self.device.absinfo(3).max + 1)
        
    def left_trigger(self):
        return self.device.absinfo(2).value / (self.device.absinfo(2).max + 1)
        
    def right_trigger(self):
        return self.device.absinfo(5).value / (self.device.absinfo(5).max + 1)

    def x_button(self):
        if self.xStatus:
            self.xStatus = False
            return True
        return False
