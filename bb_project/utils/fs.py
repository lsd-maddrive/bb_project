from datetime import datetime


def get_date_string() -> str:
    return datetime.today().strftime("%m_%d_%H_%M_%S")
