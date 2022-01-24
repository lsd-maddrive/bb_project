import os
from pathlib import Path

import pandas as pd
from matplotlib import pyplot

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.abspath(os.path.join(CURRENT_DIR, os.pardir))
LOG_DIR = os.path.join(PROJECT_DIR, "logs")

pyplot.style.use("ggplot")  # Красивые графики
pyplot.rcParams["figure.figsize"] = (15, 5)  # Размер картинок


def main():
    csv_fpath = os.path.join(LOG_DIR, "biostats.csv")
    if ".csv" not in csv_fpath:
        all_csv_fpaths = sorted(list(Path(LOG_DIR).glob(f"**{os.sep}*.csv")))
        csv_fpath = all_csv_fpaths[-1]

    data = pd.read_csv(csv_fpath, sep=",")
    # TODO: use seaborn
    data["time"].plot(figsize=(15, 10))
    data["val1"].plot(figsize=(15, 10))


if __name__ == "__main__":
    main()
