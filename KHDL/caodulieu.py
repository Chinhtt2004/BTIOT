import pandas as pd

file_path = r"C:\Users\ASUS_PC\Downloads\heart_2022_with_nans.csv"
df = pd.read_csv(file_path)

print(df.head())
