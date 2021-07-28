import time
import logging
from src.xbox_one import Gamepad, GamepadButtons, GamepadAxis
logging.basicConfig(
    level=logging.DEBUG, format="%(asctime)s - %(name)s - %(levelname)s - %(message)s"
)
logger = logging.getLogger(__name__)


def main():
    for _ in range(5):
        dev = Gamepad()
        if dev.name is None:
            logger.debug("Check the gamepad")
            time.sleep(3)
        else:
            break
    if dev.name is None:
        logger.error("Gamepad is unavailable")
        return
    logger.debug("%s is connected", dev.name)
    while not dev.read_button(GamepadButtons.B.value):
        dev.update_buttons()
        if dev.read_button(GamepadButtons.RIGHT_ARROW.value):
            dev.vibrate()
        if dev.read_button(GamepadButtons.A.value):
            print('A')
        time.sleep(0.05)


if __name__ == '__main__':
    main()
