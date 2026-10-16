import pandas as pd
import matplotlib.pyplot as plt

# Cargar el archivo CSV exportado por WebPlotDigitizer
df = pd.read_csv("Default Dataset.csv", header=None)
df.columns = ['Wavelength', 'Value']

# Crear la gráfica
plt.figure(figsize=(9, 6))
plt.plot(df['Wavelength'], df['Value'], marker='o', linestyle='-', color='b', markersize=4)

# Configurar la escala logarítmica en el eje Y
plt.yscale('log')

# Fijar los límites de los ejes
plt.xlim(200, 800)
plt.ylim(0.001, 1000)  # Desde 10^(-3) hasta 10^2 (100)

# Etiquetas y formato
plt.xlabel('Longitud de onda (nm)', fontsize=12)
plt.ylabel('Eficiencia Cuántica (%)', fontsize=12)
plt.title('Eficiencia Cuántica - Fotomultiplicador R5912-100', fontsize=14)
plt.grid(True, which="both", ls="--", alpha=0.6)

# Guardar y mostrar
plt.savefig('quantum_efficiency_custom_limits.png', dpi=300)
plt.show()