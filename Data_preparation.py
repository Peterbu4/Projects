import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder, StandardScaler
from sklearn.model_selection import train_test_split
import pickle

# 1. Wczytanie surowych danych
df = pd.read_csv("globalAirQuality.csv")

# 2. Usuwanie zbędnych kolumn
df = df.drop(columns=["country"])

# 3. Cechy czasowe z timestamp
df["timestamp"] = pd.to_datetime(df["timestamp"])
df["hour"] = df["timestamp"].dt.hour
df["day"] = df["timestamp"].dt.day
df["month"] = df["timestamp"].dt.month
df["weekday"] = df["timestamp"].dt.weekday
df = df.drop(columns=["timestamp"])

# 4. Kodowanie miasta
label_encoder = LabelEncoder()
df["city_encoded"] = label_encoder.fit_transform(df["city"])
df = df.drop(columns=["city"])

with open("city_label_encoder.pkl", "wb") as f:
    pickle.dump(label_encoder, f)

# 5. Przygotowanie wejść i wyjść
features = [
    "latitude", "longitude",
    "pm25", "pm10", "no2", "so2", "o3", "co",
    "temperature", "humidity", "wind_speed",
    "hour", "day", "month", "weekday"
]

X = df[features]
y_reg = df["aqi"]
y_cls = df["city_encoded"]

df_corr = X.copy()
df_corr["aqi"] = y_reg

corr_matrix = df_corr.corr()

print("\nKorelacje cech z AQI (przed skalowaniem)\n")
print(corr_matrix["aqi"].sort_values(ascending=False))

# 6. Skalowanie cech
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

with open("feature_scaler.pkl", "wb") as f:
    pickle.dump(scaler, f)

# 7. Podział na 80% train / 20% temp
X_train, X_temp, y_reg_train, y_reg_temp, y_cls_train, y_cls_temp = train_test_split(
    X_scaled, y_reg, y_cls, test_size=0.20, random_state=42, shuffle=True
)

# 8. Podział temp na 10% validation / 10% test
X_val, X_test, y_reg_val, y_reg_test, y_cls_val, y_cls_test = train_test_split(
    X_temp, y_reg_temp, y_cls_temp, test_size=0.5, random_state=42, shuffle=True
)

# 9. Zapis zbiorów do CSV
pd.DataFrame(X_train).to_csv("X_train.csv", index=False)
pd.DataFrame(X_val).to_csv("X_val.csv", index=False)
pd.DataFrame(X_test).to_csv("X_test.csv", index=False)

pd.DataFrame(y_reg_train).to_csv("y_reg_train.csv", index=False)
pd.DataFrame(y_reg_val).to_csv("y_reg_val.csv", index=False)
pd.DataFrame(y_reg_test).to_csv("y_reg_test.csv", index=False)

pd.DataFrame(y_cls_train).to_csv("y_cls_train.csv", index=False)
pd.DataFrame(y_cls_val).to_csv("y_cls_val.csv", index=False)
pd.DataFrame(y_cls_test).to_csv("y_cls_test.csv", index=False)

print("Dane przygotowane, zakodowane, przeskalowane i zapisane.")
# Wczytanie LabelEncoder
with open("city_label_encoder.pkl", "rb") as f:
    label_encoder = pickle.load(f)

# Wyświetlenie wszystkich miast i odpowiadających numerów
print("\n Mapowanie numer ↔ miasto")
for idx, city in enumerate(label_encoder.classes_):
    print(f"{idx} : {city}")
