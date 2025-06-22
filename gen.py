import pandas as pd
import matplotlib.pyplot as plt

# Загрузка данных
df = pd.read_csv("results.csv")  # замените на имя вашего файла

# Оформление графика
plt.figure(figsize=(12, 6))

# Стили и цвет
styles = {
    'Linear':     {'label': 'Linear',     'color': 'tab:blue',   'marker': 'o'},
    'BST':        {'label': 'BST',        'color': 'tab:orange', 'marker': 'o'},
    'RBT':        {'label': 'RBT',        'color': 'tab:green',  'marker': 'o'},
    'Hash':       {'label': 'Hash',       'color': 'tab:red',    'marker': 'o'},
    'Multimap':   {'label': 'Multimap',   'color': 'tab:purple', 'marker': 'o'},
}

# Построение линий
for column, style in styles.items():
    plt.plot(df['Size'], df[column],
             label=style['label'],
             color=style['color'],
             marker=style['marker'],
             linewidth=2)

# Настройки осей
plt.yscale('log')
plt.xlabel("Размер массива", fontsize=12)
plt.ylabel("Время поиска (лог. шкала)", fontsize=12)
plt.title("Сравнение алгоритмов поиска", fontsize=14)

# Сетка и легенда
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.legend()
plt.tight_layout()

# Сохранение
plt.savefig("comparison_cleaned.png")
plt.show()