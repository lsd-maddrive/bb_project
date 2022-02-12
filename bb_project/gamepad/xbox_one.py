import evdev
import logging
from enum import IntEnum


class ExternalDeviceNotFound(IOError):
    pass


class GamepadAxis(IntEnum):
    """ "
    Enum for analog axes of XBOX One gamepad
    """

    LEFT_Y_AXIS = 0
    LEFT_X_AXIS = 1
    LEFT_TRIGGER = 2
    RIGHT_Y_AXIS = 3
    RIGHT_X_AXIS = 4
    RIGHT_TRIGGER = 5


class GamepadButtons(IntEnum):
    """
    Enum for buttons of XBOX One gamepad
    """

    A = 304
    B = 305
    X = 307
    Y = 308
    LB = 310
    RB = 311
    START = 315
    SELECT = 314
    MAIN = 316
    LEFT_STICK = 317
    RIGHT_STICK = 318
    UPPER_ARROW = 0
    LOWER_ARROW = 1
    LEFT_ARROW = 2
    RIGHT_ARROW = 3


class Gamepad:
    """
    Class of the gamepad. Search for device with 'Xbox' in name.
    It's assumed that there'll be only one such device and it'd be gamepad.
    Should work with wired gamepad as well as with wireless one.
    Usage: dev = xbox_one.Gamepad()
    """

    def __init__(self):
        self._logger = logging.getLogger(self.__class__.__name__)
        self.name = None
        devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
        for i in devices:
            if ("Xbox" in i.name) or ("X-Box" in i.name):
                self.device = i
                self.name = self.device.name
                break
        if self.name is None:
            raise ExternalDeviceNotFound("No gamepad found")
        self._logger.debug("%s is connected", self.name)
        self.key_states = {}
        for i in GamepadButtons:
            self.key_states[i.name] = {}
            self.key_states[i.name]["value"] = False
            self.key_states[i.name]["status"] = False
        self._setup_vibration()

    def _setup_vibration(self, duration=500):
        """
        Sets the type and duration of vibration
        """
        rumble = evdev.ff.Rumble(strong_magnitude=0x0000, weak_magnitude=0xFFFF)
        effect_type = evdev.ff.EffectType(ff_rumble_effect=rumble)
        duration_ms = duration
        effect = evdev.ff.Effect(
            evdev.ecodes.FF_RUMBLE,
            -1,
            0,
            evdev.ff.Trigger(0, 0),
            evdev.ff.Replay(duration_ms, 0),
            effect_type,
        )
        self.effect_id = self.device.upload_effect(effect)

    def vibrate(self):
        """
        Implement one-time vibration
        """
        self.device.write(evdev.ecodes.EV_FF, self.effect_id, 1)

    def close(self):
        """
        release the device
        """
        self.device.close()

    def _normalize(self, axis):
        """
        Normalize analog axis values to [0 - 1] range
        """
        return self.device.absinfo(axis).value / (self.device.absinfo(axis).max + 1)

    def _check_button_state(self, key):
        """
        Check and update button state preventing multiple presses if button is on hold
        """
        if key in self.device.active_keys():
            if not self.key_states[GamepadButtons(key).name]["status"]:
                self.key_states[GamepadButtons(key).name]["value"] = True
                self.key_states[GamepadButtons(key).name]["status"] = True
        elif (
            not self.key_states[GamepadButtons(key).name]["value"]
            and self.key_states[GamepadButtons(key).name]["status"]
        ):
            self.key_states[GamepadButtons(key).name]["status"] = False

    def update_buttons(self):
        """
        Update button state (cross buttons are actually axes so a bit of spaghetti code
        """
        for i in list(map(int, GamepadButtons)):
            if i > 300:
                self._check_button_state(i)

        if self.device.absinfo(17).value == -1:
            if not self.key_states[GamepadButtons(0).name]["status"]:
                self.key_states[GamepadButtons(0).name]["value"] = True
                self.key_states[GamepadButtons(0).name]["status"] = True
        elif (
            not self.key_states[GamepadButtons(0).name]["value"]
            and self.key_states[GamepadButtons(0).name]["status"]
        ):
            self.key_states[GamepadButtons(0).name]["status"] = False

        if self.device.absinfo(17).value == 1:
            if not self.key_states[GamepadButtons(1).name]["status"]:
                self.key_states[GamepadButtons(1).name]["value"] = True
                self.key_states[GamepadButtons(1).name]["status"] = True
        elif (
            not self.key_states[GamepadButtons(1).name]["value"]
            and self.key_states[GamepadButtons(1).name]["status"]
        ):
            self.key_states[GamepadButtons(1).name]["status"] = False

        if self.device.absinfo(16).value == -1:
            if not self.key_states[GamepadButtons(2).name]["status"]:
                self.key_states[GamepadButtons(2).name]["value"] = True
                self.key_states[GamepadButtons(2).name]["status"] = True
        elif (
            not self.key_states[GamepadButtons(2).name]["value"]
            and self.key_states[GamepadButtons(2).name]["status"]
        ):
            self.key_states[GamepadButtons(2).name]["status"] = False

        if self.device.absinfo(16).value == 1:
            if not self.key_states[GamepadButtons(3).name]["status"]:
                self.key_states[GamepadButtons(3).name]["value"] = True
                self.key_states[GamepadButtons(3).name]["status"] = True
        elif (
            not self.key_states[GamepadButtons(3).name]["value"]
            and self.key_states[GamepadButtons(3).name]["status"]
        ):
            self.key_states[GamepadButtons(3).name]["status"] = False

    def read_button(self, key):
        """
        Read state of the button and unset it
        """
        if self.key_states[GamepadButtons(key).name]["value"]:
            self.key_states[GamepadButtons(key).name]["value"] = False
            return True
        return False

    def get_axis_value(self, axis):
        """
        Read normalized axis value
        """
        if type(axis) is int:
            if 0 <= axis < 6:
                if (
                    axis == GamepadAxis.LEFT_X_AXIS.value
                    or axis == GamepadAxis.RIGHT_X_AXIS.value
                ):
                    return -self._normalize(axis)
                return self._normalize(axis)
        raise ValueError("Not an axis")
