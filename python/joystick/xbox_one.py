import evdev


def joy_init():
    devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
    for i in devices:
        if i.name == 'Xbox One Wireless Controller':
            return i
        return None


def joy_close(joy):
    joy.close()


def get_left_x_axis(joy):
    return -(joy.absinfo(1).value / (joy.absinfo(1).max + 1))


def get_left_y_axis(joy):
    return joy.absinfo(0).value / (joy.absinfo(0).max + 1)


def get_right_y_axis(joy):
    return joy.absinfo(3).value / (joy.absinfo(3).max + 1)


def get_left_trg(joy):
    return joy.absinfo(2).value / (joy.absinfo(2).max + 1)


def get_right_trg(joy):
    return joy.absinfo(5).value / (joy.absinfo(5).max + 1)