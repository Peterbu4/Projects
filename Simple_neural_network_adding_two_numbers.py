import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers

# Generation training data (adding two numbers)
def generate_data(size=10000):
    X = np.random.rand(size, 2)  # two inputs: numbers in range 0 to 1
    y = np.sum(X, axis=1)        # output: sum of the numbers
    return X, y

# Training and test data
X_train, y_train = generate_data(8000)
X_test, y_test = generate_data(2000)

# Building the model
model = keras.Sequential([
    layers.Dense(10, activation='relu', input_shape=(2,)),
    layers.Dense(10, activation='relu'),
    layers.Dense(1)  # output: one number (sum)
])

# Compiling the model
model.compile(optimizer='adam', loss='mse')

# Training the model
model.fit(X_train, y_train, epochs=10, batch_size=32, validation_split=0.1)

# Testing the model
loss = model.evaluate(X_test, y_test)
print("MSE na danych testowych:", loss)

# Example of use: adding two numbers
a, b = 0.3, 0.7
result = model.predict(np.array([[a, b]]))
print(f"Model przewiduje: {result[0][0]:.4f}, prawidłowy wynik: {a + b}")
