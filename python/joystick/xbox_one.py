import evdev


class Joystick:
    def __init__(self):
        devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
        for i in devices:
            if i.name == 'Xbox One Wireless Controller':
                self.device = i
                
    def close(self):
        self.device.close()
        
    def leftXAxis(self)
        return -(self.device.absinfo(1).value / (self.device.absinfo(1).max + 1))
        
    def leftYAxis(self)
        return self.device.absinfo(0).value / (self.device.absinfo(0).max + 1)
        
    def rightYAxis(self)
        return self.device.absinfo(3).value / (self.device.absinfo(3).max + 1)
        
    def leftTrigger(self)
        return self.device.absinfo(2).value / (self.device.absinfo(2).max + 1)
        
    def rightTrigger(self)
        return self.device.absinfo(5).value / (self.device.absinfo(5).max + 1)
