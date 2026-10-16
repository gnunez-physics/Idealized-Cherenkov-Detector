import pandas as pd

# 1. Cargar el dataset actual de WebPlotDigitizer
df = pd.read_csv("Default Dataset.csv", header=None)
df.columns = ['Wavelength', 'Efficiency_Percent']

# 2. Convertir la eficiencia de porcentaje (%) a fracción (0.0 a 1.0)
df['Efficiency'] = df['Efficiency_Percent'] / 100.0

# 3. Opcional: ordenar de menor a mayor longitud de onda
df = df.sort_values(by='Wavelength').reset_index(drop=True)

# 4. Guardar únicamente las dos columnas que lee Geant4 (Longitud de onda y Eficiencia fraccionaria)
df[['Wavelength', 'Efficiency']].to_csv("eff.dat", sep='\t', index=False, header=False)

print("¡Archivo 'eff.dat' generado con éxito y listo para tu simulación!")
