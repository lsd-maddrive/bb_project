import pandas
from matplotlib import pyplot
from os import listdir

pyplot.style.use('ggplot')  # Красивые графики
pyplot.rcParams['figure.figsize'] = (15, 5)  # Размер картинок

file_to_plot = ''

csv_file = '../Logs/' + file_to_plot
if csv_file == '../Logs/':
    all_files = [f for f in listdir(csv_file) if f.endswith('.csv')]
    csv_file = ''.join([csv_file, all_files[-1]])

log = pandas.read_csv(csv_file, sep=',', encoding='latin1', index_col='time')
log['par1'].plot(figsize=(15, 10))
