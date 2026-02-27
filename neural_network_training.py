import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras.utils import to_categorical
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay

# 1. WCZYTYWANIE DANYCH
print("Wczytywanie danych...")
try:
    X_train = pd.read_csv("X_train.csv").values
    X_val   = pd.read_csv("X_val.csv").values
    X_test  = pd.read_csv("X_test.csv").values

    y_reg_train = pd.read_csv("y_reg_train.csv").squeeze().values
    y_reg_val   = pd.read_csv("y_reg_val.csv").squeeze().values
    y_reg_test  = pd.read_csv("y_reg_test.csv").squeeze().values

    y_cls_train = pd.read_csv("y_cls_train.csv").squeeze().values
    y_cls_val   = pd.read_csv("y_cls_val.csv").squeeze().values
    y_cls_test  = pd.read_csv("y_cls_test.csv").squeeze().values

    # Przygotowanie klasyfikacji (One-hot encoding)
    num_classes = len(np.unique(y_cls_train))
    y_cls_train_cat = to_categorical(y_cls_train, num_classes)
    y_cls_val_cat   = to_categorical(y_cls_val, num_classes)
    y_cls_test_cat  = to_categorical(y_cls_test, num_classes)
    
    # Obliczenie wariancji bazowej dla R^2
    baseline_mse = np.mean((y_reg_test - np.mean(y_reg_test))**2)

    print(f"Dane wczytane pomyślnie. Liczba miast: {num_classes}")
except FileNotFoundError as e:
    print(f"Błąd: Nie znaleziono plików CSV. Upewnij się, że są w tym samym folderze. {e}")
    exit()

# 2. FUNKCJE POMOCNICZE

def format_config(conf):
    """Formatuje listę neuronów na czytelny string bez nawiasów i przecinków."""
    return " ".join(str(x) for x in conf)

def build_multitask_model(layer_sizes, input_dim, num_classes):
    """Buduje model z dwoma wyjściami: regresją AQI i klasyfikacją miasta."""
    inputs = keras.Input(shape=(input_dim,), name="features")
    
    # Warstwy ukryte (wspólne)
    x = layers.Dense(layer_sizes[0], activation="relu")(inputs)
    for size in layer_sizes[1:]:
        x = layers.Dense(size, activation="relu")(x)

    # Wyjście regresyjne (AQI)
    out_reg = layers.Dense(1, name="aqi")(x)

    # Wyjście klasyfikacyjne (City)
    out_cls = layers.Dense(num_classes, activation="softmax", name="city")(x)

    model = keras.Model(inputs=inputs, outputs=[out_reg, out_cls])
    
    model.compile(
        optimizer="adam",
        loss={"aqi": "mse", "city": "categorical_crossentropy"},
        metrics={"aqi": ["mae"], "city": ["accuracy"]},
        loss_weights={"aqi": 0.2, "city": 0.8} # Balans między zadaniami dający najlepsze wyniki
    )
    return model

# 3. KONFIGURACJE SIECI

configs = [
    [8],
    [16],
    [32],
    [64],
    
    [16, 8],
    [32, 16],
    [64, 32],
    [128, 64],

    [16, 8, 4],
    [32, 16, 8],
    [64, 32, 16],
    [128, 64, 32],

    [64, 32, 16, 8],
    [128, 64, 32, 16],
    [256, 128, 64, 32],

    [256, 128, 64, 32, 16]
]

results = []
histories = {}
trained_models = {}

early_stop = keras.callbacks.EarlyStopping(
    monitor="val_loss",
    patience=8,
    restore_best_weights=True
)

# 4. PĘTLA TRENOWANIA

for conf in configs:
    conf_label = format_config(conf)
    print(f"\n[TESTOWANIE ARCHITEKTURY: {conf_label}]")
    
    model = build_multitask_model(conf, X_train.shape[1], num_classes)
    
    history = model.fit(
        X_train, 
        {"aqi": y_reg_train, "city": y_cls_train_cat},
        validation_data=(X_val, {"aqi": y_reg_val, "city": y_cls_val_cat}),
        epochs=150,
        batch_size=32,
        callbacks=[early_stop],
        verbose=1
    )

    histories[conf_label] = history
    trained_models[conf_label] = model

    # Ocena na zbiorze testowym
    eval_metrics = model.evaluate(X_test, {"aqi": y_reg_test, "city": y_cls_test_cat}, verbose=0)
  
    total_loss = eval_metrics[0]
    mse_aqi    = eval_metrics[1]
    ce_city    = eval_metrics[2]   # CROSS-ENTROPY
    mae_aqi    = eval_metrics[3]
    acc_city   = eval_metrics[4] * 100
  
    r2_pct = max(0, (1-mse_aqi/baseline_mse))*100

    results.append({
        "Konfiguracja": conf_label,
        "Total_Loss": eval_metrics[0],
        "CE_CITY": ce_city,
        "MAE_AQI": eval_metrics[3],
        "R2 %": r2_pct,
        "City_Acc_%": eval_metrics[4] * 100
    })

# 5. ANALIZA WYNIKÓW I ZAPIS NAJLEPSZYCH

df_results = pd.DataFrame(results)
# Sortowanie po Total_Loss (balans regresji i klasyfikacji)
df_sorted = df_results.sort_values(by="Total_Loss").reset_index(drop=True)

best_config = df_sorted.iloc[0]["Konfiguracja"]
best_model = trained_models[best_config]

# BŁĘDY REGRESJI AQI
y_pred_aqi = best_model.predict(X_test)[0].squeeze()
errors = y_pred_aqi - y_reg_test

plt.figure(figsize=(8, 5))
plt.hist(errors, bins=40)
plt.title(f"Rozkład błędów AQI – {best_config}")
plt.xlabel("Błąd (Pred - True)")
plt.ylabel("Liczba próbek")
plt.grid(alpha=0.3)
plt.show()

# Predykcja
y_pred_cls = np.argmax(best_model.predict(X_test)[1], axis=1)

# Macierz trafień – znormalizowana
cm = confusion_matrix(
    y_cls_test,
    y_pred_cls,
    normalize="true"
)

fig, ax = plt.subplots(figsize=(10, 10))

im = ax.imshow(cm, cmap="Blues")

# Opisy osi
ax.set_title(f"Znormalizowana macierz trafień – {best_config}", fontsize=14)
ax.set_xlabel("Klasa przewidziana", fontsize=12)
ax.set_ylabel("Klasa rzeczywista", fontsize=12)

# Tiki osi
ax.set_xticks(np.arange(num_classes))
ax.set_yticks(np.arange(num_classes))

ax.set_xticklabels(np.arange(num_classes), fontsize=8, rotation=45, ha="right")
ax.set_yticklabels(np.arange(num_classes), fontsize=8)

# Pasek kolorów
cbar = fig.colorbar(im, ax=ax)
cbar.set_label("Udział próbek", fontsize=11)

ax.grid(False)
plt.tight_layout()
plt.show()

print("\n" + "="*105)
print(f"{'ARCHITEKTURA (NEURONY)':<25} | {'STRATA ŁĄCZNA':<15} | {'MAE AQI':<10} | {'R2 %':<10} | {'ACC CITY':<9} | {'Cross-entropy'}")
print("-" * 105)
for _, row in df_sorted.iterrows():
    print(f"{row['Konfiguracja']:<25} | {row['Total_Loss']:<15.4f} | {row['MAE_AQI']:<10.2f} | {row['R2 %']:<10.2f} | {row['City_Acc_%']:8.2f}% | {row['CE_CITY']:<10.2f}")
print("="*105)

# Zapisywanie 2 najlepszych konfiguracji
print("\n>>> Zapisywanie dwóch najlepszych modeli do plików .keras...")
for i in range(min(2, len(df_sorted))):
    best_cfg_str = df_sorted.iloc[i]["Konfiguracja"]
    # Zamiana spacji na podkreślnik dla nazwy pliku
    filename = f"model_miejsce_{i+1}_{best_cfg_str.replace(' ', '_')}.keras"
    trained_models[best_cfg_str].save(filename)
    print(f"Zapisano: {filename}")

# 6. WIZUALIZACJA (BEST, MID, WORST)

def plot_history(config_label, status):
    h = histories[config_label].history
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 5))
    
    # Wykres regresji
    ax1.plot(h["aqi_mae"], label="Trening")
    ax1.plot(h["val_aqi_mae"], label="Walidacja", linestyle='--')
    ax1.set_title(f"{status}: {config_label}\nBłąd AQI (MAE)")
    ax1.set_xlabel("Epoka")
    ax1.legend()
    ax1.grid(True, alpha=0.3)

    # Wykres klasyfikacji
    ax2.plot(h["city_accuracy"], label="Trening")
    ax2.plot(h["val_city_accuracy"], label="Walidacja", linestyle='--')
    ax2.set_title(f"{status}: {config_label}\nSkuteczność Miasta (Accuracy)")
    ax2.set_xlabel("Epoka")
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()

# Wyznaczenie indeksów
best_idx = 0
mid_idx = len(df_sorted) // 2
worst_idx = len(df_sorted) - 1

# Generowanie wykresów
plot_history(df_sorted.iloc[best_idx]["Konfiguracja"], "NAJLEPSZY (BEST)")
plot_history(df_sorted.iloc[mid_idx]["Konfiguracja"], "UMIARKOWANY (MID)")
plot_history(df_sorted.iloc[worst_idx]["Konfiguracja"], "NAJGORSZY (WORST)")
